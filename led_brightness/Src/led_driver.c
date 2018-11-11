#include "led_driver.h"
#include "main.h"
#include <malloc.h>

// External variables

extern RTC_HandleTypeDef hrtc;
extern TIM_HandleTypeDef htim4;

// Private defines

#define AVAILABLE_LEDS 4

#define LED_CHANNELS_PER_TIM 4

#define PWM_IMPULSE_PERIOD 65535
#define PWM_CCR_FROM_DUTY_CYCLE(percent) (((PWM_IMPULSE_PERIOD + 1) * (percent - 1)) / 100)
#define PWM_DUTY_CYCLE_FROM_CCR(ccr_value) ((ccr_value * 100) / (PWM_IMPULSE_PERIOD + 1) + 1)

#define LED_DEFAULT_CONSTRUCTOR(timer_ptr, chan)                                                                       \
	(led_t)                                                                                                        \
	{                                                                                                              \
		.brightness = 100, .timer = (timer_ptr), .channel = (chan)                                             \
	}

// Public structures implementation

struct led {
	char brightness;
	TIM_HandleTypeDef *timer;
	char channel;
};

struct leds_list {
	struct led *leds;
	int sz;
};

// Private functions

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

static void init_leds(struct leds_list *leds)
{
	leds->sz = AVAILABLE_LEDS;
	leds->leds = malloc(AVAILABLE_LEDS * sizeof(struct led));

	for (size_t chan = 1; chan <= LED_CHANNELS_PER_TIM; chan++) {
		size_t idx = chan - 1;
		leds->leds[idx] = LED_DEFAULT_CONSTRUCTOR(&htim4, chan);
	}
}

static void deinit_leds(struct leds_list *leds)
{
	free(leds->leds);
}

static void timer_set_ccr(TIM_HandleTypeDef *tim, char chan, int ccr_value)
{
	switch (chan) {
	case 1:
		tim->Instance->CCR1 = ccr_value;
		break;
	case 2:
		tim->Instance->CCR2 = ccr_value;
		break;
	case 3:
		tim->Instance->CCR3 = ccr_value;
		break;
	case 4:
		tim->Instance->CCR4 = ccr_value;
		break;
	}
}

static int timer_get_ccr(TIM_HandleTypeDef *tim, char chan)
{
	switch (chan) {
	case 1:
		return tim->Instance->CCR1;
	case 2:
		return tim->Instance->CCR2;
	case 3:
		return tim->Instance->CCR3;
	case 4:
		return tim->Instance->CCR4;
	default:
		return 0;
	}
}

// Public functions implementation

leds_list_t *leds_new(void)
{
	init_hal();
	init_components();
	start_pwm();

	leds_list_t *list = malloc(sizeof(leds_list_t));
	init_leds(list);
	return list;
}

void leds_delete(leds_list_t *leds)
{
	deinit_leds(leds);
	free(leds);
}

unsigned int leds_size(leds_list_t *leds)
{
	return leds->sz;
}

void leds_foreach(leds_list_t *leds, led_handler_fn led_handler)
{
	for (size_t i = 0; i < leds->sz; i++) {
		led_t *led = &(leds->leds[i]);
		led_handler(led);
	}
}

void led_set_brightness(led_t *led, unsigned int percent)
{
	int ccr = PWM_CCR_FROM_DUTY_CYCLE(percent);
	timer_set_ccr(led->timer, led->channel, ccr);
}

unsigned int led_get_brightness(led_t *led)
{
	int ccr = timer_get_ccr(led->timer, led->channel);
	return PWM_DUTY_CYCLE_FROM_CCR(ccr);
}