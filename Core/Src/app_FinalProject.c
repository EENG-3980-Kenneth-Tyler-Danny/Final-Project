/*
 * app_DHT22Test.c
 *
 *  Created on: Nov 15, 2024
 *      Author: tylermiller
 */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "stm32l4xx_hal.h"
#include "app.h"
#include "lcd.h"
#include "dht.h"
#include "delayMicroseconds.h"
#include "bmp280.h"

extern TIM_HandleTypeDef htim2;
extern I2C_HandleTypeDef hi2c1;
extern BMP280_HandleTypedef bmp280;

char msg[16];
char PresStr[16];
char TempHumStr[16];
float temperature, pressure, humidity;
uint8_t Presence = 0;
uint8_t Rh_byte1 = 0;
uint8_t Rh_byte2 = 0;
uint8_t Temp_byte1 = 0;
uint8_t Temp_byte2 = 0;
uint16_t SUM = 0;
uint16_t TEMP = 0;
uint16_t RH = 0;
float Temperature = 0;
float Humidity = 0;
float avgtemp = 0;

void App_Init(void) {

	initLCD();

	bmp280_init_default_params(&bmp280.params);
	bmp280.addr = BMP280_I2C_ADDRESS_0;
	bmp280.i2c = &hi2c1;

	while (!bmp280_init(&bmp280, &bmp280.params)) {

		sprintf(msg,"BMP Init");
		clearLCD();
		writeLCD(msg);
		setCursor(0,1);
		sprintf(msg,"Failed");
		HAL_Delay(1000);

	}

	HAL_TIM_Base_Start(&htim2);

}

void App_MainLoop(void) {

	HAL_Delay(1000);

	while (!bmp280_read_float(&bmp280, &temperature, &pressure, &humidity)) {

		sprintf(msg,"BMP Reading");
		clearLCD();
		writeLCD(msg);
		setCursor(0,1);
		sprintf(msg,"Failed");
		HAL_Delay(1000);

	}

	DHT22_Start();
	Presence = DHT22_Check_Response();
	Rh_byte1 = DHT22_Read ();
	Rh_byte2 = DHT22_Read ();
	Temp_byte1 = DHT22_Read ();
	Temp_byte2 = DHT22_Read ();
	SUM = DHT22_Read();

	if (Presence == 1) {

	}

	else {

		sprintf(msg,"DHT Error");
		clearLCD();
		writeLCD(msg);
		HAL_Delay(1000);
		return;

	}

	if (SUM != (Rh_byte1+Rh_byte2+Temp_byte1+Temp_byte2)) {

		sprintf(msg,"DHT CS Error");
		clearLCD();
		writeLCD(msg);
		HAL_Delay(300);
		return;

	}

	TEMP = ((Temp_byte1<<8)|Temp_byte2);
	RH = ((Rh_byte1<<8)|Rh_byte2);

	Temperature = TEMP/10.0;
	Humidity = RH/10.0;

	Temperature = Temperature*(9.0/5.0)+32.0;

	pressure = pressure/1000.0;
	temperature = temperature*(9.0/5.0)+32.0;

	avgtemp = (temperature+Temperature)/2.0;

	sprintf(TempHumStr,"T: %.1f H: %.1f",avgtemp,Humidity);
	sprintf(PresStr,"P: %.1f",pressure);
    clearLCD();
	writeLCD(TempHumStr);
	setCursor(0,1);
	writeLCD(PresStr);

	HAL_Delay(2000);

}
