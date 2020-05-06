/**
 * @brief  State control
 * @param	State control
 * @author zcd
 * @Time 2020 5 1 
*/
#include "StatusManagement.h"
#include "Motor_ConttrolTask.h"
#include "Remote_Driver.h"


WorkState_e WorkState;
WorkState_e LastWorkState =STOP_STATE;
OperateMode_e OperateMode;
AutoMovement_e AutoMovement;
Attack_Mode_e Attack_Mode;
Gimbal_MoveMode_t Gimbal_Mode;
Chassis_MoveMode_t Move_Mode;

extern uint32_t time_tick_1ms;

//函数未完成
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
//函数未完成，报错是正常情况
void State_Update(void)
{
	LastWorkState = WorkState;
	
	//以下是几种需要直接转换状态的情况
	//遥控器错误停止
	if()
	{
		WorkState = STOP_STATE;
		return;
	}
	//云台、陀螺仪校准
	else if()
	{
	
		return;
	}
	//其他错误标志
	else if()
	{
	
		return;
	}
	//以上是需要直接转换状态的情况
	
	switch(WorkState)
	{
		case PREPARE_STATE:
		{
			if(time_tick_1ms > PREPARE_TIME_TICK_MS)//这里还有个问题没处理！！
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
//------------------------------------------
void InputMode_Select(void)
{
	if(RC_CtrlData.rc.switch_right == SWITCH_UP)
	{
		InputMode = REMOTE_INPUT;
	}
	if(RC_CtrlData.rc.switch_right == SWITCH_CENTRAL)
	{
		InputMode = KEYBOARD_INPUT;
	}
	if(RC_CtrlData.rc.switch_right == SWITCH_DOWN)
	{
		InputMode = STOP;
	}
}


//------------------------------------------
//这个函数还没完成，只能等开学完成了
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
		if(Remote_CheckJumpKey(KEY_Q)==1 && )
		{
			if(Attack_Mode = )
			{	
				Attack_Mode = Attack_Normal;
			}
			else
			{
			
			}
			//在不同的攻击模式下才进行模式切换
		}
		//这里添加其他按键对应的攻击模式切换
	
	}

}
//------------------------------------------云台状态
void GimbalMode_Select(void)
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
			Gimbal_Mode = Gimbal_Stop;
		}
	
	}

}

//------------------------------------------底盘状态
void ChassisMode_Select(void)
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
//------------------------------------------摩擦轮状态
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
//------------------------------------------拨盘状态先空着吧，这个的状态切换函数可能需要单独写



//------------------------------------------状态机初始化
void StatusMachine_Init(void)//目前还没被调用，在上电时应该被调用。在切会prepare时也应该调用
{
	WorkState = PREPARE_STATE;
	AutoMovement = Auto_NoMovement;
	Attack_Mode = Attack_Normal;
	//还有其他初始化，等完善后再添加
	

}

//------------------------------------------
void StatusMachine_Update(void)
{
	InputMode_Select();		//遥控器 右侧按键 控制的，输入状态切换（键鼠/遥控/停止）
	State_Update();			//整车状态切换，受InputMode影响
	OperateMode_Select();	//操作模式切换，受WorkState和遥控 左侧按键 影响
	DriverMode_Select();		//运行模式切换，受OperateMode和遥控数据影响
}


void StateMachine(void const* argument)
{
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{	
		StatusMachine_Update();
		osDelay(10/portTICK_RATE_MS);
	}	
}
osThreadId StateMachineHandle;

void StateMachineThreadCreate(osPriority taskPriority)
{
	osThreadDef(StateMachineThread,StateMachine,taskPriority,0,256);
	StateMachineHandle = osThreadCreate(osThread(StateMachineThread),NULL);
}

/**
  @CompletionTime 2020 5 2
*/