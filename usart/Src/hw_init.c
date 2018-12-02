#include "hw_init.h"

#include "main.h"
#include "gpio.h"
#include "rtc.h"
#include "usb.h"
#include "usart.h"

#define BUF_SZ 1024

static int8_t rbuf[BUF_SZ];
static int8_t wbuf[BUF_SZ];

extern void SystemClock_Config();

void hw_init(void)
{
    SystemClock_Config();
    MX_GPIO_Init();
    MX_RTC_Init();
    MX_USB_PCD_Init();
    MX_UART4_Init();

    hw_disable_uart_interrupts();
}

void hw_disable_uart_interrupts(void)
{
    __HAL_UART_DISABLE_IT(&huart4, UART_IT_RXNE);
    __HAL_UART_DISABLE_IT(&huart4, UART_IT_TXE);
}

iuart_initial_t hw_iuart_initial_data(void)
{
    return (iuart_initial_t){ .rbuf = rbuf,
                              .wbuf = wbuf,
                              .rbuf_sz = BUF_SZ,
                              .wbuf_sz = BUF_SZ,
                              .huart = &huart4 };
}