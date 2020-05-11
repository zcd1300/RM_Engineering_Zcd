/**
 * @brief  Processing information sent and received by CAN bus
 * @param	Decoding of returned information and sending of control information
 * @retval
 * @author ZCD
 * @Time 2019 12 7 
*/
#ifndef __CAN_BUS_Driver_
#define __CAN_BUS_Driver_

#include "can.h"

#define CAN_BUS_LeftFriction_Feefback_ID 0x203
#define CAN_BUS_RightFriction_Feedback_ID 0x204
#define CAN_BUS_BulletPlate_Feedback_ID 0x202

#define CAN_BUS_YAWMOTO_FEEDBACK_MSG_ID 0x205
#define CAN_BUS_PITCHMOTO_FEEDBACK_MSG_ID 0x206

#define RATE_BUF_SIZE 6

//*************************Definition of structure*****************************//
typedef struct{
	int32_t raw_value;   				//���������������ԭʼֵ
	int32_t last_raw_value;				//��һ�εı�����ԭʼֵ
	int32_t ecd_value;                  //��������������ı�����ֵ
	int32_t diff;						//���α�����֮��Ĳ�ֵ
	int32_t temp_count;                 //������
	uint8_t buf_count;					//�˲�����buf��
	int32_t ecd_bias;					//��ʼ������ֵ	
	int32_t ecd_raw_rate;				//ͨ������������õ����ٶ�ԭʼֵ
	int32_t rate_buf[RATE_BUF_SIZE];	
	int32_t round_cnt;					//Ȧ��
	int32_t filter_rate;				//�ٶ�
	float ecd_angle;					//�Ƕ�
}EnCoder;

typedef struct{
	int16_t	 	speed_rpm;
	int16_t  	real_current;
	uint8_t  	Temperature;
	uint16_t 	angle;
}Measure;



//*****************************Function declaration****************************//
void EncoderProcess(volatile EnCoder *v);
void get_measure(volatile Measure *mea,uint8_t * CAN_RxData_Buffer);
void CANReceiveMsgProcess(CAN_RxHeaderTypeDef *Rxmsg);
void CAN2ReceiveMsgProcess(CAN_RxHeaderTypeDef *Rxmsg);



//******************************External variables*****************************//
extern uint8_t CAN1_Tx_Buff_Std[8];
extern uint8_t CAN1_Tx_Buff_Ext[8];
extern uint8_t CAN2_Tx_Buff_Std[8];
extern uint8_t CAN2_Tx_Buff_Ext[8];

extern float YAW_Encoder_Init_offset;

extern volatile EnCoder BulletPlate_CM2006Encoder; 
extern volatile EnCoder YAW_GM6020Encoder;
extern volatile EnCoder PITCH_GM6020Encoder;

extern Measure Friction_Speed_Left_Measure;
extern Measure Friction_Speed_Right_Measure;
extern Measure BulletPlate_Measure;





//******************************************************************************//


#endif

