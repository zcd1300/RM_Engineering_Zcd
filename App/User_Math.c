/**
 * @brief  Some mathematical processing functions
 * @param	Mathematical variables to process
 * @retval  Processing the completed mathematical results
 * @author ZCD
 * @Time 2020 1 11
*/

#include "User_Math.h"
#include "math.h"
#include "stdlib.h"
#include "stm32f4xx_hal.h"

/**
 * @brief  Numerical range control.Take the minimum value when it is less than the minimum, take the maximum value when it is greater than the maximum
 * @param  Floating point type
 * @author ZCD
 * @Time 2020 1 11
*/
float NumRangeLimit(float Num_Input, float Num_Min, float Num_Max)
{
	if(Num_Input<Num_Min)
	{
		return Num_Min;
	}
	else if(Num_Input>Num_Max)
	{
		return Num_Max;
	}
	else
	{
		return Num_Input;
	}

}

/**
 * @brief  Numerical range control.Absolute value
 * @param  Integer type
 * @author ZCD
 * @Time   2020 1 11
*/
int NumRangeLimit_ABS(int Num_Input,int Num_Min, int Num_Max)
{
	int Num_temp=0;
	Num_temp = abs(Num_Input);
	if(Num_temp<Num_Min)
	{
		return Num_Min;
	}
	else if(Num_temp>Num_Max)
	{
		return Num_Max;
	}
	else
	{
		return Num_temp;
	}

}

//±ä»¯Ç÷ÊÆ¼ì²â
uint8_t Value_Change_Trend_ABS (int16_t Num ,int16_t Num_Last)
{

	Num=abs(Num);
	Num_Last=abs(Num_Last);
	if(Num-Num_Last>=5)
	{
		return 1;
	}
	else 
	{
		return 2;
	}

}

/**
 * @brief  Increasing slope function
 * @param  End value;  End value slope;
 * @author ZCD
 * @Time   2020 1 15
*/
float Ramp_Increase(Ramp_Init_e* Ramp_Temp)
{
	Ramp_Temp->count++;
	HAL_Delay(Ramp_Temp->TIME_PerCal);
	Ramp_Temp->Ramp_Buf=(Ramp_Temp->End_Value/Ramp_Temp->count_SUM)*Ramp_Temp->count;
	Ramp_Temp->Output=Ramp_Temp->Ramp_Buf;
	return Ramp_Temp->Output;
}
/**
 * @brief  Decreasing slope function
 * @param  End value;  End value slope;
 * @author ZCD
 * @Time   2020 1 15
*/
float Ramp_Decrease(Ramp_Init_e* Ramp_Temp)
{
	Ramp_Temp->count++;
	HAL_Delay(Ramp_Temp->TIME_PerCal);
	Ramp_Temp->Ramp_Buf=Ramp_Temp->End_Value - (Ramp_Temp->End_Value/Ramp_Temp->count_SUM)*Ramp_Temp->count;
	Ramp_Temp->Output=Ramp_Temp->Ramp_Buf;
	return Ramp_Temp->Output;
}




