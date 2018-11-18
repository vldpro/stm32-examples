#include "btns_driver.h"
#include "stm32f3xx_hal.h"
#include <malloc.h>

#define AVAILABLE_BTNS 2
#define TICKS_TRESHOLD 50

// Public structures implementation
#define BTN_DEFAULT (btn_t){ .pin = 0, .port = NULL, .handler = NULL, .ticks = 0 }
struct btn {
    uint16_t pin;
    GPIO_TypeDef *port;
    btn_pushed_fn handler;
    unsigned long ticks;
};

// Private structs impl

#define BTNS_LIST_DEFAULT { .btns = NULL, .sz = 0 }
typedef struct btns_list btns_list_t;
struct btns_list {
    btn_t *btns;
    uint32_t sz;
};

// Driver scope globs

static struct {
    btns_list_t btns_list;
} driver_scope = { .btns_list = BTNS_LIST_DEFAULT };

// Private functions
static void handle_press_on(btn_t *btn)
{
    if (btn->handler == NULL)
        return;

    if (HAL_GPIO_ReadPin(btn->port, btn->pin) == GPIO_PIN_SET) {
        btn->ticks++;
    } else {
        if (btn->ticks > TICKS_TRESHOLD)
            btn->handler();
        btn->ticks = 0;
    }
}

void HAL_SYSTICK_Callback(void)
{
    if (!driver_scope.btns_list.sz)
        return;

    uint32_t sz = driver_scope.btns_list.sz;
    btn_t *btns = driver_scope.btns_list.btns;
    for (uint32_t i = 0; i < sz; i++)
        handle_press_on(btns + i);
}

// Public functions

void btns_init(void)
{
    driver_scope.btns_list.btns = malloc(sizeof(btn_t) * AVAILABLE_BTNS);
    driver_scope.btns_list.btns[0] = (btn_t){
        .port = GPIOA, .pin = GPIO_PIN_10, .handler = NULL, .ticks = 0
    };
    driver_scope.btns_list.btns[1] = (btn_t){
        .port = GPIOC, .pin = GPIO_PIN_13, .handler = NULL, .ticks = 0
    };

    driver_scope.btns_list.sz = AVAILABLE_BTNS;
}

void btns_deinit(void)
{
    free(driver_scope.btns_list.btns);
    driver_scope.btns_list = (btns_list_t)BTNS_LIST_DEFAULT;
}

btn_t *btns_at(unsigned int idx)
{
    return driver_scope.btns_list.btns + idx;
}

void btn_register_press_listener(btn_t *btn, btn_pushed_fn callback)
{
    btn->handler = callback;
}