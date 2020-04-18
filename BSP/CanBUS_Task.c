/*----------------------
2019年10月1日
ZCD

------------------------*/

#include "main.h"
#include "can.h"
#include "CanBUS_Task.h" 
#include "FirMotor.h"

static uint32_t can_count = 0;

volatile EnCoder TurnTableEN ={0,0,0,0,0,0,0,0,0}; 
Measure TurnTable_Measure = {0, 0, 0, 0};
Measure LeftFrimotoMeasure = {0, 0, 0, 0};
Measure RightFrimotoMeasure = {0, 0, 0, 0};

void EncoderProcess(volatile EnCoder *v)
{
	int i = 0;
	int32_t temp_sum = 0;
	v->last_raw_value = v->raw_value;
	v->raw_value = (RxData[0] << 8) | RxData[1];
	v->diff = v->raw_value - v->last_raw_value;
	if (v->diff < -6400) //两次编码器的反馈值差别太大，表示圈数发生了改变
	{
		v->round_cnt++;
		v->ecd_raw_rate = v->diff + 8192;
	}
	else if (v->diff > 6400)
	{
		v->round_cnt--;
		v->ecd_raw_rate = v->diff - 8192;
	}
	else
	{
		v->ecd_raw_rate = v->diff;
	}
	//计算得到连续的编码器输出值
	v->ecd_value = v->raw_value + v->round_cnt * 8192;
	//计算得到角度值，范围正负无穷大
	v->ecd_angle = (float)(v->raw_value - v->ecd_bias) * 360.0f / 8192 + v->round_cnt * 360;
	v->rate_buf[v->buf_count++] = v->ecd_raw_rate;
	if (v->buf_count == RATE_BUF_SIZE)
	{
		v->buf_count = 0;
	}	
	//计算速度平均值
	for (i = 0; i < RATE_BUF_SIZE; i++)
	{
		temp_sum += v->rate_buf[i];
	}
	v->filter_rate = (int32_t)(temp_sum / RATE_BUF_SIZE);
}
/*****
	@brief 读取C620电调的回传信息
	@param 
	@retval void
*****/
void get_measure(Measure *mea)
{
	mea->angle = (uint16_t)(RxData[0] << 8 | RxData[1]);
	mea->speed_rpm = (int16_t)(RxData[2] << 8 | RxData[3]);
	mea->real_current = (int16_t)((RxData[4] << 8) | (RxData[5]));
	mea->Temperature = RxData[6];
}
void Init_Can_Count(void)
{
	can_count = 0;
}
void CANReceiveMsgProcess(CAN_RxHeaderTypeDef *Rxmsg)
{
	//can_count++;	
	switch(Rxmsg->StdId)
	{
		case CAN_BUS_LEFT_FRIMOTO_FEEDBACK_MSG_ID:
		{
			get_measure(&LeftFrimotoMeasure);
		}
		break;

		case CAN_BUS_RIGHT_FRIMOTO_FEEDBACK_MSG_ID:
		{
			get_measure(&RightFrimotoMeasure);
		}
		break;	
		case CAN_BUS1_TurnTable_FEDID:
		{
			get_measure(&TurnTable_Measure);
			EncoderProcess(&TurnTableEN);
		}
		break;
	}


}
void set_frimoto_current(CAN_HandleTypeDef *CANx, int16_t left_output, int16_t right_output)
{

//	HAL_StatusTypeDef status;
	TxMsg.StdId = 0x200;
	TxMsg.IDE = CAN_ID_STD;
	TxMsg.RTR = CAN_RTR_DATA;
	TxMsg.DLC = 0x08;

	TxDataBuf[0] = 0x00;
	TxDataBuf[1] = 0x00;//201
	TxDataBuf[2] = (int16_t)(ShootMotorSpeedPID.output)>>8;
	TxDataBuf[3] = (unsigned char)(ShootMotorSpeedPID.output);		//202
	TxDataBuf[4] = (int16_t)(left_output) >> 8;
	TxDataBuf[5] = (unsigned char)(left_output);//203
	TxDataBuf[6] = (int16_t)(right_output) >> 8;
	TxDataBuf[7] = (unsigned char)(right_output);//204
	CAN_Send(TxDataBuf);
	//CAN_bufferPush(&Que_CAN1_Tx, *hcan1.pTxMsg);
}


