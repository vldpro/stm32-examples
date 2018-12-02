#ifndef UTILS_H
#define UTILS_H

#include "uart_poll_driver.h"

inline void puart_transmit_buf(uint8_t const *buf, uint32_t sz)
{
    uint32_t i;
    for (i = 0; i < sz; i++)
        puart_transmit(buf[i]);
}

inline void puart_transmit_line(uint8_t const *buf, uint32_t sz)
{
    puart_transmit_buf(buf, sz);
    puart_transmit('\r');
}

inline uint32_t puart_receive_line(uint8_t *buf, uint32_t buf_sz)
{
    uint8_t received = 0;
    for (;;) {
        uint8_t ch = puart_receive();
        puart_transmit(ch);
        if (ch == '\r')
            return received;
        buf[received++] = ch;

        if (received == buf_sz)
            return received;
    }
}

#endif