/**
 * @brief  Fuzzy controller of PID parameters
 * @param	PID controller return value
 * @retval  Processed PID controller parameters
 * @author ZCD
 * @Time 2020 1 5
*/

#include "Fuzzy_Controller.h"
#include "PID.h"

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
 * @brief  Linear_Amplification, enlarge the result of decode fuzzy to the specificed range
 * @param	float max,float min,float qValue
 * @retval  float
 * @author zcd
 * @Time 2020 4 20
*/
static float Linear_Amplification(float Kx, float max,float min,float Q_Kx,float qValue)
{
	qValue = qValue*Q_Kx;
	Kx=Kx+qValue;
	if(Kx>=max)
	{	return max; }
	else if(Kx<min)
	{	return min; }
	else
	{	return Kx;}
}

/**
 * @brief  Decode fuzzy function
 * @param	Fuzzy *vPID,float fdb,float *deltaK
 * @retval  void
 * @Time 2020 4 20
*/
static void FuzzyDecode(Fuzzy *vPID,float fdb,float *deltaK)
{
	float qValue[2]={0,0};
	int Index_E[2] ={0,0};
	float ms_E[2]={0,0};
	int Index_Ec[2]={0,0};
	float ms_Ec[2] ={0,0};
	float qValueK[3];
	
	Linearization(vPID,fdb,qValue);
	
	Membership_Calc(ms_E,qValue[0],Index_E);
	Membership_Calc(ms_Ec,qValue[1],Index_Ec);
	
	qValueK[0]=ms_E[0]*(ms_Ec[0]*rule_Kp[Index_E[0]][Index_Ec[0]]+ms_Ec[1]*rule_Kp[Index_E[0]][Index_Ec[1]])
			  +ms_E[1]*(ms_Ec[0]*rule_Kp[Index_E[1]][Index_Ec[0]]+ms_Ec[1]*rule_Kp[Index_E[1]][Index_Ec[1]]);
	
	qValueK[1]=ms_E[0]*(ms_Ec[0]*rule_Ki[Index_E[0]][Index_Ec[0]]+ms_Ec[1]*rule_Ki[Index_E[0]][Index_Ec[1]])
			  +ms_E[1]*(ms_Ec[0]*rule_Ki[Index_E[1]][Index_Ec[0]]+ms_Ec[1]*rule_Ki[Index_E[1]][Index_Ec[1]]);
	
	qValueK[2]=ms_E[0]*(ms_Ec[0]*rule_Kd[Index_E[0]][Index_Ec[0]]+ms_Ec[1]*rule_Kd[Index_E[0]][Index_Ec[1]])
			  +ms_E[1]*(ms_Ec[0]*rule_Kd[Index_E[1]][Index_Ec[0]]+ms_Ec[1]*rule_Kd[Index_E[1]][Index_Ec[1]]);
	
	deltaK[0] = Linear_Amplification(vPID->K_x[0] ,vPID->max_Kp,vPID->min_Kp,vPID->Q_Kp,qValueK[0]);
	deltaK[1] = Linear_Amplification(vPID->K_x[1] ,vPID->max_Ki,vPID->min_Ki,vPID->Q_Ki,qValueK[1]);
	deltaK[2] = Linear_Amplification(vPID->K_x[2] ,vPID->max_Kd,vPID->min_Kd,vPID->Q_Kd,qValueK[2]);
	
}

/**
 * @brief  Connect PID and Fuzzy controlor
 * @param	Fuzzy *vPID,PID_Regulator_t *tPID
 * @retval  void
 * @Time 2020 4 20
*/
static void Connect_PID_FUZZY(Fuzzy * vPID,PID_Regulator_t *tPID)
{
	
	vPID->max_out=tPID->outputMax;
	vPID->min_out=-tPID->outputMax;
	
	tPID->ref = vPID->SetPoint;
	FuzzyDecode(vPID,tPID->fdb,vPID->K_x);	
	tPID->kp = vPID->K_x[0];
	tPID->ki = vPID->K_x[1];
	tPID->kd = vPID->K_x[2];
	

}



