#include "utils.h"
#include "led_driver.h"
#include "stm32f3xx_hal.h"

static void
set_one_led_br(leds_list_t *leds, unsigned char led_num, unsigned int val)
{
    led_t *led = leds_at(leds, led_num);
    led_set_brightness(led, val);
}

static void set_all_led_br(leds_list_t *leds, unsigned int val)
{
    unsigned int size = leds_size(leds);
    for (unsigned int i = 0; i < size; i++)
        set_one_led_br(leds, i, val);
}

void test_leds_formation(leds_list_t *leds)
{
    unsigned int size = leds_size(leds);
    set_all_led_br(leds, LED_MIN_BRIGHTNESS);

    for (unsigned int i = 0; i < size; i++) {
        set_one_led_br(leds, i, LED_MAX_BRIGHTNESS);
        delay(1000);
        set_one_led_br(leds, i, LED_MIN_BRIGHTNESS);
    }
}

void delay(int i)
{
    return HAL_Delay(i);
}
