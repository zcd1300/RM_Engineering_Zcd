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
    PREPARE_STATE,          //�ϵ���ʼ״̬
    NORMAL_RC_STATE,        //ң��������״̬
    KEYBOARD_RC_STATE,      //���̿���״̬	
    STOP_STATE,             //ֹͣ״̬
}WorkState_e;	

typedef enum
{
    Stop_Mode,
    NormalRC_Mode,		//RC--Remote Control
    KeyMouse_Mode,
    Auto_Mode,			//�Զ�ģʽָ�����Զ�����ĳһ�˶�,��ȡ����
}OperateMode_e;


typedef enum
{
	Attack_Insurance,   //����
	Attack_Normal,		//��ͨ����
	Attack_HalfAuto,	//���Զ�����
	Attack_Dragon, 		//���ģʽ����֪�������ܲ��ܴ������Ű�
	Attack_ROTATE,		
} Attack_Mode_e;

typedef enum
{
	Gimbal_Prepare = 0,
	Gimbal_Stop = 1,
	Gimbal_RC_Mode = 2,
	Gimbal_Mouse_Mode = 3,
	Gimbal_Auto = 4,//���
	Gimbal_Follow = 5,//�Զ�����
	Gimbal_Standby = 6,
	Gimbal_Debug = 7,
	
}Gimbal_MoveMode_t;

typedef  enum
{
	StopMoveLock  = 0x00,
	MoveRC_Normal = 0x01,              //ң��ģʽ
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

typedef enum  //������Զ�����״̬��ûȷ�����ȷ��Ű�
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


