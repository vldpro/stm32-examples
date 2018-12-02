#ifndef HW_INIT_H
#define HW_INIT_H

#include "usart.h"
#include "initial_data.h"

typedef struct iuart_initial {
    uint8_t *rbuf;
    uint8_t *wbuf;
    uint32_t rbuf_sz;
    uint32_t wbuf_sz;
    UART_HandleTypeDef *huart;
} iuart_initial_t;

void hw_init(void);
void hw_disale_uart_interrupts(void);
iuart_initial_t hw_iuart_initial_data(void);
btns_initial_t *hw_btns_initial_data(void);
leds_initial_t *hw_leds_initial_data(void);

#endif