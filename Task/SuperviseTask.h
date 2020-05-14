#ifndef __SUPERVISETASK_H
#define __SUPERVISETASK_H

#include "stdint.h"
#include "cmsis_os.h"

#define LOST_ERROR_RC               		0u
#define LOST_ERROR_MOTOR_YAW		    	1u				
#define LOST_ERROR_MOTOR_PITCH			    2u			
#define LOST_ERROR_MOTOR_Friction_Left	    3u			
#define LOST_ERROR_MOTOR_Friction_Right		4u
#define LOST_ERROR_MOTOR_BulltePlate     	5u
#define LOST_ERROR_MiniPC_USBRx   			6u
#define LOST_ERROR_MiniPC_USBTx     		7u
#define LOST_ERROR_1				8u		//留空
#define LOST_ERROR_2				9u      //留空
#define LOST_ERROR_3				10u     //留空
#define LOST_ERROR_CAN_TX1					11u    
#define LOST_ERROR_CAN_TX2					12u	
#define LOST_ERROR_CAN_RX1         			13u
#define LOST_ERROR_CAN_RX2            	  	14u
#define LOST_ERROR_IMU						15u						

typedef __packed struct  Lost_Counter_Max
{
	const uint16_t LOST_ERROR_RC_MAX;
	const uint16_t LOST_ERROR_MOTOR1_MAX;
	const uint16_t LOST_ERROR_MOTOR2_MAX;
	const uint16_t LOST_ERROR_MOTOR3_MAX;
	const uint16_t LOST_ERROR_MOTOR4_MAX;
	const uint16_t LOST_ERROR_MOTOR_LBELT_MAX;
	const uint16_t LOST_ERROR_MOTOR_RBELT_MAX;
    const uint16_t LOST_ERROR_MOTOR_ARM_MAX;
    const uint16_t LOST_ERROR_CAN_TX_MAX;
    const uint16_t LOST_ERROR_CAN_RX_MAX;
}Lost_Counter_Max;			//看门狗溢出值

#define THREAD_IN  0u
#define THREAD_OUT 1u
typedef struct 
{
	uint32_t previousTime;
	uint32_t excutionTime;
	uint32_t deltaTime;
}ThreadMonitor_t;

//#define LOST_ERROR_ALL (LOST_ERROR_RC|LOST_ERROR_IMU|LOST_ERROR_MOTOR1|LOST_ERROR_MOTOR2|LOST_ERROR_MOTOR3|LOST_ERROR_MOTOR4|LOST_ERROR_GIMBAL_YAW|LOST_ERROR_GIMBAL_YAW|LOST_ERROR_DEADLOCK|LOST_ERROR_NOCALI)

extern uint32_t lost_err;
//extern uint32_t lost_counter[40];


extern uint16_t DBUSFrameRate;
extern uint16_t DBUSFrameCounter;

extern uint16_t CAN_Send_FrameRate[2];
extern uint16_t CAN_Send_FrameCounter[2];

extern uint16_t CAN_Res_FrameRate[2];
extern uint16_t CAN_Res_FrameCounter[2];

extern uint16_t UART2FrameRate;
extern uint16_t UART2FrameCounter;

extern uint16_t IMUFrameRate ;
extern uint16_t IMUFrameCounter ;

extern uint16_t YAWFrameRate;
extern uint16_t YAWFrameCounter;
 
extern uint16_t PITCHFrameRate;
extern uint16_t PITCHFrameCounter;

extern uint16_t MiniPCFrameRate_USBRx;
extern uint16_t MiniPCFrameCounter_USBRx;

extern uint16_t MiniPCFrameRate_USBTx;
extern uint16_t MiniPCFrameCounter_USBTx;

extern uint16_t FrictionFrameRate_Left;
extern uint16_t FrictionFrameCounter_Left;
extern uint16_t FrictionFrameRate_Right;
extern uint16_t FrictionFrameCounter_Right;

extern uint16_t BulletPlateFrameRate;
extern uint16_t BulletPlateFrameCounter;
void SuperviseTask(void);

void Set_Error_Flag(uint32_t index);
void Reset_Error_Flag(uint32_t index);

uint8_t Is_Lost_Error_Set(uint32_t err_code);
uint8_t Is_Serious_Error(void);
void ThreadMonitor(ThreadMonitor_t* obj,uint8_t Mode);
//void Monitor_ThreadCreate(osPriority taskPriority);
void SuperviseThreadCreate(osPriority taskPriority);
void Supervise_Task(void const * argument);
void FrameGet(void);
void ErrorFlagSet(void);

//void BeepForError(void);
//蜂鸣器可能用不到，先注释掉了
/*
//蜂鸣器部分,作为检测之后的表现

typedef enum{

  Do1L = 0, ///261.63Hz    3822us
  Re2L,     ///293.66Hz    3405us
  Mi3L,     ///329.63Hz    3034us
  Fa4L,     ///349.23Hz    2863us
  So5L,     ///392.00Hz    2551us
  La6L,     ///440.00Hz    2272us
  Si7L,     ///493.88Hz    2052us

  Do1M,     ///523.25Hz    1911us
  Re2M,     ///587.33Hz    1703us
  Mi3M,     ///659.26Hz    1517us
  Fa4M,     ///698.46Hz    1432us
  So5M,     ///784.00Hz    1276us
  La6M,     ///880.00Hz    1136us
  Si7M,     ///987.77Hz    1012us

  Do1H,     ///1046.50H/   956us
  Re2H,     ///1174.66H/   851us
  Mi3H,     ///1318.51H/   758us
  Fa4H,     ///1396.91H/   716us
  So5H,     ///1567.98H/   638us
  La6H,     ///1760.00H/   568us
  Si7H,     ///1975.53H/   506us

  Silent,
}Sound_tone_e;
*/
/*
#ifdef NEW_BELIEF
#define BEEP_ON     (TIM12->CCR1 = 599)
#define BEEP_OFF    (TIM12->CCR1 = 0)

#define BEEP_ARR    (TIM12->ARR)
#define BEEP_CH     (TIM12->CCR1)

#else
#define BEEP_ON     (TIM3->CCR1 = 599)
#define BEEP_OFF    (TIM3->CCR1 = 0)

#define BEEP_ARR    (TIM3->ARR)
#define BEEP_CH     (TIM3->CCR1)
#endif
*/
//void Sing(Sound_tone_e tone);
//void Sing_Music(const Sound_tone_e *music, uint8_t length);
//void BeepTask(void);

#endif


