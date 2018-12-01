#include "uart_int_driver.h"

#include <stdint.h>
#include <stddef.h>

#include "gpio.h"
#include "rtc.h"
#include "usb.h"
#include "usart.h"

//
// Temporary globals
//

#define BUF_SZ 4096
static uint8_t g_rbuf[BUF_SZ] = { 0 };
static uint8_t g_wbuf[BUF_SZ] = { 0 };

//
// Module's internal structs
//

typedef struct fifo {
    uint8_t *data;
    uint32_t cap;
    uint32_t roff;
    uint32_t woff;
} fifo_t;

#define FIFO_DEFAULT_CONSTRUCTOR()                                             \
    {                                                                          \
        .data = NULL, .cap = 0, .roff = 0, .woff = 0                           \
    }

#define FIFO_CONSTRUCTOR(buf, sz)                                              \
    {                                                                          \
        .data = buf, .cap = sz, .roff = 0, .woff = 0                           \
    }

static struct mod_scope {
    fifo_t wfifo;
    fifo_t rfifo;
};

struct mod_scope mod_scope = { .wfifo = FIFO_DEFAULT_CONSTRUCTOR(),
                               .rfifo = FIFO_DEFAULT_CONSTRUCTOR() };

//
// Private functions
//

static void fifo_push_back(fifo_t *fifo, uint8_t val)
{
    fifo->data[fifo->woff] = val;
    fifo->woff++;
    fifo->woff = fifo->woff % fifo->cap; // prevent buffer overflow
}

static uint8_t fifo_pop_front(fifo_t *fifo)
{
    uint8_t res = fifo->data[fifo->roff];
    fifo->roff++;
    fifo->roff = fifo->roff % fifo->cap; // prevent buffer overflow
    return res;
}

static uint8_t fifo_is_empty(fifo_t const *fifo)
{
    return fifo->roff == fifo->woff;
}

//
// Public functions
//

void iuart_init(void)
{
    mod_scope = (struct mod_scope){ .rfifo = FIFO_CONSTRUCTOR(g_rbuf, BUF_SZ),
                                    .wfifo = FIFO_CONSTRUCTOR(g_wbuf, BUF_SZ) };
    __HAL_UART_ENABLE_IT(&huart4, UART_IT_RXNE);
}

void iuart_transmit(uint8_t val)
{
    /* disable interrupts before modifying the fifo */

    uint8_t fifo_was_empty = fifo_is_empty(&mod_scope.wfifo);
    fifo_push_back(&mod_scope.wfifo, val);

    /* enable interrupts again */
    __HAL_UART_ENABLE_IT(&huart4, UART_IT_TXE);
}

int8_t iuart_receive(uint8_t *buf)
{
    if (fifo_is_empty(&mod_scope.rfifo))
        return -1;
    *buf = fifo_pop_front(&mod_scope.rfifo);
    return 0;
}

void uart_interrupt_handler(UART_HandleTypeDef *huart)
{
    if (__HAL_UART_GET_IT(huart, UART_IT_TXE) != RESET)
        if (!fifo_is_empty(&mod_scope.wfifo)) {
            uint8_t byte = fifo_pop_front(&mod_scope.wfifo);
            huart->Instance->TDR = byte;
        } else {
            __HAL_UART_DISABLE_IT(&huart4, UART_IT_TXE);
        }

    if (__HAL_UART_GET_IT(huart, UART_IT_RXNE) != RESET) {
        uint8_t byte = (huart->Instance->RDR & (uint16_t)0x00FF);
        fifo_push_back(&mod_scope.rfifo, byte);
        __HAL_UART_SEND_REQ(huart, UART_RXDATA_FLUSH_REQUEST);
    }
}