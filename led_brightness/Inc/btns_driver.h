#ifndef BTNS_DRIVER_H
#define BTNS_DRIVER_H

typedef struct btn btn_t;
typedef void (*btn_pushed_fn)(void);

/**
 * @brief 
 * 
 * @return btns_list_t* 
 */
void btns_init(void);

/**
 * @brief 
 * 
 * @param list 
 */
void btns_deinit(void);

/**
 * @brief 
 * 
 * @param list 
 * @param idx 
 */
btn_t *btns_at(unsigned int idx);

/**
 * @brief 
 * 
 * @param btn 
 * @param callback 
 */
void btn_register_press_listener(btn_t *btn, btn_pushed_fn callback);

#endif