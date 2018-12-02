#include "uart_int_driver.h"

#include <stdint.h>
#include <stddef.h>

#include "gpio.h"
#include "rtc.h"
#include "usb.h"
#include "usart.h"

#include "fifo.h"

//
// Module's internal structs
//

static struct mod_scope {
    fifo_t wfifo;
    fifo_t rfifo;
    UART_HandleTypeDef *huart;
};

struct mod_scope mod_scope = { .wfifo = FIFO_INITIAL_DATA,
                               .rfifo = FIFO_INITIAL_DATA };

//
// Public functions
//

void iuart_init(UART_HandleTypeDef *uartHandle,
                uint8_t *rbuf,
                uint32_t rbuf_sz,
                uint8_t *wbuf,
                uint32_t wbuf_sz)
{
    fifo_init(&mod_scope.rfifo, rbuf, rbuf_sz);
    fifo_init(&mod_scope.wfifo, wbuf, wbuf_sz);
    mod_scope.huart = uartHandle;
    __HAL_UART_ENABLE_IT(mod_scope.huart, UART_IT_RXNE);
}

iuart_res_t iuart_transmit(uint8_t val)
{
    fifo_res_t res = fifo_push_back(&mod_scope.wfifo, val);
    if (res == FIFO_FULL)
        return IUART_BUF_FULL;
    __HAL_UART_ENABLE_IT(mod_scope.huart, UART_IT_TXE);
    return IUART_SUCCESS;
}

iuart_res_t iuart_receive(uint8_t *buf)
{
    fifo_res_t res = fifo_pop_front(&mod_scope.rfifo, buf);
    return res == FIFO_EMPTY ? IUART_NO_DATA : IUART_SUCCESS;
}

void uart_interrupt_handler(UART_HandleTypeDef *huart)
{
    if (__HAL_UART_GET_IT(huart, UART_IT_TXE) != RESET)
        if (!fifo_is_empty(&mod_scope.wfifo)) {
            uint8_t byte = 0;
            fifo_pop_front(&mod_scope.wfifo, &byte);
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