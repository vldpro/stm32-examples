#ifndef UART_INT_DRIVER_H
#define UART_INT_DRIVER_H

#include <stdint.h>

typedef enum iuart_io_res {
    IUART_SUCCESS,
    IUART_NO_DATA,
    IUART_BUF_FULL
} iuart_res_t;

void iuart_init(void);

iuart_res_t iuart_transmit(uint8_t val);
iuart_res_t iuart_receive(uint8_t *val);

#endif
