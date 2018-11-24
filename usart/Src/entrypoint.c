#include "main.h"
#include "gpio.h"
#include "rtc.h"
#include "usb.h"
#include "usart.h"

extern void SystemClock_Config();

void main(void)
{
    SystemClock_Config();
    MX_GPIO_Init();
    MX_RTC_Init();
    MX_USB_PCD_Init();
    MX_USART3_UART_Init();

    uint8_t buffer = 'a';
    for (;;) {
        HAL_UART_Transmit(&huart3, &buffer, sizeof(buffer), HAL_MAX_DELAY);
    }
}