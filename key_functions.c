#include <stdbool.h>
#include <stdint.h>
#include "usb_keyboard.h"
#include "key_functions.h"

#define LAYER        main_arg_layer
#define LAYER_OFFSET main_arg_layer_offset
#define ROW          main_arg_row
#define COL          main_arg_col
#define IS_PRESSED   main_arg_is_pressed

// --- USB key state primitives ---

void _kbfun_press_release(bool press, uint8_t keycode) {
	if (keycode == 0) return;
	switch (keycode) {
		case KEY_LeftControl:  (press) ? (keyboard_modifier_keys |=  (1<<0))
		                               : (keyboard_modifier_keys &= ~(1<<0)); return;
		case KEY_LeftShift:    (press) ? (keyboard_modifier_keys |=  (1<<1))
		                               : (keyboard_modifier_keys &= ~(1<<1)); return;
		case KEY_LeftAlt:      (press) ? (keyboard_modifier_keys |=  (1<<2))
		                               : (keyboard_modifier_keys &= ~(1<<2)); return;
		case KEY_LeftGUI:      (press) ? (keyboard_modifier_keys |=  (1<<3))
		                               : (keyboard_modifier_keys &= ~(1<<3)); return;
		case KEY_RightControl: (press) ? (keyboard_modifier_keys |=  (1<<4))
		                               : (keyboard_modifier_keys &= ~(1<<4)); return;
		case KEY_RightShift:   (press) ? (keyboard_modifier_keys |=  (1<<5))
		                               : (keyboard_modifier_keys &= ~(1<<5)); return;
		case KEY_RightAlt:     (press) ? (keyboard_modifier_keys |=  (1<<6))
		                               : (keyboard_modifier_keys &= ~(1<<6)); return;
		case KEY_RightGUI:     (press) ? (keyboard_modifier_keys |=  (1<<7))
		                               : (keyboard_modifier_keys &= ~(1<<7)); return;
	}
	for (uint8_t i = 0; i < 6; i++) {
		if (press)  { if (keyboard_keys[i] == 0)       { keyboard_keys[i] = keycode; return; } }
		else        { if (keyboard_keys[i] == keycode)  { keyboard_keys[i] = 0;       return; } }
	}
}

bool _kbfun_is_pressed(uint8_t keycode) {
	switch (keycode) {
		case KEY_LeftControl:  if (keyboard_modifier_keys & (1<<0)) return true; break;
		case KEY_LeftShift:    if (keyboard_modifier_keys & (1<<1)) return true; break;
		case KEY_LeftAlt:      if (keyboard_modifier_keys & (1<<2)) return true; break;
		case KEY_LeftGUI:      if (keyboard_modifier_keys & (1<<3)) return true; break;
		case KEY_RightControl: if (keyboard_modifier_keys & (1<<4)) return true; break;
		case KEY_RightShift:   if (keyboard_modifier_keys & (1<<5)) return true; break;
		case KEY_RightAlt:     if (keyboard_modifier_keys & (1<<6)) return true; break;
		case KEY_RightGUI:     if (keyboard_modifier_keys & (1<<7)) return true; break;
	}
	for (uint8_t i = 0; i < 6; i++)
		if (keyboard_keys[i] == keycode) return true;
	return false;
}

// --- Key functions used by the layout ---

void kbfun_press_release(void) {
	uint8_t keycode = kb_layout_get(LAYER, ROW, COL);
	_kbfun_press_release(IS_PRESSED, keycode);
}

void kbfun_transparent(void) {
	main_arg_trans_key_pressed = true;
	LAYER_OFFSET++;
	LAYER = main_layers_peek(LAYER_OFFSET);
	main_layers_pressed[ROW][COL] = LAYER;
	main_exec_key();
}

// Layer 1 push/pop — only layer used beyond the default
static uint8_t layer_id_1 = 0;

void kbfun_layer_push_1(void) {
	uint8_t keycode = kb_layout_get(LAYER, ROW, COL);
	if (layer_id_1) { main_layers_pop_id(layer_id_1); layer_id_1 = 0; }
	layer_id_1 = main_layers_push(keycode);
}

void kbfun_layer_pop_1(void) {
	if (layer_id_1) { main_layers_pop_id(layer_id_1); layer_id_1 = 0; }
}

// Two-key capslock: holding either shift key makes the other toggle caps lock.
void kbfun_2_keys_capslock_press_release(void) {
	static uint8_t keys_pressed;
	static bool lshift_pressed;
	static bool rshift_pressed;

	uint8_t keycode = kb_layout_get(LAYER, ROW, COL);

	if (!IS_PRESSED) keys_pressed--;
	_kbfun_press_release(IS_PRESSED, keycode);

	if (keys_pressed == 1 && IS_PRESSED) {
		lshift_pressed = _kbfun_is_pressed(KEY_LeftShift);
		rshift_pressed = _kbfun_is_pressed(KEY_RightShift);
		_kbfun_press_release(false, KEY_LeftShift);
		_kbfun_press_release(false, KEY_RightShift);
		_kbfun_press_release(true,  KEY_CapsLock);
		usb_keyboard_send();
		_kbfun_press_release(false, KEY_CapsLock);
		usb_keyboard_send();
		if (lshift_pressed) _kbfun_press_release(true, KEY_LeftShift);
		if (rshift_pressed) _kbfun_press_release(true, KEY_RightShift);
	}
	if (IS_PRESSED) keys_pressed++;
}
