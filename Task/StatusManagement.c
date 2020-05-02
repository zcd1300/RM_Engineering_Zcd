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
//extern uint32_t time_tick_1ms;


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
void State_Update(void)
{
	LastWorkState = WorkState;
	//Ò£¿ØÆ÷´íÎóÍ£Ö¹
	if()
	{
		WorkState = STOP_STATE;
		return;
	}
	//ÔÆÌ¨¡¢ÍÓÂÝÒÇÐ£×¼
	else if()
	{
	
		return;
	}
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
InputMode_e GetInputMode(void)
{
	return InputMode;
}
//------------------------------------------
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
			
		}break;
		case KEYBOARD_RC_STATE:
		{
		
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


