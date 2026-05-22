#ifndef LAYOUT_H
#define LAYOUT_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "keycodes.h"

#define KB_ROWS    6
#define KB_COLUMNS 14
#define KB_LAYERS  2

typedef void (*void_funptr_t)(void);

// Hardware LED macros — Teensy 2.0 PWM on OC1A/B/C (pins B5/B6/B7)
// LEDs are active when pin is set as output (DDR=1); brightness via OCR1x.
#define _kb_led_1_on()           (DDRB |=  (1<<5))
#define _kb_led_1_off()          (DDRB &= ~(1<<5))
#define _kb_led_1_set_percent(n) (OCR1A = (uint8_t)((n) * 0xFF))

#define _kb_led_2_on()           (DDRB |=  (1<<6))
#define _kb_led_2_off()          (DDRB &= ~(1<<6))
#define _kb_led_2_set_percent(n) (OCR1B = (uint8_t)((n) * 0xFF))

#define _kb_led_3_on()           (DDRB |=  (1<<7))
#define _kb_led_3_off()          (DDRB &= ~(1<<7))
#define _kb_led_3_set_percent(n) (OCR1C = (uint8_t)((n) * 0xFF))

#define _kb_led_all_off() do { _kb_led_1_off(); _kb_led_2_off(); _kb_led_3_off(); } while(0)
#define _kb_led_all_on()  do { _kb_led_1_on();  _kb_led_2_on();  _kb_led_3_on();  } while(0)
#define _kb_led_all_set_percent(n) do { \
    _kb_led_1_set_percent(n); \
    _kb_led_2_set_percent(n); \
    _kb_led_3_set_percent(n); } while(0)

// Logical LED names: LED1=num lock, LED2=caps lock, LED3=scroll lock
#define kb_led_num_on()     _kb_led_1_on()
#define kb_led_num_off()    _kb_led_1_off()
#define kb_led_caps_on()    _kb_led_2_on()
#define kb_led_caps_off()   _kb_led_2_off()
#define kb_led_scroll_on()  _kb_led_3_on()
#define kb_led_scroll_off() _kb_led_3_off()
#define kb_led_compose_on()
#define kb_led_compose_off()
#define kb_led_kana_on()
#define kb_led_kana_off()

// LED state sequences called from main() during startup
#define kb_led_state_power_on() do { \
    _kb_led_all_set_percent(MAKEFILE_LED_BRIGHTNESS / 10); \
    _kb_led_all_on(); } while(0)

#define kb_led_delay_usb_init() do { \
    _kb_led_1_set_percent(MAKEFILE_LED_BRIGHTNESS); _delay_ms(333); \
    _kb_led_2_set_percent(MAKEFILE_LED_BRIGHTNESS); _delay_ms(333); \
    _kb_led_3_set_percent(MAKEFILE_LED_BRIGHTNESS); _delay_ms(333); } while(0)

#define kb_led_state_ready() do { \
    _kb_led_all_off(); \
    _kb_led_all_set_percent(MAKEFILE_LED_BRIGHTNESS); } while(0)

// Maps spatial key positions to the 6x14 matrix.
// Left hand:  rows 0..5, cols 0..6  (driven by MCP23018)
// Right hand: rows 0..5, cols 7..D  (driven by Teensy 2.0)
#define KB_MATRIX_LAYER( \
        na, \
        k50,k51,k52,k53,k54,k55,k56, \
        k40,k41,k42,k43,k44,k45,k46, \
        k30,k31,k32,k33,k34,k35, \
        k20,k21,k22,k23,k24,k25,k26, \
        k10,k11,k12,k13,k14, \
                                k05,k06, \
                            k15,k16,k04, \
                            k03,k02,k01, \
        k57,k58,k59,k5A,k5B,k5C,k5D, \
        k47,k48,k49,k4A,k4B,k4C,k4D, \
            k38,k39,k3A,k3B,k3C,k3D, \
        k27,k28,k29,k2A,k2B,k2C,k2D, \
                k19,k1A,k1B,k1C,k1D, \
        k07,k08, \
        k09,k17,k18, \
        k0C,k0B,k0A ) \
{{  na,k01,k02,k03,k04,k05,k06, k07,k08,k09,k0A,k0B,k0C, na }, \
 { k10,k11,k12,k13,k14,k15,k16, k17,k18,k19,k1A,k1B,k1C,k1D }, \
 { k20,k21,k22,k23,k24,k25,k26, k27,k28,k29,k2A,k2B,k2C,k2D }, \
 { k30,k31,k32,k33,k34,k35, na,  na,k38,k39,k3A,k3B,k3C,k3D }, \
 { k40,k41,k42,k43,k44,k45,k46, k47,k48,k49,k4A,k4B,k4C,k4D }, \
 { k50,k51,k52,k53,k54,k55,k56, k57,k58,k59,k5A,k5B,k5C,k5D }}

// PROGMEM layout array declarations
extern const uint8_t       PROGMEM _kb_layout[KB_LAYERS][KB_ROWS][KB_COLUMNS];
extern const void_funptr_t PROGMEM _kb_layout_press[KB_LAYERS][KB_ROWS][KB_COLUMNS];
extern const void_funptr_t PROGMEM _kb_layout_release[KB_LAYERS][KB_ROWS][KB_COLUMNS];

#define kb_layout_get(layer, row, col) \
    ((uint8_t)pgm_read_byte(&(_kb_layout[layer][row][col])))
#define kb_layout_press_get(layer, row, col) \
    ((void_funptr_t)pgm_read_word(&(_kb_layout_press[layer][row][col])))
#define kb_layout_release_get(layer, row, col) \
    ((void_funptr_t)pgm_read_word(&(_kb_layout_release[layer][row][col])))

#endif
