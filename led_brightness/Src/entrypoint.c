#include "led_driver.h"
#include "utils.h"

#define LED_BRIGHTNESS_DELTA 19

static void set_initial_brightness_gradient(leds_list_t *leds)
{
	unsigned int size = leds_size(leds);
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
		delay(100);
	}
}

void main(void)
{
	leds_list_t *leds = leds_new();
	set_initial_brightness_gradient(leds);

	for (;;) {
		brightness_shift(leds);
	}
}