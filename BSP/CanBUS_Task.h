#ifndef __CanBUS_Task_H
#define __CanBUS_Task_H
#include "can.h"
#include "main.h"

#define CAN_BUS_LEFT_FRIMOTO_FEEDBACK_MSG_ID  	  0x203
#define CAN_BUS_RIGHT_FRIMOTO_FEEDBACK_MSG_ID 	  0x204
#define CAN_BUS1_TurnTable_FEDID 0x202   //���̵��22006 ID 0x207  ��ʶ��0x1FF





#define RATE_BUF_SIZE 6

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


void EncoderProcess(volatile EnCoder *v);
void CANReceiveMsgProcess(CAN_RxHeaderTypeDef *Rxmsg);
void set_frimoto_current(CAN_HandleTypeDef *CANx, int16_t left_output, int16_t right_output);

extern volatile EnCoder TurnTableEN;
extern Measure LeftFrimotoMeasure;
extern Measure RightFrimotoMeasure;
extern Measure TurnTable_Measure;




#endif

