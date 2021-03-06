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
//Chassis_MoveMode_t Move_Mode;

uint32_t time_tick_1ms=0;
uint16_t GimbalCalibrationKEY_JudgeTime =0;
uint8_t Gimbal_Debug_Flag=0;


static void WorkstateInit(void)
{
	if((LastWorkState!=WorkState)&&(WorkState==PREPARE_STATE))
	{
		StatusMachine_Init();//状态机重新初始化
		ChassisData_Init();//底盘速度初始化
		time_tick_1ms=0;//这个用来做整车上电的准备时间计时
	}
	
}	
void SetWorkState(WorkState_e state)
{	WorkState = state;	}
WorkState_e GetWorkState(void)
{	return WorkState;	}



//整车的状态控制,基本完成.(如果没有需要增加的话)(√)
void State_Update(void)
{
	LastWorkState = WorkState;
	
	//以下是几种需要直接转换状态的情况
	//遥控器错误停止
	if(Is_Lost_Error_Set(LOST_ERROR_RC)||InputMode == STOP)
	{
		WorkState = STOP_STATE;
		return;
	}
	//云台、陀螺仪校准
	else if(Gimbal_Debug_Flag)
	{
		//Move_Mode = MoveDebug;//这里先注释掉,用Chassis部分的枚举状态.
		ChassisMode = Chassis_Locked;
		Gimbal_Mode = Gimbal_Debug;
		return;
	}
//	//其他重要错误标志
//	else if()
//	{
//	
//		return;
//	}
//--------------------以上是需要直接转换状态的情况---------------------------//
	
	switch(WorkState)
	{
		case PREPARE_STATE:
		{
			if(time_tick_1ms > PREPARE_TIME_TICK_MS)
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
//------------------------------------------遥控器直接控制(√)
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
//这个函数还没完成，等开学完成
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
				//这里写一下左侧键在上右侧在上应该对应的初始化值啥的，
				//先空着，不知道等疫情开学后和他们联调的时候整。
				
			}
			if(RC_CtrlData.rc.switch_left == SWITCH_CENTRAL)
			{
				OperateMode = NormalRC_Mode;
				AutoMovement = Auto_NoMovement;	
				//这里同样空着，具体实现啥看需求（我可能会写成单纯控制运动）
				//
				
			}
			if(RC_CtrlData.rc.switch_left == SWITCH_DOWN)
			{
				OperateMode = NormalRC_Mode;
				AutoMovement = Auto_NoMovement;	
				//这可能做成电机停止运动，或者当遥控器Channel值大于600时手动触发自动控制
				
				if(RC_CtrlData.rc.Channel_1 >600)
				{
					AutoMovement = Auto_Get_Bullet;
				}
				//其他通道同理，通过if判断即可
			
			}	
		}break;
		case KEYBOARD_RC_STATE:
		{
			OperateMode = KeyMouse_Mode;
			if(AutoMovement != Auto_NoMovement)//其实这些东西可以考虑加在mode下
			{
			//不变
			}
			/********以上是保持动作不变**********/
			
			//检测按键按下进行状态切换以及参数设定
			else if(Remote_CheckJumpKey(KEY_Z))
			{
				AutoMovement = Auto_Give_Bullet;
			}
			//其他按键同理
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
//------------------------------------------攻击模式
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
			else if(Attack_Mode == Attack_Normal)
			{
				Attack_Mode = Attack_HalfAuto;
			}
			else
			{
				Attack_Mode = Attack_Insurance;
			}
			//在不同的攻击模式下轮流进行模式切换

		}
		//这里添加其他按键对应的攻击模式切换(我打算只用一个Q键解决)
	
	}

}
//------------------------------------------云台状态
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
			}break;
		
		}
	}
}

//------------------------------------------底盘状态(√)
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
//------------------------------------------摩擦轮状态
void FrictionMode_Select(void)
{
	switch(WorkState)
	{
		case PREPARE_STATE:
		{
			friction_wheel_state = FRICTION_WHEEL_OFF;
			FrictionWheel_Speed = Speed_Low;
			Shoot_State = NOSHOOTING;
		}break;
		case NORMAL_RC_STATE:
		{
//			if()
//			{
//			
//			}
			
		}break;
		case KEYBOARD_RC_STATE:
		{
		
		}break;
		case STOP_STATE:
		{
			friction_wheel_state = FRICTION_WHEEL_OFF;
			FrictionWheel_Speed = Speed_Low;
			Shoot_State = NOSHOOTING;
		}break;
		default:
		{
			friction_wheel_state = FRICTION_WHEEL_OFF;
			FrictionWheel_Speed = Speed_Low;
			Shoot_State = NOSHOOTING;			
		}break;
	}
}
//-----------------拨盘状态和摩擦轮状态写在一起了,单独的拨盘控制就不写了-----------------//


//-----------------------------------------------------云台校准模式切换
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
	if(GimbalCalibrationKEY_JudgeTime >= 300 && Gimbal_Mode == Gimbal_Stop)//长按3s切换
	{
		Gimbal_Mode = Gimbal_Debug;
		Gimbal_Debug_Flag = 1;
	}
	else if(GimbalCalibrationKEY_JudgeTime >= 300 && Gimbal_Mode == Gimbal_Prepare)
	{
		Gimbal_Mode = Gimbal_Debug;
		Gimbal_Debug_Flag = 1;
	}

}
//------------------------------------------状态机初始化
void StatusMachine_Init(void)
{
	WorkState = PREPARE_STATE;
	AutoMovement = Auto_NoMovement;
	Attack_Mode = Attack_Normal;
	ChassisMode = Chassis_Locked;
	Gimbal_Mode = Gimbal_Stop;
	//还有其他初始化，等完善后再添加
}

//------------------------------------------
void StatusMachine_Update(void)
{
	InputMode_Select();		//遥控器 右侧按键 控制的，输入状态切换（键鼠/遥控/停止）
	State_Update();			//整车状态切换，受InputMode影响
	OperateMode_Select();	//操作模式切换，受WorkState和遥控 左侧按键 影响
	DriverMode_Select();	//运行模式切换，受OperateMode和遥控数据影响
	ChassisMode_Select();	//底盘运动状态控制
	GimbalMode_Select();	//云台状态控制
	AttackMode_Select();	//攻击模式选择
}

void StateMachine(void const* argument)
{
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{	
		StatusMachine_Update();

		GimbalCalibrationKEY_Judge();

		if(time_tick_1ms<1500)
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

