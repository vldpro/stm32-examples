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

static res_buf_t convert_input(uint8_t c)
{
    uint8_t res = c;
    if (is_cyrillic(c))
        res = to_upper(c);
    if (is_latin(c))
        res = to_lower(c);
    return to_triple(res);
}

//
// Work Modes
//

static void mode_latin_cyrillic_converter(void)
{
    uint8_t ch = puart_receive();
    res_buf_t res = convert_input(ch);
    puart_transmit_buf(res.data, res.sz);
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
        mode_decimal_to_binary_conveter();
        /*while (iuart_receive(&ch) == IUART_NO_DATA)
            ;
        while (iuart_transmit(ch) == IUART_BUF_FULL)
            ;*/
    }
}