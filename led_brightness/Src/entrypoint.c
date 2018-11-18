#include "led_driver.h"
#include "btns_driver.h"
#include "utils.h"
#include <malloc.h>

#define LED_BRIGHTNESS_DELTA 13

#define VIEW_BRIGHTNESS_ANIMATION 0
#define VIEW_BTN_PRESSES_COUNT 1

#define BTN_COUNTER_ID 0
#define BTN_SWITCH_ID 1
#define VIEW_BTN_PRESSES_RENDER_TIMEOUT 100000
#define VIEW_BRIGHTNESS_ANIMATION_RENDER_TIMEOUT 120

static struct {
    leds_list_t *leds;
    unsigned char btn_press_cnt;
    char current_view;
} module_scope = { .leds = NULL,
                   .btn_press_cnt = 0,
                   .current_view = VIEW_BRIGHTNESS_ANIMATION };

static void brightness_shift(leds_list_t *leds)
{
    unsigned int size = leds_size(leds);

    led_t *last_ld = leds_at(leds, size - 1);
    unsigned int prev_br = led_get_brightness(last_ld);
    unsigned int tmp = 0;

    for (unsigned int i = 0; i < size; i++) {
        led_t *cur_led = leds_at(leds, i);
        tmp = led_get_brightness(cur_led);
        led_set_brightness(cur_led, prev_br);
        prev_br = tmp;
    }
}

static void show_btn_presses_count(leds_list_t *leds)
{
    for (;;) {
        leds_display_number(module_scope.leds, module_scope.btn_press_cnt);
        soft_delay(VIEW_BTN_PRESSES_RENDER_TIMEOUT);
        if (module_scope.current_view != VIEW_BTN_PRESSES_COUNT)
            return;
    }
}

static void initialize_led(led_t *led)
{
    static unsigned int br = LED_MIN_BRIGHTNESS;
    led_set_brightness(led, br);
    br = br + LED_BRIGHTNESS_DELTA > LED_MAX_BRIGHTNESS ?
             LED_MIN_BRIGHTNESS :
             br + LED_BRIGHTNESS_DELTA;
}

static void show_led_brightness_animation(leds_list_t *leds)
{
    leds_foreach(leds, initialize_led);
    for (;;) {
        brightness_shift(leds);
        delay(VIEW_BRIGHTNESS_ANIMATION_RENDER_TIMEOUT);
        if (module_scope.current_view != VIEW_BRIGHTNESS_ANIMATION)
            return;
    }
}

static void on_switch_btn_pressed(void)
{
    module_scope.current_view =
        module_scope.current_view == VIEW_BTN_PRESSES_COUNT ?
            VIEW_BRIGHTNESS_ANIMATION :
            VIEW_BTN_PRESSES_COUNT;
}

static void on_counter_btn_pressed(void)
{
    module_scope.btn_press_cnt++;
}

static void leds_init_and_set(void)
{
    leds_driver_init();
    module_scope.leds = leds_new();
}

static void btns_init_and_set_callbacks(void)
{
    btns_init();
    btn_t *counter_btn = btns_at(BTN_COUNTER_ID);
    btn_t *switch_btn = btns_at(BTN_SWITCH_ID);
    btn_register_press_listener(counter_btn, on_counter_btn_pressed);
    btn_register_press_listener(switch_btn, on_switch_btn_pressed);
}

void main(void)
{
    leds_init_and_set();
    btns_init_and_set_callbacks();

    for (;;)
        if (module_scope.current_view == VIEW_BRIGHTNESS_ANIMATION)
            show_led_brightness_animation(module_scope.leds);
        else
            show_btn_presses_count(module_scope.leds);
}