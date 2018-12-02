#include "uart_poll_driver.h"

#include "usart.h"

static UART_HandleTypeDef *huart = &huart4;

static HAL_StatusTypeDef wait_until_flag_status(uint32_t flag,
                                                FlagStatus status)
{
    uint32_t tickstart = HAL_GetTick();
    uint32_t timeout = HAL_MAX_DELAY;
    return UART_WaitOnFlagUntilTimeout(huart, flag, status, tickstart, timeout);
}

void puart_init(void)
{
}

puart_res_t puart_transmit(uint8_t val)
{
    if (wait_until_flag_status(UART_FLAG_TXE, RESET) != HAL_OK)
        return PUART_IO_TIMEOUT;
    huart->Instance->TDR = val;
    if (wait_until_flag_status(UART_FLAG_TC, RESET) != HAL_OK)
        return PUART_IO_TIMEOUT;
    return PUART_SUCCESS;
}

uint8_t puart_receive(void)
{
    while (wait_until_flag_status(UART_FLAG_RXNE, RESET) != HAL_OK)
        ;
    return (huart->Instance->RDR & (uint16_t)0x00FF);
}