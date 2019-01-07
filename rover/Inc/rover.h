#include <stddef.h>
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
                          struct pwm_channel esc);

int rover_whl_turn(struct rover *rover, enum WheelsTurn turn)