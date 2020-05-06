/**
 * @brief  State control
 * @param	State control
 * @author zcd
 * @Time 2020 5 1 
*/
#ifndef __StatusManagement_H
#define __StatusManagement_H

#include "stdint.h"
#include "Remote_Driver.h"
#include "cmsis_os.h"

#define PREPARE_TIME_TICK_MS 2000

/*-----------------------------------------------------------------------*/
typedef enum
{
    PREPARE_STATE,          //上电后初始状态
    NORMAL_RC_STATE,        //遥控器控制状态
    KEYBOARD_RC_STATE,      //键盘控制状态	
    STOP_STATE,             //停止状态
}WorkState_e;	

typedef enum
{
    Stop_Mode,
    NormalRC_Mode,		//RC--Remote Control
    KeyMouse_Mode,
    Auto_Mode,			//自动模式指的是自动进行某一运动,如取弹等
}OperateMode_e;


typedef enum
{
	Attack_Insurance,   //保险
	Attack_Normal,		//普通单发
	Attack_HalfAuto,	//半自动连发
	Attack_Dragon, 		//打符模式，不知道工程能不能打先留着吧
	Attack_ROTATE,		
} Attack_Mode_e;

typedef enum
{
	Gimbal_Prepare = 0,
	Gimbal_Stop = 1,
	Gimbal_RC_Mode = 2,
	Gimbal_Mouse_Mode = 3,
	Gimbal_Auto = 4,//大符
	Gimbal_Follow = 5,//自动跟踪
	Gimbal_Standby = 6,
	Gimbal_Debug = 7,
	
}Gimbal_MoveMode_t;

typedef  enum
{
	StopMoveLock  = 0x00,
	MoveRC_Normal = 0x01,              //遥控模式
	MoveKEY_Normal = 0x02,
	MoveAuto = 0x03,                 
	MoveDebug = 0x04,
}Chassis_MoveMode_t;

typedef enum
{
	FRICTION_WHEEL_OFF = 0,
	FRICTION_WHEEL_START_TURNNING = 1,
	FRICTION_WHEEL_ON = 2,
}FrictionWheelState_e;

typedef enum
{
	NOSHOOTING = 0,
	SHOOTING = 1,
}Shoot_State_e;

typedef enum  //具体的自动控制状态还没确定，先放着吧
{
	Auto_NoMovement,
	Auto_Get_Bullet,
	Auto_Get_Bullets,
	Auto_Get_Bullets2,
	Auto_Get_Bulletss,
	Auto_Get_I_Bullet,	
	Auto_Get_I_Bullets,
	Auto_Give_Bullet,
	Auto_Cali_For_Bullet,
}AutoMovement_e;
/*----------------------------------------------------------------------*/

extern WorkState_e 	WorkState;
extern OperateMode_e OperateMode;

void StatusMachine_Init(void);
InputMode_e GetInputMode(void);
void StateMachineThreadCreate(osPriority taskPriority);
#endif


