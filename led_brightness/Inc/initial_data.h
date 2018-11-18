#ifndef INITIAL_DATA_H
#define INITIAL_DATA_H

#include "stm32f3xx_hal.h"

typedef struct btn_initial {
    uint16_t pin;
    GPIO_TypeDef *port;
} btn_initial_t;

typedef struct btns_initial {
    btn_initial_t *btns_inits;
    unsigned int sz;
} btns_initial_t;

#endif