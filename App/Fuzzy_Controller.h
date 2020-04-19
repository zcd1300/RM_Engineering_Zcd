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
	float SetPoint;			//�趨ֵ
	
	float Kp;				
	float Ki;
	float Kd;
	
	float deadband;			//����
	
	float LastERROR;		//�ϴ�ƫ��
	float PreERROR;			//���ϴ�ƫ��
	
	float output;			//���
	float result;			//���������

	float max_out;			//�������
	float min_out;			//�������
	
	float max_Kp;			//Kp��Χ
	float min_Kp;
	float Q_Kp;				//Kp������
	
	float max_Ki;
	float min_Ki;
	float Q_Ki;
	
	float max_Kd;
	float min_Kd;
	float Q_Kd;
	
}Fuzzy;



#endif

