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
#include "PID.h"
#define NB -6
#define NM -4
#define NS -2
#define ZO 0
#define PS 2
#define PM 4
#define PB 6

static int8_t rule_Kp[7][7]=\
{\
	{PB,PB,PM,PM,PS,ZO,ZO},\
	{PB,PB,PM,PS,PS,ZO,NS},\
	{PM,PM,PM,PS,ZO,NS,NS},\
	{PM,PM,PS,ZO,NS,NM,NM},\
	{PS,PS,ZO,NS,NS,NM,NM},\
	{PS,ZO,NS,NM,NM,NM,NB},\
	{ZO,ZO,NM,NM,NM,NB,NB}\
};

static int8_t rule_Ki[7][7]=\
{\
	{NB,NB,NM,NM,NS,ZO,ZO},\
	{NB,NB,NM,NS,NS,ZO,ZO},\
	{NB,NM,NS,NS,ZO,PS,PS},\
	{NM,NM,NS,ZO,PS,PM,PM},\
	{NM,NS,ZO,PS,PS,PM,PB},\
	{ZO,ZO,PS,PS,PM,PB,PB},\
	{ZO,ZO,PS,PM,PM,PB,PB}\
};

static int8_t rule_Kd[7][7]=\
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
	float SetPoint;			//设定值
	
	float K_x[3];	
	float deadband;			//死区
	
	float LastERROR;		//上次偏差
/*
	float PreERROR;			//上上次偏差
	
	float output;			//输出
	float result;			//物理量输出
*/
	float max_out;			//输出上限
	float min_out;			//输出下限

	float max_Kp;			//Kp范围
	float min_Kp;
	float Q_Kp;				//Kp影响系数
	
	float max_Ki;
	float min_Ki;
	float Q_Ki;
	
	float max_Kd;
	float min_Kd;
	float Q_Kd;
	
}Fuzzy;

static void FuzzyDecode(Fuzzy *vPID,float fdb,float *deltaK);
void Connect_PID_FUZZY(Fuzzy * vPID,PID_Regulator_t *tPID,float Ref_Temp);

#endif

