/**
 * @brief  State control
 * @param	State control
 * @author zcd
 * @Time 2020 5 1 
*/
#include "StatusManagement.h"
#include "Motor_ConttrolTask.h"
#include "Remote_Driver.h"
#include "SuperviseTask.h"
#include "Friction.h"
#include "Driver_Chassis.h"

WorkState_e WorkState;
WorkState_e LastWorkState =STOP_STATE;
OperateMode_e OperateMode;
AutoMovement_e AutoMovement;
Attack_Mode_e Attack_Mode;
Gimbal_MoveMode_t Gimbal_Mode;
Chassis_MoveMode_t Move_Mode;

uint32_t time_tick_1ms=0;
uint8_t GimbalCalibration_Flag=0;
uint8_t GimbalCalibrationKEY_JudgeTime =0;
uint8_t Gimbal_Debug_Flag=0;

//����δ���
static void WorkstateInit(void)
{
	if((LastWorkState!=WorkState)&&(WorkState==PREPARE_STATE))
	{
		
		
	}
	
}	
void SetWorkState(WorkState_e state)
{	WorkState = state;	}
WorkState_e GetWorkState(void)
{	return WorkState;	}
//--------------------------------------------
//����δ��ɣ��������������
void State_Update(void)
{
	LastWorkState = WorkState;
	
	//�����Ǽ�����Ҫֱ��ת��״̬�����
	//ң��������ֹͣ
	if(Is_Lost_Error_Set(LOST_ERROR_RC)||InputMode == STOP)
	{
		WorkState = STOP_STATE;
		return;
	}
	//��̨��������У׼
	else if(GimbalCalibration_Flag)
	{
		Move_Mode = MoveDebug;
		Gimbal_Mode = Gimbal_Debug;
		//����У׼�ĺ�����ûд��д���˼ǵ�ɾ��
		return;
	}
//	//���������־
//	else if()
//	{
//	
//		return;
//	}
	//��������Ҫֱ��ת��״̬�����
	
	switch(WorkState)
	{
		case PREPARE_STATE:
		{
			if(time_tick_1ms > PREPARE_TIME_TICK_MS)//���ﻹ�и�����û������
			{
				if(InputMode == REMOTE_INPUT)
				{
				WorkState = NORMAL_RC_STATE;
				}
				else if(InputMode == KEYBOARD_INPUT)
				{
				WorkState = KEYBOARD_RC_STATE;
				}
			}		
		}break;
		case NORMAL_RC_STATE:
		{
			if(InputMode == KEYBOARD_INPUT)
			{
				WorkState = PREPARE_STATE;
			}		
			else if(InputMode == STOP)
			{
				WorkState = STOP_STATE;
			}		
		}break;
		case KEYBOARD_RC_STATE:
		{
			if(InputMode == REMOTE_INPUT)
			{
				WorkState = PREPARE_STATE;
			}
			else if(InputMode == STOP)
			{
				WorkState = STOP_STATE;
			}		
		
		}break;
		case STOP_STATE:
		{
			if(InputMode != STOP)
			{
				WorkState = PREPARE_STATE;
			}		
		}break;
		
	}
	WorkstateInit();
}
//------------------------------------------ң����ֱ�ӿ���(��)
void InputMode_Select(void)
{
	if(RC_CtrlData.rc.switch_right == SWITCH_UP)
	{
		InputMode = REMOTE_INPUT;
	}
	if(RC_CtrlData.rc.switch_right == SWITCH_CENTRAL)
	{
		InputMode = STOP;
	}
	if(RC_CtrlData.rc.switch_right == SWITCH_DOWN)
	{
		InputMode = KEYBOARD_INPUT;
	}
}


//------------------------------------------
//���������û��ɣ��ȿ�ѧ���
void OperateMode_Select(void)
{
	switch(WorkState)
	{
		case PREPARE_STATE:
		{
			OperateMode = Stop_Mode;
		}break;
		case NORMAL_RC_STATE:
		{
			OperateMode = NormalRC_Mode;
			AutoMovement = Auto_NoMovement;
			
			if(RC_CtrlData.rc.switch_left == SWITCH_UP)
			{
				OperateMode = NormalRC_Mode;
				AutoMovement = Auto_NoMovement;
				//����дһ�����������Ҳ�����Ӧ�ö�Ӧ�ĳ�ʼ��ֵɶ�ģ�
				//�ȿ��ţ���֪�������鿪ѧ�������������ʱ������
				
			}
			if(RC_CtrlData.rc.switch_left == SWITCH_CENTRAL)
			{
				OperateMode = NormalRC_Mode;
				AutoMovement = Auto_NoMovement;	
				//����ͬ�����ţ�����ʵ��ɶ�������ҿ��ܻ�д�ɵ��������˶���
				//
				
			}
			if(RC_CtrlData.rc.switch_left == SWITCH_DOWN)
			{
				OperateMode = NormalRC_Mode;
				AutoMovement = Auto_NoMovement;	
				//��������ɵ��ֹͣ�˶������ߵ�ң����Channelֵ����600ʱ�ֶ������Զ�����
				
				if(RC_CtrlData.rc.Channel_1 >600)
				{
					AutoMovement = Auto_Get_Bullet;
				}
				//����ͨ��ͬ��ͨ��if�жϼ���
			
			}	
		}break;
		case KEYBOARD_RC_STATE:
		{
			OperateMode = KeyMouse_Mode;
			if(AutoMovement != Auto_NoMovement)//��ʵ��Щ�������Կ��Ǽ���mode��
			{
			//����
			}
			/********�����Ǳ��ֶ�������**********/
			
			//��ⰴ�����½���״̬�л��Լ������趨
			else if(Remote_CheckJumpKey(KEY_Z))
			{
				AutoMovement = Auto_Give_Bullet;
			}
			//��������ͬ��
		}break;
		case STOP_STATE:
		{
			OperateMode = Stop_Mode;
		}break;
		default:
		{
			OperateMode = Stop_Mode;
		}break;
	}
}

//-----------------------------------------
void DriverMode_Select(void)
{
  switch (OperateMode)
  {
	  case Stop_Mode:
	  {
		  
	  }break;
	  case 	NormalRC_Mode:
	  {
	  
	  }break;
	  case KeyMouse_Mode:
	  {
		  
	  }break;
	  case Auto_Mode:
	  {
	  
	  }break;
	  default:
	  {
	  
	  }break;
  }
}
//------------------------------------------����ģʽ
void AttackMode_Select(void)
{
	static uint32_t time_stamp_1 = 0;
	if(InputMode == KEYBOARD_INPUT)
	{
		if(Remote_CheckJumpKey(KEY_Q)==1 && xTaskGetTickCount()- time_stamp_1 > 200000)
		{
			if(Attack_Mode == Attack_Insurance)
			{	
				Attack_Mode = Attack_Normal;
			}
			else
			{
			
			}
			//�ڲ�ͬ�Ĺ���ģʽ�²Ž���ģʽ�л�
		}
		//�����������������Ӧ�Ĺ���ģʽ�л�
	
	}

}
//------------------------------------------��̨״̬
void GimbalMode_Select(void)
{
	if(Gimbal_Debug_Flag == 1)
	{
		Gimbal_Mode = Gimbal_Debug;
	}
	else
	{
		switch(WorkState)
		{
			case PREPARE_STATE:
			{
				Gimbal_Mode = Gimbal_Prepare;
			}break;
			case NORMAL_RC_STATE:
			{
				Gimbal_Mode = Gimbal_RC_Mode;
			}break;
			case KEYBOARD_RC_STATE:
			{
				Gimbal_Mode = Gimbal_Mouse_Mode;
			}break;
			case STOP_STATE:
			{
				Gimbal_Mode = Gimbal_Stop;
			}break;
			default:
			{
				Gimbal_Mode = Gimbal_Stop;
			}
		
		}
	}
}

//------------------------------------------����״̬(��)
void ChassisMode_Select(void)
{
	switch(WorkState)
	{
		case PREPARE_STATE:
		{
			ChassisMode = Chassis_Locked;
		
		}break;
		case NORMAL_RC_STATE:
		{
			ChassisMode = Chassis_NormalRCMode;
		}break;
		case KEYBOARD_RC_STATE:
		{
			ChassisMode = Chassis_KeyMouseMode;
		}break;
		case STOP_STATE:
		{
			ChassisMode = Chassis_Locked;		
		}break;
		default:
		{
			ChassisMode = Chassis_Locked;
		}break;
	}


}
//------------------------------------------Ħ����״̬
void FrictionMode_Select(void)
{
	switch(WorkState)
	{
		case PREPARE_STATE:
		{
		
		}break;
		case NORMAL_RC_STATE:
		{
		
		}break;
		case KEYBOARD_RC_STATE:
		{
		
		}break;
		case STOP_STATE:
		{
		
		}break;
		default:
		{
			
		}
	}
}
//------------------------------------------����״̬�ȿ��Űɣ������״̬�л�����������Ҫ����д


//-----------------------------------------------------��̨У׼ģʽ�л�
void GimbalCalibrationKEY_Judge(void)
{
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2) == GPIO_PIN_SET && GimbalCalibrationKEY_JudgeTime < 210)
	{
		GimbalCalibrationKEY_JudgeTime++;	
	}
	else
	{
		GimbalCalibrationKEY_JudgeTime = 0;
	}
	if(GimbalCalibrationKEY_JudgeTime >= 200 && Gimbal_Mode == Gimbal_Stop)//����2s�л�
	{
		Gimbal_Mode = Gimbal_Debug;
		Gimbal_Debug_Flag = 1;
	}
	else if(GimbalCalibrationKEY_JudgeTime >= 200 && Gimbal_Mode == Gimbal_Prepare)
	{
		Gimbal_Mode = Gimbal_Debug;
		Gimbal_Debug_Flag = 1;
	}

}
//------------------------------------------״̬����ʼ��
void StatusMachine_Init(void)//Ŀǰ��û�����ã����ϵ�ʱӦ�ñ����á����л�prepareʱҲӦ�õ���
{
	WorkState = PREPARE_STATE;
	AutoMovement = Auto_NoMovement;
	Attack_Mode = Attack_Normal;
	ChassisMode = Chassis_Locked;
	Gimbal_Mode = Gimbal_Stop;
	//����������ʼ���������ƺ������
}

//------------------------------------------
void StatusMachine_Update(void)
{
	InputMode_Select();		//ң���� �Ҳఴ�� ���Ƶģ�����״̬�л�������/ң��/ֹͣ��
	State_Update();			//����״̬�л�����InputModeӰ��
	OperateMode_Select();	//����ģʽ�л�����WorkState��ң�� ��ఴ�� Ӱ��
	DriverMode_Select();	//����ģʽ�л�����OperateMode��ң������Ӱ��
	ChassisMode_Select();
	GimbalMode_Select();
}

void StateMachine(void const* argument)
{
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{	
		StatusMachine_Update();

		GimbalCalibrationKEY_Judge();

		if(time_tick_1ms<2000)
		{
			time_tick_1ms++;
		}
		osDelayUntil(&xLastWakeTime,10/portTICK_RATE_MS);
	}	
}
osThreadId StateMachineHandle;

void StateMachineThreadCreate(osPriority taskPriority)
{
	osThreadDef(StateMachineThread,StateMachine,taskPriority,0,256);
	StateMachineHandle = osThreadCreate(osThread(StateMachineThread),NULL);
}

/**
  @CompletionTime 2020 5 18
*/

