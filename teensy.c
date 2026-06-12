// Teensy 2.0 hardware init and right-half matrix scan (cols 7..D, rows 0..5).
// Columns are driven low one at a time; rows are read as inputs with pull-ups.

#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "layout.h"
#include "teensy.h"
#include "twi.h"

#if F_CPU != 16000000
	#error "Expecting F_CPU = 16000000"
#endif
#if KB_ROWS != 6 || KB_COLUMNS != 14
	#error "Expecting 6x14 matrix"
#endif

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
#define CPU_16MHz 0x00

// Unused pins
#define UNUSED_0  C, 7
#define UNUSED_1  D, 7
#define UNUSED_2  D, 4
#define UNUSED_3  D, 5
#define UNUSED_4  E, 6

// Row pins (inputs with pull-ups)
#define ROW_0  F, 7
#define ROW_1  F, 6
#define ROW_2  F, 5
#define ROW_3  F, 4
#define ROW_4  F, 1
#define ROW_5  F, 0

// Column pins (driven low to scan, otherwise hi-Z)
#define COLUMN_7  B, 0
#define COLUMN_8  B, 1
#define COLUMN_9  B, 2
#define COLUMN_A  B, 3
#define COLUMN_B  D, 2
#define COLUMN_C  D, 3
#define COLUMN_D  C, 6

#define SET   |=
#define CLEAR &=~

#define _pin_write(reg, op, letter, number) \
	do { ((reg##letter) op (1<<(number))); _delay_us(1); } while(0)
#define pin_write(reg, op, pin)  _pin_write(reg, op, pin)
#define pin_read(letter, number) ((PIN##letter) & (1<<(number)))
#define pin_read2(pin)           pin_read(pin)

#define write_all_unused(reg, op) do { \
	pin_write(reg, op, UNUSED_0); \
	pin_write(reg, op, UNUSED_1); \
	pin_write(reg, op, UNUSED_2); \
	pin_write(reg, op, UNUSED_3); \
	pin_write(reg, op, UNUSED_4); } while(0)

#define write_all_rows(reg, op) do { \
	pin_write(reg, op, ROW_0); \
	pin_write(reg, op, ROW_1); \
	pin_write(reg, op, ROW_2); \
	pin_write(reg, op, ROW_3); \
	pin_write(reg, op, ROW_4); \
	pin_write(reg, op, ROW_5); } while(0)

#define write_all_cols(reg, op) do { \
	pin_write(reg, op, COLUMN_7); \
	pin_write(reg, op, COLUMN_8); \
	pin_write(reg, op, COLUMN_9); \
	pin_write(reg, op, COLUMN_A); \
	pin_write(reg, op, COLUMN_B); \
	pin_write(reg, op, COLUMN_C); \
	pin_write(reg, op, COLUMN_D); } while(0)

// Drive column low, read all rows, return column to hi-Z.
#define scan_col(matrix, col) do { \
	pin_write(DDR, SET, COLUMN_##col); \
	matrix[0][0x##col] = !pin_read2(ROW_0); \
	matrix[1][0x##col] = !pin_read2(ROW_1); \
	matrix[2][0x##col] = !pin_read2(ROW_2); \
	matrix[3][0x##col] = !pin_read2(ROW_3); \
	matrix[4][0x##col] = !pin_read2(ROW_4); \
	matrix[5][0x##col] = !pin_read2(ROW_5); \
	pin_write(DDR, CLEAR, COLUMN_##col); } while(0)

uint8_t teensy_init(void) {
	CPU_PRESCALE(CPU_16MHz);

	// D6 is the Teensy 2.0 onboard LED, used as layer indicator — set as output, start off
	DDRD  |=  (1<<6); PORTD &= ~(1<<6);
	// B4 is tied to Vcc on the ErgoDox PCB — keep as input to avoid short
	DDRB  &= ~(1<<4); PORTB &= ~(1<<4);

	// Keyboard LEDs: fast PWM on OC1A/B/C
	_kb_led_all_off();
	TCCR1A = 0b10101001;
	TCCR1B = 0b00001001;

	twi_init();  // I2C on D1/D0

	write_all_unused(DDR, CLEAR);
	write_all_unused(PORT, SET);

	write_all_rows(DDR,  CLEAR);
	write_all_cols(DDR,  CLEAR);
	write_all_rows(PORT, SET);    // pull-ups on rows
	write_all_cols(PORT, CLEAR);  // no pull-ups on cols (we drive them)

	return 0;
}

uint8_t teensy_update_matrix(bool matrix[KB_ROWS][KB_COLUMNS]) {
	scan_col(matrix, 7);
	scan_col(matrix, 8);
	scan_col(matrix, 9);
	scan_col(matrix, A);
	scan_col(matrix, B);
	scan_col(matrix, C);
	scan_col(matrix, D);
	return 0;
}
