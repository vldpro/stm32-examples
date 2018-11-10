/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f3xx.h"
#include "stm32f3xx_nucleo_144.h"

void init_led(void)
{
	GPIO_InitTypeDef init = {0};
	__HAL_RCC_GPIOC_CLK_ENABLE();

	init.Pin = GPIO_PIN_13;
	init.Mode = GPIO_MODE_OUTPUT_PP;
	init.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &init);
}

int main(void)
{
	HAL_Init();
	init_led();

	for(;;) {
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		HAL_Delay(200);
	}
}
