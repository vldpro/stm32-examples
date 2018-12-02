#ifndef UART_POLL_DRIVER
#define UART_POLL_DRIVER

#include <stdint.h>

typedef enum puart_res { PUART_SUCCESS, PUART_IO_TIMEOUT } puart_res_t;

void puart_init(void);

puart_res_t puart_transmit(uint8_t val);
uint8_t puart_receive(void);

#endif
