// MCP23018 I/O expander init and left-half matrix scan (cols 0..6, rows 0..5).
// Columns are driven low one at a time via GPIOA; rows are read from GPIOB.

#include <stdbool.h>
#include <stdint.h>
#include <util/twi.h>
#include "layout.h"
#include "mcp23018.h"
#include "twi.h"

#if KB_ROWS != 6 || KB_COLUMNS != 14
	#error "Expecting 6x14 matrix"
#endif

// MCP23018 register addresses
#define IODIRA 0x00
#define IODIRB 0x01
#define GPPUA  0x0C
#define GPPUB  0x0D
#define GPIOA  0x12
#define GPIOB  0x13
#define OLATA  0x14
#define OLATB  0x15

#define TWI_ADDR_WRITE ((MCP23018_TWI_ADDRESS<<1) | TW_WRITE)
#define TWI_ADDR_READ  ((MCP23018_TWI_ADDRESS<<1) | TW_READ)

// Driving columns (GPIOA), reading rows (GPIOB).
// IODIRA: col pins as output (0), unused pin as input (1) → 0b10000000
// IODIRB: row pins as input (1)                           → 0b11111111
uint8_t mcp23018_init(void) {
	uint8_t ret;

	twi_start();
	ret = twi_send(TWI_ADDR_WRITE);
	if (ret) goto out;
	twi_send(IODIRA);
	twi_send(0b10000000);  // IODIRA: cols 0..6 as output, bit7 unused input
	twi_send(0b11111111);  // IODIRB: rows 0..5 as input
	twi_stop();

	twi_start();
	ret = twi_send(TWI_ADDR_WRITE);
	if (ret) goto out;
	twi_send(GPPUA);
	twi_send(0b10000000);  // GPPUA: pull-up on unused input only
	twi_send(0b11111111);  // GPPUB: pull-ups on all row inputs
	twi_stop();

	twi_start();
	ret = twi_send(TWI_ADDR_WRITE);
	if (ret) goto out;
	twi_send(OLATA);
	twi_send(0b11111111);  // OLATA: all cols hi-Z initially
	twi_send(0b11111111);  // OLATB

out:
	twi_stop();
	return ret;
}

uint8_t mcp23018_update_matrix(bool matrix[KB_ROWS][KB_COLUMNS]) {
	uint8_t ret, data;

	ret = mcp23018_init();
	if (ret) {
		for (uint8_t row = 0; row <= 5; row++)
			for (uint8_t col = 0; col <= 6; col++)
				matrix[row][col] = 0;
		return ret;
	}

	for (uint8_t col = 0; col <= 6; col++) {
		// Drive active column low, all others hi-Z
		twi_start();
		twi_send(TWI_ADDR_WRITE);
		twi_send(GPIOA);
		twi_send(0xFF & ~(1<<col));
		twi_stop();

		// Read row data
		twi_start();
		twi_send(TWI_ADDR_WRITE);
		twi_send(GPIOB);
		twi_start();
		twi_send(TWI_ADDR_READ);
		twi_read(&data);
		twi_stop();

		for (uint8_t row = 0; row <= 5; row++)
			matrix[row][col] = !(data & (1<<(5-row)));
	}

	// Return all columns hi-Z
	twi_start();
	twi_send(TWI_ADDR_WRITE);
	twi_send(GPIOA);
	twi_send(0xFF);
	twi_stop();

	return 0;
}
