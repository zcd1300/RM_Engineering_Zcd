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
	float End_Value;		//Ŀ��ֵ	
	uint16_t count_SUM;		//�Լ��������趨
	float TIME_PerCal;		//ÿ��ִ����ʱʱ�䣨���Կ�����ʱ�䣩�������趨
	uint16_t count;			//��ʼ��Ϊ0
	float Ramp_Buf;			//��ʼ��Ϊ0
	float Output;			//��ʼ��Ϊ0
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

