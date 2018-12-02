#include "hw_init.h"

#include "main.h"
#include "gpio.h"
#include "rtc.h"
#include "usb.h"
#include "usart.h"
#include "tim.h"

//
// Module's private data
//

#define BUF_SZ 1024
#define HW_AVAILABLE_TIMS 2
#define HW_AVAILABLE_BTNS 1

static int8_t rbuf[BUF_SZ];
static int8_t wbuf[BUF_SZ];

static struct {
    btns_initial_t btns_initial;
    leds_initial_t leds_initial;
} drivers_initial_data;

extern void SystemClock_Config();

//
// Private functions
//

static void hw_init_btns(void)
{
    btns_initial_t *btns_initial = &(drivers_initial_data.btns_initial);
    btns_initial->btns_inits =
        malloc(sizeof(btn_initial_t) * HW_AVAILABLE_BTNS);

    btns_initial->btns_inits[0] =
        (btn_initial_t){ .port = GPIOC, .pin = GPIO_PIN_13 };

    btns_initial->sz = HW_AVAILABLE_BTNS;
}

static void hw_init_leds(void)
{
    TIM_HandleTypeDef **tims =
        malloc(HW_AVAILABLE_TIMS * sizeof(TIM_HandleTypeDef *));
    tims[0] = &htim2;
    tims[1] = &htim4;

    drivers_initial_data.leds_initial =
        (leds_initial_t){ .tims = (void *)tims, .sz = HW_AVAILABLE_TIMS };
}

static void hw_init_components(void)
{
    SystemClock_Config();
    MX_GPIO_Init();
    MX_RTC_Init();
    MX_USB_PCD_Init();
    MX_UART4_Init();
    MX_TIM2_Init();
    MX_TIM4_Init();
}

//
// Public functions
//

void hw_disable_uart_interrupts(void)
{
    __HAL_UART_DISABLE_IT(&huart4, UART_IT_RXNE);
    __HAL_UART_DISABLE_IT(&huart4, UART_IT_TXE);
}

void hw_init(void)
{
    hw_init_components();
    hw_init_leds();
    hw_init_btns();
    hw_disable_uart_interrupts();
}

iuart_initial_t hw_iuart_initial_data(void)
{
    return (iuart_initial_t){ .rbuf = rbuf,
                              .wbuf = wbuf,
                              .rbuf_sz = BUF_SZ,
                              .wbuf_sz = BUF_SZ,
                              .huart = &huart4 };
}

btns_initial_t *hw_btns_initial_data(void)
{
    return &(drivers_initial_data.btns_initial);
}

leds_initial_t *hw_leds_initial_data(void)
{
    return &(drivers_initial_data.leds_initial);
}