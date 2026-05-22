#ifndef TWI_H
#define TWI_H

#define TWI_FREQ 400000

void    twi_init(void);
uint8_t twi_start(void);
void    twi_stop(void);
uint8_t twi_send(uint8_t data);
uint8_t twi_read(uint8_t *data);

#endif
