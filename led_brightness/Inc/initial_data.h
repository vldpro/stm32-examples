#ifndef INITIAL_DATA_H
#define INITIAL_DATA_H

#include <stdint.h>

// btns_driver initial

typedef struct btn_initial {
    uint16_t pin;
    void *port;
} btn_initial_t;

typedef struct btns_initial {
    btn_initial_t *btns_inits;
    unsigned int sz;
} btns_initial_t;

// led_driver initial

typedef struct leds_initial {
    void *tims;
    uint32_t sz;
} leds_initial_t;

#endif