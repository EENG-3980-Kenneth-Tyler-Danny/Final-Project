/*
 * dht.h
 *
 *  Created on: Nov 15, 2024
 *      Author: tylermiller
 */

#ifndef INC_DHT_H_
#define INC_DHT_H_

#include "stm32l4xx_hal.h"

void DHT22_Start(void);
uint8_t DHT22_Check_Response(void);
uint8_t DHT22_Read(void);

#endif /* INC_DHT_H_ */
