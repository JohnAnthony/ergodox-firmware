#ifndef KEY_FUNCTIONS_H
#define KEY_FUNCTIONS_H

#include <stdbool.h>
#include <stdint.h>
#include "main.h"

void _kbfun_press_release(bool press, uint8_t keycode);
bool _kbfun_is_pressed(uint8_t keycode);

void kbfun_press_release(void);
void kbfun_press_release_preserve_sticky(void);
void kbfun_transparent(void);
void kbfun_layer_push_1(void);
void kbfun_layer_pop_1(void);
void kbfun_2_keys_capslock_press_release(void);

#endif
