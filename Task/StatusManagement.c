/**
 * @brief  State control
 * @param	State control
 * @author zcd
 * @Time 2020 5 1 
*/
#include "StatusManagement.h"
#include "Motor_ConttrolTask.h"



WorkState_e WorkState;
WorkState_e LastWorkState =STOP_STATE;

//OperateMode_e OperateMode;
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

void State_Update(void)
{
	LastWorkState = WorkState;
	//ң��������ֹͣ
	if()
	{
		WorkState = STOP_STATE;
		return;
	}
	//��̨��������У׼
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




