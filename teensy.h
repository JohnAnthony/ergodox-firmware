#ifndef TEENSY_H
#define TEENSY_H

#include <stdbool.h>
#include <stdint.h>
#include "layout.h"

uint8_t teensy_init(void);
uint8_t teensy_update_matrix(bool matrix[KB_ROWS][KB_COLUMNS]);

#endif
