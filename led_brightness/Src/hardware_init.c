#include "hardware_init.h"
#include "stm32f3xx_hal.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "usb.h"
#include "gpio.h"
#include "utils.h"
#include <malloc.h>

#define HW_AVAILABLE_BTNS 2
#define HW_AVAILABLE_TIMS 2

void SystemClock_Config(void);

// Module's private data

static struct {
    btns_initial_t btns_initial;
    leds_initial_t leds_initial;
} drivers_initial_data;

// Module's private functions

static void init_hal(void)
{
    HAL_Init();
}

static void init_components(void)
{
    SystemClock_Config();
    MX_GPIO_Init();
    MX_RTC_Init();
    MX_USART3_UART_Init();
    MX_USB_PCD_Init();
    MX_TIM4_Init();
    MX_TIM2_Init();
}

static void init_btns(void)
{
    btns_initial_t *btns_initial = &(drivers_initial_data.btns_initial);
    btns_initial->btns_inits =
        malloc(sizeof(btn_initial_t) * HW_AVAILABLE_BTNS);

    btns_initial->btns_inits[0] =
        (btn_initial_t){ .port = GPIOA, .pin = GPIO_PIN_10 };
    btns_initial->btns_inits[1] =
        (btn_initial_t){ .port = GPIOC, .pin = GPIO_PIN_13 };

    btns_initial->sz = HW_AVAILABLE_BTNS;
}

static void init_leds(void)
{
    TIM_HandleTypeDef **tims =
        malloc(HW_AVAILABLE_TIMS * sizeof(TIM_HandleTypeDef *));
    tims[0] = &htim4;
    tims[1] = &htim2;

    drivers_initial_data.leds_initial =
        (leds_initial_t){ .tims = (void *)tims, .sz = HW_AVAILABLE_TIMS };
}

// Module's public functions

void hw_init(void)
{
    init_hal();
    init_components();
    init_btns();
    init_leds();
}

btns_initial_t *hw_btns_initial_data(void)
{
    return &(drivers_initial_data.btns_initial);
}

leds_initial_t *hw_leds_initial_data(void)
{
    return &(drivers_initial_data.leds_initial);
}