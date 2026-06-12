#include <stdint.h>
#include <util/twi.h>
#include "twi.h"

#define TWI_TIMEOUT 10000

void twi_init(void) {
	TWSR &= ~((1<<TWPS1) | (1<<TWPS0));  // prescaler = 1
	TWBR = ((F_CPU / TWI_FREQ) - 16) / 2;
}

// Force STOP, disable and re-enable TWI to escape a stuck bus.
static void twi_reset(void) {
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	TWCR = 0;
	TWCR = (1<<TWEN);
}

uint8_t twi_start(void) {
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTA);
	uint16_t t = TWI_TIMEOUT;
	while (!(TWCR & (1<<TWINT)) && --t);
	if (!t) { twi_reset(); return 0xFF; }
	if (TW_STATUS != TW_START && TW_STATUS != TW_REP_START)
		return TW_STATUS;
	return 0;
}

void twi_stop(void) {
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	uint16_t t = TWI_TIMEOUT;
	while ((TWCR & (1<<TWSTO)) && --t);
	if (!t) twi_reset();
}

uint8_t twi_send(uint8_t data) {
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);
	uint16_t t = TWI_TIMEOUT;
	while (!(TWCR & (1<<TWINT)) && --t);
	if (!t) { twi_reset(); return 0xFF; }
	if (TW_STATUS != TW_MT_SLA_ACK &&
	    TW_STATUS != TW_MT_DATA_ACK &&
	    TW_STATUS != TW_MR_SLA_ACK)
		return TW_STATUS;
	return 0;
}

uint8_t twi_read(uint8_t *data) {
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	uint16_t t = TWI_TIMEOUT;
	while (!(TWCR & (1<<TWINT)) && --t);
	if (!t) { twi_reset(); return 0xFF; }
	*data = TWDR;
	if (TW_STATUS != TW_MR_DATA_ACK)
		return TW_STATUS;
	return 0;
}
