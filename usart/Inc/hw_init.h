#ifndef HW_INIT_H
#define HW_INIT_H

#include "usart.h"

typedef struct iuart_initial {
    uint8_t *rbuf;
    uint8_t *wbuf;
    uint32_t rbuf_sz;
    uint32_t wbuf_sz;
    UART_HandleTypeDef *huart;
} iuart_initial_t;

void hw_init(void);
iuart_initial_t hw_iuart_initial_data(void);

#endif