#ifndef __PROTOCOL_H
#define __PROTOCOL_H
#include "main.h"
#include "stm32f4xx.h"
#include "cmsis_os.h"
#include "BSP_Data.h"

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

typedef struct 
{
	uint8_t SendPID1;
	uint8_t SendPID2;
	uint8_t SendPID5;
	uint8_t SendJudge;
} flag;


extern flag PID_Flag;
extern uint8_t TxBuffer[256];
extern uint8_t TxCounter;
extern uint8_t count;
extern  uint8_t Protocol_Finish_Flag;
extern CIRCLE_BUFF_t Que_Protocol;


//串口数据处理
void Dataframe_Process(void);
void SendDataLog(char *log, uint8_t size);
void SendLog_Circle(uint8_t hz, char *log);
void Protocol(void);

void Protocol_ThreadCreate(osPriority taskPriority);

#endif
