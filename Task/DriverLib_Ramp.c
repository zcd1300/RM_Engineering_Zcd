/* Includes ------------------------------------------------------------------*/
#include "DriverLib_Ramp.h"

//键鼠控制Key2speed中使用的函数 在这里定义 


RampGen_t LRSpeedRamp = RAMP_GEN_DAFAULT;   //mouse左右移动斜坡
RampGen_t FBSpeedRamp = RAMP_GEN_DAFAULT;   //mouse前后移动斜坡


void RampInit(RampGen_t *ramp, int32_t XSCALE)
{
}

float RampCalc(RampGen_t *ramp)
{
	ramp->count++;
	if(ramp->count < ramp->XSCALE)
	{
	 return (float) ramp->count / ramp->XSCALE;
	}
	else
	{
		return 1.0f;
	}
}

void RampSetCounter(struct RampGen_t *ramp, int32_t count)
{
	ramp->count=count;
}

void RampResetCounter(struct RampGen_t *ramp)
{
	ramp->count=0;
}

void RampSetScale(struct RampGen_t *ramp, int32_t scale)
{
	ramp->XSCALE=scale;
}

uint8_t RampIsOverflow(struct RampGen_t *ramp)
{
	if(ramp->count < ramp->XSCALE) return 0;
	else return 1;
}

