/*
 * dht.c
 *
 *  Created on: Nov 15, 2024
 *      Author: tylermiller
 */
#include "stm32l4xx_hal.h"
#include "delayMicroseconds.h"

#define DHT22_Port GPIOC
#define DHT22_Pin GPIO_PIN_8

void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

void DHT22_Start (void) {

	Set_Pin_Output(DHT22_Port, DHT22_Pin);
	HAL_GPIO_WritePin(DHT22_Port, DHT22_Pin, 0);
	HAL_Delay(1200);

	HAL_GPIO_WritePin(DHT22_Port, DHT22_Pin, 1);
	delayMicroseconds(30);

	Set_Pin_Input(DHT22_Port, DHT22_Pin);

}

uint8_t DHT22_Check_Response (void) {

	Set_Pin_Input(DHT22_Port, DHT22_Pin);
	uint8_t Response = 0;
	delayMicroseconds(40);

	if(!(HAL_GPIO_ReadPin(DHT22_Port, DHT22_Pin))) {

		delayMicroseconds(80);

		if((HAL_GPIO_ReadPin(DHT22_Port, DHT22_Pin))) {

			Response = 1;

		}

		else {

			Response = -1;

		}
	}

	while((HAL_GPIO_ReadPin(DHT22_Port, DHT22_Pin)));

	return Response;

}

uint8_t DHT22_Read (void) {

	uint8_t i,j;

	for(j = 0; j < 8; j++) {

		while(!(HAL_GPIO_ReadPin(DHT22_Port, DHT22_Pin)));
		delayMicroseconds(40);

		if(!(HAL_GPIO_ReadPin(DHT22_Port, DHT22_Pin))) {

			i&= ~(1<<(7-j));

		}

		else {

			i|= (1<<(7-j));

		}

		while ((HAL_GPIO_ReadPin(DHT22_Port, DHT22_Pin)));

	}

	return i;

}

void Set_Pin_Output (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);

}

void Set_Pin_Input (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);

}
