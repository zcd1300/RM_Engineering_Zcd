/**
 * @brief  Fuzzy controller of PID parameters
 * @param	PID controller return value
 * @retval  Processed PID controller parameters
 * @author ZCD
 * @Time 2020 1 5
*/

#include "Fuzzy_Controller.h"



/**
 * @brief  Linear quantization operation function; 
		   Range {-6~6}
 * @param	Fuzzy *vPID,float pv,float *qValue
 * @retval  void
 * @Time 2020 4 19
*/
static void Linearization(Fuzzy *vPID,float fdb,float *qValue)
{
	float thisError;
	float deltaError;
	
	thisError = vPID->SetPoint-fdb;			//本次误差
	deltaError = thisError-vPID->LastERROR;	//上次误差
	qValue[0] = 6.0*thisError/(vPID->max_out-vPID->min_out);  	//F(E)计算
	qValue[1] = 3.0*deltaError/(vPID->max_out-vPID->min_out);	//F(Ec)计算 
	
}

/**
 * @brief  Membership calculation function
 * @param	float *ms(membership),float qv(qValue),int *Index
 * @retval  void
 * @Time 2020 4 19
*/

static void Membership_Calc(float *ms,float qv,int *Index)
{
	if((qv>=NB)&&(qv<NM))
	{
		Index[0]=0;
		Index[1]=1;
		ms[0]= -0.5*qv-2.0;
		ms[1]= 	0.5*qv+3.0;
	}
	else if((qv>=NM)&&(qv<NS))
	{
		Index[0]=1;
		Index[1]=2;
		ms[0]= -0.5*qv-1.0;
		ms[1]= 	0.5*qv +2.0;
	}
	else if((qv>=NS)&&(qv<ZO))
	{
		Index[0]=2;
		Index[1]=3;
		ms[0]= -0.5*qv;
		ms[1]=	0.5*qv+1.0;
	}
	else if((qv>=ZO)&&(qv<PS))
	{
		Index[0]=3;
		Index[1]=4;
		ms[0]= -0.5*qv+1.0;
	    ms[1]=	0.5*qv;
	}
	else if((qv>=PS)&&(qv<PM))
	{
		Index[0]=4;
		Index[1]=5;
		ms[0]= -0.5*qv+2.0;
		ms[1]=	0.5*qv-1.0;
	}
	else if((qv>=PM)&&(qv<=PB))
	{
		Index[0]=5;
		Index[1]=6;
		ms[0]= -0.5*qv+3.0;
		ms[1]=	0.5*qv-2.0;
	}
}

/**
 * @brief  Decode fuzzy function
 * @param	float *ms(membership),float qv(qValue),int *Index
 * @retval  void
 * @Time 2020 4 19
*/
static void FuzzyDecode()



