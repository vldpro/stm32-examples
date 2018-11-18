#include "led_driver.h"
#include "initial_data.h"
#include "stm32f3xx_hal.h"
#include <malloc.h>

// External variables

extern void SystemClock_Config(void);

// Private defines

#define AVAILABLE_LEDS 8

#define LED_CHANNELS_PER_TIM 4
#define LED_DEFAULT_BRIGHTNESS 100

#define PWM_IMPULSE_PERIOD 65535
#define PWM_CCR_FROM_DUTY_CYCLE(percent)                                       \
    (((PWM_IMPULSE_PERIOD + 1) * (percent - 1)) / 100)
#define PWM_DUTY_CYCLE_FROM_CCR(ccr_value)                                     \
    ((ccr_value * 100) / (PWM_IMPULSE_PERIOD + 1) + 1)

#define LED_DEFAULT_CONSTRUCTOR(timer_ptr, chan)                               \
    (led_t)                                                                    \
    {                                                                          \
        .brightness = LED_DEFAULT_BRIGHTNESS, .timer = (timer_ptr),            \
        .channel = (chan)                                                      \
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

static struct {
    leds_list_t leds_list;
} driver_scope;

// Private functions

static void start_pwm_tim(TIM_HandleTypeDef *tim)
{
    HAL_TIM_PWM_Start(tim, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(tim, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(tim, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(tim, TIM_CHANNEL_4);
}

static void start_pwm(struct leds_initial *init_data)
{
    uint32_t sz = init_data->sz;
    for (uint32_t i = 0; i < sz; i++) {
        TIM_HandleTypeDef **tims = (TIM_HandleTypeDef **)init_data->tims;
        start_pwm_tim(tims[i]);
    }
}

static void init_leds_tim(TIM_HandleTypeDef *tim,
                          struct leds_list *leds,
                          unsigned int led_list_offset)
{
    for (size_t chan = 1; chan <= LED_CHANNELS_PER_TIM; chan++) {
        led_t *led = &(leds->leds[led_list_offset]);
        *led = LED_DEFAULT_CONSTRUCTOR(tim, chan);
        led_set_brightness(led, LED_DEFAULT_BRIGHTNESS);
        led_list_offset++;
    }
}

static void init_leds(struct leds_list *leds, struct leds_initial *init_data)
{
    leds->sz = init_data->sz * LED_CHANNELS_PER_TIM;
    leds->leds = malloc(leds->sz * sizeof(struct led));

    for (uint32_t i = 0; i < leds->sz; i++) {
        uint32_t offset = LED_CHANNELS_PER_TIM * i;
        TIM_HandleTypeDef **tims = (TIM_HandleTypeDef **)(init_data->tims);
        init_leds_tim(tims[i], leds, offset);
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

// Public functions implementation

void leds_driver_init(struct leds_initial *init_data)
{
    start_pwm(init_data);
    init_leds(&(driver_scope.leds_list), init_data);
}

leds_list_t *leds_get(void)
{
    return &(driver_scope.leds_list);
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

void leds_display_number(leds_list_t *leds, unsigned char num)
{
    for (unsigned char i = 0; i < leds->sz; i++) {
        unsigned char br =
            bit_at(num, i) ? LED_MAX_BRIGHTNESS : LED_MIN_BRIGHTNESS;
        led_set_brightness(leds->leds + i, br);
    }
}

led_t *leds_at(leds_list_t *leds, unsigned int idx)
{
    return &(leds->leds[idx]);
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
    led->brightness = percent;
}

unsigned int led_get_brightness(led_t *led)
{
    return led->brightness;
}