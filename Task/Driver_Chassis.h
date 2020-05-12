
#ifndef __CHASSIS_H
#define __CHASSIS_H

#include "stdint.h"
#include "cmsis_os.h"
//#include "PID.h"


#define CHASSIS_SPEED_ATTENUATION (1.0f)
#define Ang2Rad(m)  (m/180.0f*M_PI)

typedef __packed struct
{
    int16_t Y;
    int16_t X;
    int16_t Omega;
}ChassisSpeed_Ref_TypeDef;

typedef struct
{
	float ChassisAngle;
	int16_t ChassisWheelSpeedRef[4];
	ChassisSpeed_Ref_TypeDef ChassisSpeedRef;
}ChassisDataTypeDef;

//控制模式的枚举类型 
typedef enum
{		Chassis_Locked,
    Chassis_NormalRCMode,
    Chassis_KeyMouseMode,
}ChassisMode_e;


//键鼠控制的一些宏定义，在Key2speed中使用 
#define NORMAL_FORWARD_BACK_SPEED 			500
#define NORMAL_LEFT_RIGHT_SPEED   			500
#define HIGH_FORWARD_BACK_SPEED 			750//600
#define HIGH_LEFT_RIGHT_SPEED   			650//500
#define LOW_FORWARD_BACK_SPEED 				300
#define LOW_LEFT_RIGHT_SPEED   				300

#define MOUSE_LR_RAMP_TICK_COUNT			300//50
#define MOUSR_FB_RAMP_TICK_COUNT			800
#define KEY_FACTOR_NORMAL             15

void CM_Get_SpeedRef(void);
void Key2Speed(int16_t FB, int16_t LR);
void Chassis_DecodeThreadCreate(osPriority taskPriority);




#define STICK_TO_YAW_ANGLE_INC_FACT         0.004f//0.004f
#define MOUSE_TO_YAW_ANGLE_INC_FACT 		0.07f//0.1f



extern ChassisDataTypeDef ChassisData;
extern ChassisMode_e ChassisMode;



#endif
