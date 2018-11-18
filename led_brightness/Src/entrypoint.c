#include "led_driver.h"
#include "btns_driver.h"
#include "stm32f3xx_hal.h"
#include "utils.h"

#define LED_BRIGHTNESS_DELTA 19
#define TICKS_TRESHOLD 50

#define VIEW_BRIGHTNESS_ANIMATION 0
#define VIEW_BTN_PRESSES_COUNT 1

#define BTN_COUNTER_ID 0
#define BTN_SWITCH_ID 1

typedef void (*btn_press_fn)(void);

static struct {
    leds_list_t *leds;
    unsigned char btn_press_cnt;
    char current_view;
} module_scope = { .leds = NULL,
                   .btn_press_cnt = 0,
                   .current_view = VIEW_BRIGHTNESS_ANIMATION };

static void set_initial_brightness_gradient(leds_list_t *leds)
{
    led_set_brightness(leds_at(leds, 0), 1);
    led_set_brightness(leds_at(leds, 1), 2);
    led_set_brightness(leds_at(leds, 2), 10);
    led_set_brightness(leds_at(leds, 3), 25);
    led_set_brightness(leds_at(leds, 4), 50);
    led_set_brightness(leds_at(leds, 5), 70);
    led_set_brightness(leds_at(leds, 6), 80);
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

void on_switch_btn_pressed(void)
{
    module_scope.current_view =
        module_scope.current_view == VIEW_BTN_PRESSES_COUNT ?
            VIEW_BRIGHTNESS_ANIMATION :
            VIEW_BTN_PRESSES_COUNT;
}

void on_counter_btn_pressed(void)
{
    module_scope.btn_press_cnt++;
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
        delay(120);
        if (module_scope.current_view != VIEW_BRIGHTNESS_ANIMATION)
            return;
    }
}


static void btns_init_and_set_callbacks(void) 
{
    btns_init();
    btn_t* counter_btn = btns_at(BTN_COUNTER_ID);
    btn_t* switch_btn = btns_at(BTN_SWITCH_ID);
    btn_register_press_listener(counter_btn, on_counter_btn_pressed);
    btn_register_press_listener(switch_btn, on_switch_btn_pressed);
}

void main(void)
{
    leds_driver_init();
    btns_init_and_set_callbacks();
    module_scope.leds = leds_new();

    for (;;) {
        if (module_scope.current_view == VIEW_BRIGHTNESS_ANIMATION)
            brightness_animation_view(module_scope.leds);
        else
            btn_presses_cnt_view(module_scope.leds);
    }
}