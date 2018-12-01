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
    iuart_transmit(g_char);

    for (;;) {
        //HAL_UART_Transmit(&huart4, &buffer, sizeof(buffer), HAL_MAX_DELAY);
        HAL_Delay(100);
    }
}