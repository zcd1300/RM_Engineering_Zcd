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
	
	thisError = vPID->SetPoint-fdb;			//�������
	deltaError = thisError-vPID->LastERROR;	//�ϴ����
	qValue[0] = 6.0*thisError/(vPID->max_out-vPID->min_out);  	//F(E)����
	qValue[1] = 3.0*deltaError/(vPID->max_out-vPID->min_out);	//F(Ec)���� 
	
}

/**
 * @brief  Membership calculation function
 * @param	float *ms,float qv,int *Index
 * @retval  void
 * @Time 2020 4 19
*/

static void Membership_Calc(float *ms,float qv,int *Index)
{
	if((qv>=-NB)&&(qv<-NM))
	{
	
		
	}
	
	
	
	
}


