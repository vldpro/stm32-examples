#include "led_driver.h"

#define LED_BRIGHTNESS_50 50

void set_half_brightness(led_t *led)
{
	led_set_brightness(led, LED_BRIGHTNESS_50);
}

void main(void)
{
	leds_list_t *leds = leds_new();
	leds_foreach(leds, set_half_brightness);

	for (;;) {
	}
}