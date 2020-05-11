/**
 * @brief  Processing information sent and received by CAN bus
 * @param	Decoding of returned information and sending of control information
 * @retval
 * @author ZCD
 * @Time 2019 12 7 
*/

#include "cmsis_os.h"
#include "CAN_BUS_Driver.h"
#include "can.h"
#include "Motor_ConttrolTask.h"
#include "SuperviseTask.h"

uint8_t CAN1_Tx_Buff_Std[8]={0};
uint8_t CAN1_Tx_Buff_Ext[8]={0};

uint8_t CAN2_Tx_Buff_Std[8]={0};
uint8_t CAN2_Tx_Buff_Ext[8]={0};

float YAW_Encoder_Init_offset=0;			//因为装配原因，云台YAW轴在不同位置初始化可能导致初始角度值不同。Pitch轴因为不会经过角度突变点，所以没写。

static uint32_t can_count = 0;

Measure Friction_Speed_Left_Measure={0,0,0,0};
Measure Friction_Speed_Right_Measure={0,0,0,0};
Measure BulletPlate_Measure={0,0,0,0};

volatile EnCoder BulletPlate_CM2006Encoder = {0,0,0,0,0,0,0,0,0};
volatile EnCoder YAW_GM6020Encoder = {0,0,0,0,0,0,0,0,0};
volatile EnCoder PITCH_GM6020Encoder = {0,0,0,0,0,0,0,0,0};



void EncoderCorrection(void)
{
	if(YAW_GM6020Encoder.ecd_angle>=0)
	{
		YAW_Encoder_Init_offset= -360;
	}
	else
	{
		YAW_Encoder_Init_offset=0;
	}
	YAW_GM6020Encoder.ecd_angle=YAW_GM6020Encoder.ecd_angle+YAW_Encoder_Init_offset;
}

/*****
	@brief Process the encoder data and store it in the corresponding structure
	@param Because the CAN line of GM6020 can only be connected to the CAN1 bus of board A.
		   So, the data source is the buffer of CAN1.
	@retval void
*****/
void EncoderProcess(volatile EnCoder *v)
{
	int i = 0;
	int32_t temp_sum = 0;
	v->last_raw_value = v->raw_value;
	v->raw_value = (CAN1_RxData_Buffer[0] << 8) | CAN1_RxData_Buffer[1];
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
	@brief Read the return information of C620
	@param Input the structure of stored information and corresponding can buffer array
	@retval void
*****/
void get_measure(volatile Measure *mea,uint8_t * CAN_RxData_Buffer)
{
	mea->angle = (uint16_t)(CAN_RxData_Buffer[0] << 8 | CAN_RxData_Buffer[1]);
	mea->speed_rpm = (int16_t)(CAN_RxData_Buffer[2] << 8 | CAN_RxData_Buffer[3]);
	mea->real_current = (int16_t)((CAN_RxData_Buffer[4] << 8) | (CAN_RxData_Buffer[5]));
	mea->Temperature = CAN_RxData_Buffer[6];
}

void Init_Can_Count(void)
{
	can_count = 0;
	can_count++;
}

/*****
	@brief For the returned data, call the corresponding function for data processing
	@param 
	@retval void
*****/
void CANReceiveMsgProcess(CAN_RxHeaderTypeDef *Rxmsg)
{
	//can_count++;	
	switch(Rxmsg->StdId)
	{
		case CAN_BUS_YAWMOTO_FEEDBACK_MSG_ID:
		{
			YAWFrameCounter++;
			//get_measure(&YAW_GM6020Measure,CAN1_RxData_Buffer);
			EncoderProcess(&YAW_GM6020Encoder);
			EncoderCorrection();
		}
		break;
		case CAN_BUS_PITCHMOTO_FEEDBACK_MSG_ID:
		{
			PITCHFrameCounter++;			
			EncoderProcess(&PITCH_GM6020Encoder);
		}
		break;
		
		case CAN_BUS_LeftFriction_Feefback_ID:
		{
			FrictionFrameCounter_Left++;
			get_measure(&Friction_Speed_Left_Measure,CAN1_RxData_Buffer);
		}
		break;
		case CAN_BUS_RightFriction_Feedback_ID:
		{
			FrictionFrameCounter_Right++;
			get_measure(&Friction_Speed_Right_Measure,CAN1_RxData_Buffer);
		}
		break;
		case CAN_BUS_BulletPlate_Feedback_ID:
		{
			BulletPlateFrameCounter++;
			get_measure(&BulletPlate_Measure,CAN1_RxData_Buffer);
			EncoderProcess(&BulletPlate_CM2006Encoder);
		}
		break;
	}


}
void CAN2ReceiveMsgProcess(CAN_RxHeaderTypeDef *Rxmsg)
{
	switch(Rxmsg->StdId)
	{
//		case CAN_BUS_YAWMOTO_FEEDBACK_MSG_ID:
//		{
//			//get_measure(&YAW_GM6020Measure,CAN1_RxData_Buffer);
//			EncoderProcess(&YAW_GM6020Encoder);
//			EncoderCorrection();
//		}
//		break;

	}
	
}
void CAN_DRIVER_FUN(void const* argument)
{
	
	for(;;)
	{
		osDelay(1);
	
	}
}

osThreadId CAN_DriverHandle ;

void CAN_DriverThreadCreate (osPriority taskPriority)
{
	osThreadDef(CAN_DriverThread,CAN_DRIVER_FUN,taskPriority,0,256);
	CAN_DriverHandle=osThreadCreate(osThread(CAN_DriverThread),NULL);

}

