#include "main.h"
#include "SuperviseTask.h"
#include "cmsis_os.h"
#include "task.h"

uint32_t lost_err = 0;     //每一位代表一个错误

uint16_t DBUSFrameRate = 0;
uint16_t DBUSFrameCounter = 0;

uint16_t CAN_Send_FrameRate[2] = {0};
uint16_t CAN_Send_FrameCounter[2] = {0};

uint16_t CAN_Res_FrameRate[2] = {0};
uint16_t CAN_Res_FrameCounter[2] = {0};

uint16_t UART2FrameRate = 0;
uint16_t UART2FrameCounter = 0;

uint16_t IMUFrameRate = 0;
uint16_t IMUFrameCounter = 0;

uint16_t YAWFrameRate = 0;
uint16_t YAWFrameCounter = 0;
uint16_t PITCHFrameRate = 0;
uint16_t PITCHFrameCounter = 0;

uint16_t FrictionFrameRate_Left = 0;
uint16_t FrictionFrameCounter_Left = 0;
uint16_t FrictionFrameRate_Right = 0;
uint16_t FrictionFrameCounter_Right = 0;

uint16_t BulletPlateFrameRate = 0;
uint16_t BulletPlateFrameCounter = 0;

uint16_t MiniPCFrameRate_USBRx = 0;
uint16_t MiniPCFrameCounter_USBRx = 0;
uint16_t MiniPCFrameRate_USBTx = 0;
uint16_t MiniPCFrameCounter_USBTx = 0;

osThreadId SuperviseHandle;
//uint32_t lost_counter[40] = {0};		//看门狗计数(这个好像没用到,先注释掉了)
//uint32_t test_count[7] = {0};

uint32_t error_count[16] = {0};
Lost_Counter_Max lost_counter_max = {	
										.LOST_ERROR_RC_MAX 			= 20,
										.LOST_ERROR_MOTOR1_MAX 		= 500,
										.LOST_ERROR_MOTOR2_MAX 		= 500,
										.LOST_ERROR_MOTOR3_MAX 		= 500,
										.LOST_ERROR_MOTOR4_MAX  	= 500,
										.LOST_ERROR_MOTOR_LBELT_MAX = 500,
										.LOST_ERROR_MOTOR_RBELT_MAX = 500,
										.LOST_ERROR_MOTOR_ARM_MAX	= 500,
										.LOST_ERROR_CAN_TX_MAX      = 1000,
										.LOST_ERROR_CAN_RX_MAX      = 1000,
                                    };

//标志错误位
void Set_Error_Flag(uint32_t index)
{
	lost_err |= (uint32_t)(1<<index);
}

//重置错误位
void Reset_Error_Flag(uint32_t index)
{
	lost_err &= (uint32_t)~(1<<index);
}                                    

////获取计数的地址
//uint32_t *GetLostCounter(uint32_t index)
//{
//	return (uint32_t *)&lost_counter[index];
//}
//获取计数上限
uint16_t GetLostCounter_max(uint32_t index)
{
	uint16_t *pmax =(uint16_t *)(&lost_counter_max + index*8*sizeof(uint16_t));
	return *pmax;
}
//检查某一位是否出错
uint8_t Is_Lost_Error_Set(uint32_t index)
{
	if((lost_err &(1 << index)) != 0)return 1;
	else							return 0;
}

/*
uint8_t Is_Serious_Error()
{
	#if (XDRM_CONVERTER == 0)
	return Is_Lost_Error_Set(LOST_ERROR_IMU);
	
	#else
	return Is_Lost_Error_Set(LOST_ERROR_XDRMBLACK);

	#endif
}
不知道2020年的工程会不会用到，所以先注释掉*/

/**
  * @brief  用于监控线程的占用情况
  * @param  obj   Mode(THREAD_IN放在线程开头 THREAD_OUT放在结尾,即可)
  * @retval None
  */
void ThreadMonitor(ThreadMonitor_t* obj,uint8_t Mode)
{
	uint32_t currentTime = xTaskGetTickCount();			//步兵老版用的是自己写的Get_Time_Micros()，直接读取TIM2->CNT
	if(Mode == THREAD_IN)
	{
		obj->deltaTime = xTaskGetTickCount() - obj->previousTime;	//距离距离上次运行的时间
		obj->previousTime = currentTime;							//线程上次进入的时间
	}
	else if(Mode == THREAD_OUT)
	{
		obj->excutionTime = currentTime - obj->previousTime;		//线程的执行时间
	}	
}

//portBASE_TYPE Control_Stack = 1;
//portBASE_TYPE DataStream_Stack = 1;
portBASE_TYPE SuperviseTask_Stack = 1;

void Task_Monitor(void)
{		
	FrameGet();
	ErrorFlagSet();
		
//-----------------------------------操作系统堆栈监测------------------------------------
//	DataStream_Stack=uxTaskGetStackHighWaterMark(Data_StreamHandle);
//	Control_Stack=uxTaskGetStackHighWaterMark(Task_ControlHandle);
	SuperviseTask_Stack = uxTaskGetStackHighWaterMark(SuperviseHandle);
//	uxTaskGetStackHighWaterMark函数要在CubeMX中勾选Enable,这里我手动使能函数。
}		

void FrameGet(void)//获得帧率信息
{
	//DBUS帧率统计(√)
	DBUSFrameRate = DBUSFrameCounter*2;	
	DBUSFrameCounter = 0;
	
	//CAN收发帧率统计(√)
	CAN_Send_FrameRate[0] = CAN_Send_FrameCounter[0]*2;
	CAN_Send_FrameCounter[0] = 0;
	CAN_Send_FrameRate[1] = CAN_Send_FrameCounter[1]*2;
	CAN_Send_FrameCounter[1] =0;
	
	CAN_Res_FrameRate[0] = CAN_Res_FrameCounter[0]*2;
	CAN_Res_FrameCounter[0] = 0;
	CAN_Res_FrameRate[1] = CAN_Res_FrameCounter[1]*2;
	CAN_Res_FrameCounter[1] = 0;
	
	//陀螺仪帧率
	IMUFrameRate = IMUFrameCounter*2;
	IMUFrameCounter = 0;

	//串口2帧率检测(√),现在还没用到,先留着,以后可能会用
	UART2FrameRate = UART2FrameCounter*2;
	UART2FrameCounter = 0;
	
	//云台YAW、PITCH(√)
	YAWFrameRate = YAWFrameCounter*2;
	YAWFrameCounter = 0;	
	PITCHFrameRate = PITCHFrameCounter*2;
	PITCHFrameCounter = 0;
	//miniPC(√)
	MiniPCFrameRate_USBRx = MiniPCFrameCounter_USBRx*2;
	MiniPCFrameCounter_USBRx = 0;	
	MiniPCFrameRate_USBTx = MiniPCFrameCounter_USBTx*2;
	MiniPCFrameCounter_USBTx = 0;
	//摩擦轮(√)
	FrictionFrameRate_Left = FrictionFrameCounter_Left*2;
	FrictionFrameCounter_Left = 0;
	FrictionFrameRate_Right = FrictionFrameCounter_Right*2;
	FrictionFrameCounter_Right = 0;
	//拨盘(√)
	BulletPlateFrameRate = BulletPlateFrameCounter*2;
	BulletPlateFrameCounter = 0;	
}

void ErrorFlagSet(void)//设置错误位
{
	//遥控器帧率过低(√)
	if(DBUSFrameRate < 10)
	{
		Set_Error_Flag(LOST_ERROR_RC);
		error_count[0]++;
	}
	else
	{
		Reset_Error_Flag(LOST_ERROR_RC);
	}
       //CAN收发帧率过低(√)
	if(CAN_Send_FrameRate[0] < 100)
	{
		Set_Error_Flag(LOST_ERROR_CAN_TX1);
		error_count[11]++;
	}
	else if(CAN_Send_FrameRate[1] < 100)
	{
		Set_Error_Flag(LOST_ERROR_CAN_TX2);
		error_count[12]++;
	}
	else
	{
		Reset_Error_Flag(LOST_ERROR_CAN_TX1);
		Reset_Error_Flag(LOST_ERROR_CAN_TX2);
	}

	if(CAN_Res_FrameRate[0] < 50)
	{
		Set_Error_Flag(LOST_ERROR_CAN_RX1);
		error_count[13]++;
	}
	else if(CAN_Res_FrameRate[1] < 50)
	{
		Set_Error_Flag(LOST_ERROR_CAN_RX2);
		error_count[14]++;	
	}
	else
	{
		Reset_Error_Flag(LOST_ERROR_CAN_RX1);
		Reset_Error_Flag(LOST_ERROR_CAN_RX2);
	}
	
	//陀螺仪检测
	if(IMUFrameRate < 200)
	{
		Set_Error_Flag(LOST_ERROR_IMU);
		error_count[15]++;
	}
	else
	{
		Reset_Error_Flag(LOST_ERROR_IMU);
	}
	//云台(√)
	//这个云台最低频率有点低？实际要测试下
	if(YAWFrameRate < 200)
	{
		Set_Error_Flag(LOST_ERROR_MOTOR_YAW);
		error_count[1]++;
	}
	else if(PITCHFrameRate < 200)
	{
		Set_Error_Flag(LOST_ERROR_MOTOR_PITCH);
		error_count[2]++;
	}
	else
	{
		Reset_Error_Flag(LOST_ERROR_MOTOR_YAW);
		Reset_Error_Flag(LOST_ERROR_MOTOR_PITCH);
	}
	//MiniPC(√)
	//usb是第一次使用，具体频率还要测试。先用200
	if(MiniPCFrameRate_USBRx < 200)
	{
		Set_Error_Flag(LOST_ERROR_MiniPC_USBRx);
		error_count[6]++;
	}
	else if(MiniPCFrameRate_USBTx < 200)
	{
		Set_Error_Flag(LOST_ERROR_MiniPC_USBTx);
		error_count[7]++;
	}
	else
	{
		Reset_Error_Flag(LOST_ERROR_MiniPC_USBRx);
		Reset_Error_Flag(LOST_ERROR_MiniPC_USBTx);
	}	
	//摩擦轮(√)
	if(FrictionFrameRate_Left < 200)
	{
		Set_Error_Flag(LOST_ERROR_MOTOR_Friction_Left);
		error_count[3]++;
	}
	else if(FrictionFrameRate_Right <200)
	{
		Set_Error_Flag(LOST_ERROR_MOTOR_Friction_Right);
		error_count[4]++;
	}
	else
	{
		Reset_Error_Flag(LOST_ERROR_MOTOR_Friction_Left);
		Reset_Error_Flag(LOST_ERROR_MOTOR_Friction_Right);
	}
	//拨盘(√)
	if(BulletPlateFrameRate < 200)
	{
		Set_Error_Flag(LOST_ERROR_MOTOR_BulltePlate);
		error_count[5]++;
	}
	else
	{
		Reset_Error_Flag(LOST_ERROR_MOTOR_BulltePlate);
	}
}

/*
void BeepForError(void)
{
	if(lost_err & 0x0001 )
	{
		Beep(1,10);//遥控器掉线
		test_count[0]++;
	}
	else if(lost_err & 0x001e)
	{
		Beep(2,10);//底盘电机掉线
		test_count[1]++;
	}
	else if(lost_err & 0x01e0)
	{
		Beep(3,10);//抬升电机掉线
		test_count[2]++;
	}
	else if(lost_err & 0x0600)
	{
		Beep(4,10);//导轮电机掉线
		test_count[3]++;
	}
//	else if(lost_err & 0x1800)
//	{
//		Beep(5,10);//机械臂电机掉线
//		test_count[4]++;
//	}
	else if(lost_err & 0x6000)
	{
		Beep(6,10);//CAN接收发送异常
		test_count[5]++;
	}
	else if(lost_err & 0x8000)
	{
		Beep(7,10);//陀螺仪异常
		test_count[6]++;
	}
	else
	{
		TIM12->CCR1 = 0;
	}
}
*/

void Supervise(void const * argument)
{
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
  /* Infinite loop */
	for(;;)
	{
		Task_Monitor();	
		osDelayUntil(&xLastWakeTime,500/portTICK_RATE_MS);//运行间隔0.5s
	}
}
osThreadId SuperviseHandle;
void SuperviseThreadCreate(osPriority taskPriority)
{
	osThreadDef(SuperviseThread,Supervise,taskPriority,0,256);
	SuperviseHandle = osThreadCreate(osThread(SuperviseThread),NULL);
}
