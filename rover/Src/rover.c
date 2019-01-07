#include "rover.h"
#include "tim.h"

struct pwm_channel {
    uint32_t tim_channel;
    uint32_t current_val;
    uint16_t delta;

    const uint32_t pwm_max;
    const uint32_t pwm_min;
};

struct rover {
    TIM_HandleTypeDef *tim;
    struct pwm_channel servo;
    struct pwm_channel esc;
};

enum WheelsTurn {
    WheelsTurn_ToTheRight,
    WheelsTurn_ToTheLeft,
};

struct rover rover_create(TIM_HandleTypeDef *tim,
                          struct pwm_channel servo,
                          struct pwm_channel esc)
{
    return (struct rover){ .tim = tim, .servo = servo, .esc = esc };
}

int rover_whl_turn(struct rover *rover, enum WheelsTurn turn)
{
    struct pwm_channel *servo = &(rover->servo);
    uint32_t chan = servo->tim_channel;
    uint32_t next_pwm =
        servo->current_val +
        (turn == WheelsTurn_ToTheRight ? servo->delta : -servo->delta);

    if (next_pwm > servo->pwm_max || next_pwm < servo->pwm_min)
        return -1;
    __HAL_TIM_SET_COMPARE(rover->tim, chan, next_pwm);
    return 0;
}