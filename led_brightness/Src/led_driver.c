#include "led_driver.h"
#include "main.h"
#include "stm32f3xx_hal.h"
#include <malloc.h>

#define AVAILABLE_LEDS 4
#define LED_CHANNELS_PER_TIM 4
#define TIM_CHANNELD_(i) TIM_CHANNEL_##i
#define LED_DEFAULT_CONSTRUCTOR(timer_ptr, channel)                                                                    \
	(led_t)                                                                                                        \
	{                                                                                                              \
		.brightness = 100, .timer = (timer_ptr), .channel = TIM_CHANNEL_(channel)                              \
	}

struct led {
	char brightness;
	TIM_HandleTypeDef *timer;
	char channel;
};

struct led_list {
	struct led *leds;
	int sz;
};

static void init_hal(void)
{
	HAL_Init();
	SystemClock_Config();
}

static void init_components(void)
{
	MX_GPIO_Init();
	MX_RTC_Init();
	MX_USART3_UART_Init();
	MX_USB_PCD_Init();
	MX_TIM4_Init();
}

static void start_pwm(void)
{
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
}

static void init_leds(struct led_list *leds)
{
	leds->sz = AVAILABLE_LEDS;
	leds->leds = malloc(AVAILABLE_LEDS * sizeof(struct led));

	for (size_t chan = 1; i <= LED_CHANNELS_PER_TIM; chan++) {
		size_t idx = chan - 1;
		leds->leds[idx] = LED_DEFAULT_CONSTRUCTOR(htim4, chan);
	}
}

static void void leds_init(led_list_t *leds)
{
	init_hal();
	init_components();
	init_leds(leds);
	start_pwm();
}