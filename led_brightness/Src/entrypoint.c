#include "led_driver.h"
#include "stm32f3xx_hal.h"
#include "utils.h"

#define LED_BRIGHTNESS_DELTA 19
#define TICKS_TRESHOLD 50

#define VIEW_BRIGHTNESS_ANIMATION 0
#define VIEW_BTN_PRESSES_COUNT 1

static struct {
	leds_list_t *leds;
	unsigned char btn_press_cnt;
	char current_view;
} module_scope = { .leds = NULL, .btn_press_cnt = 0, .current_view = VIEW_BRIGHTNESS_ANIMATION };

static void set_initial_brightness_gradient(leds_list_t *leds)
{
	led_set_brightness(leds_at(leds, 0), LED_MIN_BRIGHTNESS);
	led_set_brightness(leds_at(leds, 1), LED_MIN_BRIGHTNESS);
	led_set_brightness(leds_at(leds, 2), LED_MIN_BRIGHTNESS);
	led_set_brightness(leds_at(leds, 3), 10);
	led_set_brightness(leds_at(leds, 4), 30);
	led_set_brightness(leds_at(leds, 5), 50);
	led_set_brightness(leds_at(leds, 6), 70);
	led_set_brightness(leds_at(leds, 7), 100);
}

static void brightness_shift(leds_list_t *leds)
{
	unsigned int size = leds_size(leds);

	led_t *last_ld = leds_at(leds, size - 1);
	unsigned int prev_br = led_get_brightness(last_ld);

	for (unsigned int i = 0; i < size; i++) {
		led_t *cur_led = leds_at(leds, i);

		unsigned int tmp = led_get_brightness(cur_led);
		led_set_brightness(cur_led, prev_br);
		prev_br = tmp;
	}
}

// Callback for external interrupt. Triggers view switch
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_13)
		module_scope.current_view = module_scope.current_view == VIEW_BTN_PRESSES_COUNT ?
						    VIEW_BRIGHTNESS_ANIMATION :
						    VIEW_BTN_PRESSES_COUNT;
}

// Callback for systick. Counts button's presses
void HAL_SYSTICK_Callback(void)
{
	static unsigned int ticks = 0;

	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == GPIO_PIN_SET) {
		ticks++;
	} else if (ticks > TICKS_TRESHOLD) {
		module_scope.btn_press_cnt++;
		ticks = 0;
	} else {
		ticks = 0;
	}
}

static void soft_delay(unsigned long i)
{
	for (; i--;) {
	}
}

static void btn_presses_cnt_view(leds_list_t *leds)
{
	for (;;) {
		leds_display_number(module_scope.leds, module_scope.btn_press_cnt);
		soft_delay(100000);
		if (module_scope.current_view != VIEW_BTN_PRESSES_COUNT)
			return;
	}
}

static void brightness_animation_view(leds_list_t *leds)
{
	set_initial_brightness_gradient(leds);
	for (;;) {
		brightness_shift(leds);
		delay(100);
		//soft_delay(100000000);
		if (module_scope.current_view != VIEW_BRIGHTNESS_ANIMATION)
			return;
	}
}

void main(void)
{
	module_scope.leds = leds_new();

	for (;;) {
		if (module_scope.current_view == VIEW_BRIGHTNESS_ANIMATION) {
			brightness_animation_view(module_scope.leds);
		} else {
			btn_presses_cnt_view(module_scope.leds);
		}
	}
}