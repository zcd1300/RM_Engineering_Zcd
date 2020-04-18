#ifndef __CanBUS_Task_H
#define __CanBUS_Task_H
#include "can.h"
#include "main.h"

#define CAN_BUS_LEFT_FRIMOTO_FEEDBACK_MSG_ID  	  0x203
#define CAN_BUS_RIGHT_FRIMOTO_FEEDBACK_MSG_ID 	  0x204
#define CAN_BUS1_TurnTable_FEDID 0x202   //拨盘电机22006 ID 0x207  标识符0x1FF





#define RATE_BUF_SIZE 6

typedef struct{
	int32_t raw_value;   				//编码器不经处理的原始值
	int32_t last_raw_value;				//上一次的编码器原始值
	int32_t ecd_value;                  //经过处理后连续的编码器值
	int32_t diff;						//两次编码器之间的差值
	int32_t temp_count;                 //计数用
	uint8_t buf_count;					//滤波更新buf用
	int32_t ecd_bias;					//初始编码器值	
	int32_t ecd_raw_rate;				//通过编码器计算得到的速度原始值
	int32_t rate_buf[RATE_BUF_SIZE];	
	int32_t round_cnt;					//圈数
	int32_t filter_rate;				//速度
	float ecd_angle;					//角度
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

