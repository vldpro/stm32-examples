#ifndef UART_INT_DRIVER_H
#define UART_INT_DRIVER_H

#include <stdint.h>
#include "usart.h"

typedef enum iuart_io_res {
    IUART_SUCCESS,
    IUART_NO_DATA,
    IUART_BUF_FULL
} iuart_res_t;

void iuart_init(UART_HandleTypeDef *uartHandle,
                uint8_t *rbuf,
                uint32_t rbuf_sz,
                uint8_t *wbuf,
                uint32_t wbuf_sz);

iuart_res_t iuart_transmit(uint8_t val);
iuart_res_t iuart_receive(uint8_t *val);

#endif
