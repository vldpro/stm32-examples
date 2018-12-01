#ifndef UART_INT_DRIVER_H
#define UART_INT_DRIVER_H

#include <stdint.h>

void iuart_init(void);

void iuart_transmit(uint8_t val);
int8_t iuart_receive(uint8_t *val);

#endif
