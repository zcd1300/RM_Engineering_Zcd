/**
 * @brief  Device power up init function
 * @param	
 * @retval
 * @author ZCD
 * @Time 2020 4 18 
*/
#ifndef __PowerUP_Init_H
#define __PowerUP_Init_H

#include "stm32f4xx_hal.h"
#include "stdint.h"
#include "Flash_Operation.h"

//---------------------------------//
typedef struct
{
	uint16_t YAW_Angle_OFFSET;
	uint16_t PITCH_Angle_OFFSET;
	uint16_t InitFlag;
	
}GimbalOffset_SAVE;

void LED_GPIO_Init(void);


#endif


