#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>
#include <stdint.h>
#include "layout.h"

extern bool (*main_kb_is_pressed)[KB_ROWS][KB_COLUMNS];
extern bool (*main_kb_was_pressed)[KB_ROWS][KB_COLUMNS];
extern uint8_t main_layers_pressed[KB_ROWS][KB_COLUMNS];

extern uint8_t main_loop_row;
extern uint8_t main_loop_col;

extern uint8_t main_arg_layer;
extern uint8_t main_arg_layer_offset;
extern uint8_t main_arg_row;
extern uint8_t main_arg_col;
extern bool    main_arg_is_pressed;
extern bool    main_arg_was_pressed;
extern bool    main_arg_trans_key_pressed;

void    main_exec_key(void);
uint8_t main_layers_peek(uint8_t offset);
uint8_t main_layers_push(uint8_t layer);
void    main_layers_pop_id(uint8_t id);

#endif
