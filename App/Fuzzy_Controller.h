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

int8_t rule_Kp[7][7]=\
{\
	{PB,PB,PM,PM,PS,ZO,ZO},\
	{PB,PB,PM,PS,PS,ZO,NS},\
	{PM,PM,PM,PS,ZO,NS,NS},\
	{PM,PM,PS,ZO,NS,NM,NM},\
	{PS,PS,ZO,NS,NS,NM,NM},\
	{PS,ZO,NS,NM,NM,NM,NB},\
	{ZO,ZO,NM,NM,NM,NB,NB}\
};

int8_t rule_Ki[7][7]=\
{\
	{NB,NB,NM,NM,NS,ZO,ZO},\
	{NB,NB,NM,NS,NS,ZO,ZO},\
	{NB,NM,NS,NS,ZO,PS,PS},\
	{NM,NM,NS,ZO,PS,PM,PM},\
	{NM,NS,ZO,PS,PS,PM,PB},\
	{ZO,ZO,PS,PS,PM,PB,PB},\
	{ZO,ZO,PS,PM,PM,PB,PB}\
};

int8_t rule_Kd[7][7]=\
{\
	{PS,NS,NB,NB,NB,NM,PS},\
	{PS,NS,NB,NM,NM,NS,ZO},\
	{ZO,NS,NM,NM,NS,NS,ZO},\
	{ZO,NS,NS,NS,NS,NS,ZO},\
	{ZO,ZO,ZO,ZO,ZO,ZO,ZO},\
	{PB,NS,PS,PS,PS,PS,PB},\
	{PB,PM,PM,PM,PS,PS,PB}\
};

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

