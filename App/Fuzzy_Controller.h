/**
 * @brief  Fuzzy controller of PID parameters
 * @param	PID controller return value
 * @retval  Processed PID controller parameters
 * @author ZCD
 * @Time 2020 1 5
*/

#ifndef _Fuzzy_Controller_H
#define _Fuzzy_Controller_H


#include "stdint.h"

#define NB -6
#define NM -4
#define NS -2
#define ZO 0
#define PS 2
#define PM 4
#define PB 6

typedef struct
{
	float SetPoint;			//设定值
	
	float Kp;				
	float Ki;
	float Kd;
	
	float deadband;			//死区
	
	float LastERROR;		//上次偏差
	float PreERROR;			//上上次偏差
	
	float output;			//输出
	float result;			//物理量输出

	float max_out;			//输出上限
	float min_out;			//输出下限
	
	float max_Kp;			//Kp范围
	float min_Kp;
	float Q_Kp;				//Kp隶属度
	
	float max_Ki;
	float min_Ki;
	float Q_Ki;
	
	float max_Kd;
	float min_Kd;
	float Q_Kd;
	
}Fuzzy;



#endif

