/**
 * @brief  Some mathematical processing functions
 * @param	Mathematical variables to process
 * @retval  Processing the completed mathematical results
 * @author ZCD
 * @Time 2020 1 11
*/
#ifndef __User_Math_H
#define __User_Math_H

//********************************** head **************************************//
#include "stdint.h"


//******************************* Structure ************************************//
typedef struct
{
	float End_Value;		//目标值	
	uint16_t count_SUM;		//自己按需求设定
	float TIME_PerCal;		//每次执行延时时间（可以控制总时间），按需设定
	uint16_t count;			//初始化为0
	float Ramp_Buf;			//初始化为0
	float Output;			//初始化为0
}Ramp_Init_e;
//***************************** External variables *****************************//

//*************************** Function declaration *****************************//
float NumRangeLimit(float Num_Input, float Num_Min, float Num_Max);
int NumRangeLimit_ABS(int Num_Input,int Num_Min, int Num_Max);
uint8_t Value_Change_Trend_ABS (int16_t Num ,int16_t Num_Last);
float Ramp_Increase(Ramp_Init_e* Ramp_Temp);
float Ramp_Decrease(Ramp_Init_e* Ramp_Temp);
//*******************************************************************************//


#endif

