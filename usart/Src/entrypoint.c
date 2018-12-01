#include "main.h"
#include "gpio.h"
#include "rtc.h"
#include "usb.h"
#include "usart.h"
#include "uart_int_driver.h"

static const uint8_t g_char = 'a';

extern void SystemClock_Config();

void main(void)
{
    SystemClock_Config();
    MX_GPIO_Init();
    MX_RTC_Init();
    MX_USB_PCD_Init();
    MX_UART4_Init();

    iuart_init();
    iuart_transmit('c');
    iuart_transmit('b');
    iuart_transmit('a');

    uint8_t ch = 'e';

    for (;;) {
        while (iuart_receive(&ch) == -1)
            ;
        iuart_transmit(ch);
        long long unsigned int i = 300;
        for (;;) {
            if (!(i--))
                break;
        }
    }
}