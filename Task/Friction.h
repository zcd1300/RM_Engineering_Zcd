/**
 * @brief  Friction motor and Bullet plate Control
 * @param	Control the friction wheel motor, transmission and dial motor
 * @author zcd
 * @Time 2020 1 9 
*/


#ifndef __Friction_H
#define __Friction_H

#include "stdint.h"
#include "cmsis_os.h"
/******************************Function declaration*********************/
void BulletPlate_Control(void);
void Frition_Control(void);
void FrictionThreadCreate(osPriority taskPriority);
/************************ structural morphology*************************/
typedef enum
{
	FRICTION_WHEEL_OFF = 0,
	FRICTION_WHEEL_START_TURNNING = 1,
	FRICTION_WHEEL_ON = 2,
	
}FrictionWheelState_e;

typedef enum
{
	Speed_Low =1, 
	Speed_Hight=2,
}FrictionWheelSpeed_Mode_e;

typedef enum
{
	NOSHOOTING = 0,
	SHOOTING = 1,
}Shoot_State_e;

typedef struct
{
	int16_t Speed;
	float	Angle;
}BulletPlate_ref_t;

/******************************External variables***********************/
extern Shoot_State_e Shoot_State;
extern FrictionWheelState_e friction_wheel_state;
extern FrictionWheelSpeed_Mode_e FrictionWheel_Speed;

/***********************************************************************/

#endif

