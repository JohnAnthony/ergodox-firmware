#include <stdint.h>
#include <util/twi.h>
#include "twi.h"

void twi_init(void) {
	TWSR &= ~((1<<TWPS1) | (1<<TWPS0));  // prescaler = 1
	TWBR = ((F_CPU / TWI_FREQ) - 16) / 2;
}

uint8_t twi_start(void) {
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTA);
	while (!(TWCR & (1<<TWINT)));
	if (TW_STATUS != TW_START && TW_STATUS != TW_REP_START)
		return TW_STATUS;
	return 0;
}

void twi_stop(void) {
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	while (TWCR & (1<<TWSTO));
}

uint8_t twi_send(uint8_t data) {
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	if (TW_STATUS != TW_MT_SLA_ACK &&
	    TW_STATUS != TW_MT_DATA_ACK &&
	    TW_STATUS != TW_MR_SLA_ACK)
		return TW_STATUS;
	return 0;
}

uint8_t twi_read(uint8_t *data) {
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	while (!(TWCR & (1<<TWINT)));
	*data = TWDR;
	if (TW_STATUS != TW_MR_DATA_ACK)
		return TW_STATUS;
	return 0;
}
