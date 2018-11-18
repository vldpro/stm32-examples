#ifndef HARDWARE_INIT_H
#define HARDWARE_INIT_H

#include "initial_data.h"

/**
 * @brief Initialize microcontroller hardware
 * 
 */
void hw_init(void);

/**
 * @brief Get initial data for buttons driver
 * Should be called after hw_init()
 * 
 * @return btns_initial_t* initial_data for buttons driver
 */
btns_initial_t *hw_btns_initial_data(void);

/**
 * @brief Get initial data for leds driver 
 * Should be called after hw_init()
 * 
 * @return leds_initial_t* initial_data for leds driver
 */
leds_initial_t *hw_leds_initial_data(void);

#endif