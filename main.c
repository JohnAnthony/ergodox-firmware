#include <stdbool.h>
#include <stdint.h>
#include <util/delay.h>
#include "usb_keyboard.h"
#include "main.h"
#include "key_functions.h"
#include "layout.h"
#include "teensy.h"
#include "mcp23018.h"

#define MAX_ACTIVE_LAYERS 20

static bool _main_kb_is_pressed[KB_ROWS][KB_COLUMNS];
bool (*main_kb_is_pressed)[KB_ROWS][KB_COLUMNS] = &_main_kb_is_pressed;

static bool _main_kb_was_pressed[KB_ROWS][KB_COLUMNS];
bool (*main_kb_was_pressed)[KB_ROWS][KB_COLUMNS] = &_main_kb_was_pressed;

static bool main_kb_was_transparent[KB_ROWS][KB_COLUMNS];

uint8_t main_layers_pressed[KB_ROWS][KB_COLUMNS];
uint8_t main_loop_row;
uint8_t main_loop_col;
uint8_t main_arg_layer;
uint8_t main_arg_layer_offset;
uint8_t main_arg_row;
uint8_t main_arg_col;
bool    main_arg_is_pressed;
bool    main_arg_was_pressed;
bool    main_arg_trans_key_pressed;

int main(void) {
	teensy_init();

	kb_led_state_power_on();

	usb_init();
	while (!usb_configured());
	kb_led_delay_usb_init();

	kb_led_state_ready();

	for (;;) {
		bool (*temp)[KB_ROWS][KB_COLUMNS] = main_kb_was_pressed;
		main_kb_was_pressed = main_kb_is_pressed;
		main_kb_is_pressed = temp;

		teensy_update_matrix(*main_kb_is_pressed);
		mcp23018_update_matrix(*main_kb_is_pressed);

		#define row        main_loop_row
		#define col        main_loop_col
		#define layer      main_arg_layer
		#define is_pressed main_arg_is_pressed
		#define was_pressed main_arg_was_pressed
		for (row = 0; row < KB_ROWS; row++) {
			for (col = 0; col < KB_COLUMNS; col++) {
				is_pressed  = (*main_kb_is_pressed)[row][col];
				was_pressed = (*main_kb_was_pressed)[row][col];

				if (is_pressed != was_pressed) {
					if (is_pressed) {
						layer = main_layers_peek(0);
						main_layers_pressed[row][col] = layer;
						main_arg_trans_key_pressed = false;
					} else {
						layer = main_layers_pressed[row][col];
						main_arg_trans_key_pressed = main_kb_was_transparent[row][col];
					}

					main_arg_row          = row;
					main_arg_col          = col;
					main_arg_layer_offset = 0;
					main_exec_key();
					main_kb_was_transparent[row][col] = main_arg_trans_key_pressed;
				}
			}
		}
		#undef row
		#undef col
		#undef layer
		#undef is_pressed
		#undef was_pressed

		usb_keyboard_send();
		usb_extra_consumer_send();
		_delay_ms(MAKEFILE_DEBOUNCE_TIME);

		if (keyboard_leds & (1<<0)) { kb_led_num_on();    } else { kb_led_num_off();    }
		if (keyboard_leds & (1<<1)) { kb_led_caps_on();   } else { kb_led_caps_off();   }
		if (keyboard_leds & (1<<2)) { kb_led_scroll_on(); } else { kb_led_scroll_off(); }
		if (keyboard_leds & (1<<3)) { kb_led_compose_on(); } else { kb_led_compose_off(); }
		if (keyboard_leds & (1<<4)) { kb_led_kana_on();   } else { kb_led_kana_off();   }
	}

	return 0;
}

// --- Layer stack ---

struct layer_entry {
	uint8_t layer;
	uint8_t id;
};

static struct layer_entry layers[MAX_ACTIVE_LAYERS];
static uint8_t layers_head = 0;
static uint8_t layers_ids_in_use[MAX_ACTIVE_LAYERS] = {true};

void main_exec_key(void) {
	void (*key_function)(void) =
		(main_arg_is_pressed)
		? kb_layout_press_get(main_arg_layer, main_arg_row, main_arg_col)
		: kb_layout_release_get(main_arg_layer, main_arg_row, main_arg_col);

	if (key_function)
		(*key_function)();
}

uint8_t main_layers_peek(uint8_t offset) {
	if (offset <= layers_head)
		return layers[layers_head - offset].layer;
	return 0;
}

uint8_t main_layers_push(uint8_t layer) {
	for (uint8_t id = 1; id < MAX_ACTIVE_LAYERS; id++) {
		if (!layers_ids_in_use[id]) {
			layers_ids_in_use[id] = true;
			layers_head++;
			layers[layers_head].layer = layer;
			layers[layers_head].id    = id;
			return id;
		}
	}
	return 0;
}

void main_layers_pop_id(uint8_t id) {
	for (uint8_t i = 1; i <= layers_head; i++) {
		if (layers[i].id == id) {
			for (; i < layers_head; i++) {
				layers[i].layer = layers[i+1].layer;
				layers[i].id    = layers[i+1].id;
			}
			layers[layers_head].layer = 0;
			layers[layers_head].id    = 0;
			layers_ids_in_use[id] = false;
			layers_head--;
			return;
		}
	}
}
