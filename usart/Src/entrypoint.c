#include "hw_init.h"
#include "uart_int_driver.h"

static const uint8_t g_char = 'a';

static void initialize_iuart(void)
{
    iuart_initial_t init = hw_iuart_initial_data();
    iuart_init(init.huart, init.rbuf, init.rbuf_sz, init.wbuf, init.wbuf_sz);
}

void main(void)
{
    hw_init();
    initialize_iuart();

    uint8_t ch = 'e';

    for (;;) {
        while (iuart_receive(&ch) == IUART_NO_DATA)
            ;
        while (iuart_transmit(ch) == IUART_BUF_FULL)
            ;
    }
}