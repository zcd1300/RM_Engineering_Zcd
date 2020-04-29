/**
 * @brief  Control codes for all motors
 * @param	Control codes for all motors
 * @retval
 * @ZCD
 * @Time 2019 12 7 
*/

#ifndef __Motor_ConttrolTask_H
#define __Motor_ConttrolTask_H

#include "CAN_BUS_Driver.h"
#include "cmsis_os.h"
#include "PID.h"
#include "usart.h"
#include "XDU_USB_HID_Z.h"

void Motor_ControlThraedCreate (osPriority taskPriority);

extern volatile float YAW_Initial_Angle;

#endif

