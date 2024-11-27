/*
 * delayMicroseconds.c
 *
 *  Created on: Nov 15, 2024
 *      Author: tylermiller
 */

#include "stm32l4xx_hal.h"

extern TIM_HandleTypeDef htim2;

void delayMicroseconds(uint16_t time) {

	__HAL_TIM_SET_COUNTER(&htim2, 0);
	while((__HAL_TIM_GET_COUNTER(&htim2))<time);
}
