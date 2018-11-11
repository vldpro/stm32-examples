#ifndef LED_DRIVER_H
#define LED_DRIVER_H

/**
 * @brief Represents the one led
 */
typedef struct led led_t;

/**
 * @brief A list of leds
 */
typedef struct led_list led_list_t;
typedef void(led_handler_fn)(led_t *);

/**
 * @brief Initialize led_list and driver
 * @param leds
 */
led_list_t *leds_new(void);

/**
 * @brief Deinitialize leds and driver
 * @param leds 
 */
void leds_delete(led_list_t *leds);

/**
 * @brief Get number of available leds
 * @param leds - list of leds
 */
void leds_size(led_list_t *leds);

/**
 * @brief Iterate over leds_list
 * @param leds - list of leds
 * @param led_handler - callback
 */
void leds_foreach(led_list_t *leds, led_handler_fn led_handler);

/**
 * @brief Set brightness for led
 * @param led
 * @param percent - value in 1..100
 */
void led_set_brightness(led_t *led, unsigned int percent);

/**
 * @brief Get brightness of led
 * @param led 
 * @return unsigned int - brightness in percents (1..100)
 */
unsigned int led_get_brightness(led_t *led);

#endif