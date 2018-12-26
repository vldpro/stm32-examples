#include "hw_init.h"
#include "uart_int_driver.h"
#include "uart_poll_driver.h"

#include "led_driver.h"
#include "btns_driver.h"

#include "convertors.h"
#include "utils.h"

#define ERR_MSG_INVALID_NUM "Invalid number: Number should be between 0 and 255"
#define ERR_MSG_NAN "NaN"
#define IN_BUFFER_SIZE 1024

//
// Module's data
//

//
// Helpers
//

static void initialize_iuart(void)
{
    iuart_initial_t init = hw_iuart_initial_data();
    iuart_init(init.huart, init.rbuf, init.rbuf_sz, init.wbuf, init.wbuf_sz);
}

union my_char {
    uint8_t c[2];
    uint16_t utf8_ch;
};

static void convert_input(void)
{
    uint8_t c = puart_receive();

    if (c <= 127 && is_latin(c)) {
        c = to_lower(c);
        puart_transmit(c);
        puart_transmit(c);
        puart_transmit(c);
    } else {
        uint8_t c2 = puart_receive();
        union my_char ch;
        ch.c[0] = c2;
        ch.c[1] = c;
        /*if ((ch.c[0] == 0xD0 && ch.c[1] >= 0xB0) ||
            (ch.c[0] == 0xD1 && ch.c[1] >= 0x80))
            ch.c[1] -= 32;*/

        ch.utf8_ch = cyrrillic_to_upper(ch.utf8_ch);
        puart_transmit(ch.c[0]);
        puart_transmit(ch.c[1]);

        puart_transmit(ch.c[0]);
        puart_transmit(ch.c[1]);

        puart_transmit(ch.c[0]);
        puart_transmit(ch.c[1]);
        //uint16_t res = ((uint16_t)c) << 8 + c2;

        /*
        puart_transmit((uint8_t)(res >> 8));
        puart_transmit((uint8_t)(res & 0x00FF));

        puart_transmit((uint8_t)(res >> 8));
        puart_transmit((uint8_t)(res & 0x00FF));

        puart_transmit((uint8_t)(res >> 8));
        puart_transmit((uint8_t)(res & 0x00FF));*/
    }
}

//
// Work Modes
//

static void mode_latin_cyrillic_converter(void)
{
    convert_input();
    //puart_transmit_buf(res.data, res.sz);
}

static void mode_decimal_to_binary_conveter(void)
{
    static uint8_t in_buffer[IN_BUFFER_SIZE];

    uint32_t received = puart_receive_line(in_buffer, IN_BUFFER_SIZE);
    int64_t num = to_num(in_buffer, received);

    if (num == -1) {
        puart_transmit_line(ERR_MSG_NAN, sizeof(ERR_MSG_NAN) - 1);
        return;
    }

    if (num > 255) {
        puart_transmit_line(ERR_MSG_INVALID_NUM,
                            sizeof(ERR_MSG_INVALID_NUM) - 1);
        return;
    }

    res_buf_t bin = to_binary(num);
    leds_display_number(leds_get(), num);
    puart_transmit_line(bin.data, bin.sz);
}

static void init_drivers(void)
{
    leds_driver_init(hw_leds_initial_data());
    btns_init(hw_btns_initial_data());
    puart_init();
}

void main(void)
{
    hw_init();
    init_drivers();
    //initialize_iuart();

    uint8_t ch = 'e';

    for (;;) {
        mode_latin_cyrillic_converter();
        //mode_decimal_to_binary_conveter();
        /*while (iuart_receive(&ch) == IUART_NO_DATA)
            ;
        while (iuart_transmit(ch) == IUART_BUF_FULL)
            ;*/
    }
}