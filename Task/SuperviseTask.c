#include "main.h"
#include "SuperviseTask.h"
#include "cmsis_os.h"
#include "task.h"
#include "Driver_Beep.h"
uint32_t *GetLostCounter(uint32_t index);
uint16_t GetLostCounter_max(uint32_t index);
uint32_t lost_err = 0;     //每一位代表一个错误
uint32_t lost_counter[40] = {0};			//看门狗计数

uint32_t test_count[7] = {0};
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
                    .LOST_ERROR_CAN_TX_MAX          = 1000,
                    .LOST_ERROR_CAN_RX_MAX          = 1000,
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

//获取计数的地址
uint32_t *GetLostCounter(uint32_t index)
{
	return (uint32_t *)&lost_counter[index];
}
//获取计数上限
uint16_t GetLostCounter_max(uint32_t index)
{
	uint16_t *pmax =(uint16_t *)(&lost_counter_max + index*8*sizeof(uint16_t));
	return *pmax;
}
//是否出错
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
不知道2019年的工程会不会用到，所以先注释掉*/

/**
  * @brief  用于监控线程的占用情况
  * @param  obj   Mode(THREAD_IN放在线程开头 THREAD_OUT放在结尾,即可)
  * @retval None
  */
void ThreadMonitor(ThreadMonitor_t* obj,uint8_t Mode)
{
	uint32_t currentTime = xTaskGetTickCount();//18年步兵用的是自己写的Get_Time_Micros()，我暂时还不知道区别
	if(Mode == THREAD_IN)
	{
		obj->deltaTime = xTaskGetTickCount() - obj->previousTime;	//距离距离上次运行的时间
		obj->previousTime = currentTime;						//线程上次进入的时间
	}
	else if(Mode == THREAD_OUT)
	{
		obj->excutionTime = currentTime - obj->previousTime;	//线程的执行时间
	}	
}

uint32_t lost_err;
uint32_t lost_counter[40];


uint16_t DBUSFrameRate = 0;
uint16_t DBUSFrameCounter = 0;

uint16_t ChassisFrameRate[4] = {0};
uint16_t ChassisFrameCounter[4] = {0};

uint16_t LiftChainFrameRate[4] = {0};
uint16_t LiftChainFrameCounter[4] = {0};

uint16_t GuideMotorFrameRate[2] = {0};
uint16_t GuideMotorFrameCounter[2] = {0};

uint16_t MoveArmMotorFrameRate = 0;
uint16_t MoveArmMotorFrameCounter = 0;

uint16_t FlipArmMotorFrameRate = 0;
uint16_t FlipArmMotorFrameCounter = 0;


uint16_t CAN_Send_FrameRate = 0;
uint16_t CAN_Send_FrameCounter = 0;
uint16_t CAN_RS_FrameRate = 0;
uint16_t CAN_RS_FrameCounter = 0;

uint16_t CAN_Res_FrameRate = 0;
uint16_t CAN_Res_FrameCounter = 0;

uint16_t UART2FrameRate = 0;
uint16_t UART2FrameCounter = 0;

uint16_t IMUFrameRate = 0;
uint16_t IMUFrameCounter = 0;

portBASE_TYPE Control_Stack = 1;
portBASE_TYPE DataStream_Stack = 1;
portBASE_TYPE SuperviseTask_Stack = 1;

extern osThreadId Led_ToggleHandle;
extern osThreadId Data_StreamHandle;
extern osThreadId Task_ControlHandle;
extern osThreadId Status_UpdateHandle;
extern osThreadId Supervise_TaskHandle;

void Task_Monitor(void)
{

		
		FrameGet();
		ErrorFlagSet();
		BeepForError();
		
		
		
//-----------------------------------操作系统堆栈监测------------------------------------
		DataStream_Stack=uxTaskGetStackHighWaterMark(Data_StreamHandle);
		Control_Stack=uxTaskGetStackHighWaterMark(Task_ControlHandle);
		SuperviseTask_Stack=uxTaskGetStackHighWaterMark(Supervise_TaskHandle);
}		



void FrameGet(void)//获得帧率信息
{
			//DBUS帧率统计
			DBUSFrameRate = DBUSFrameCounter*2;	
			DBUSFrameCounter = 0;
			
			//底盘电机帧率统计
			ChassisFrameRate[0] = ChassisFrameCounter[0]*2;
			ChassisFrameCounter[0] = 0;
			ChassisFrameRate[1] = ChassisFrameCounter[1]*2;
			ChassisFrameCounter[1] = 0;
			ChassisFrameRate[2] = ChassisFrameCounter[2]*2;
			ChassisFrameCounter[2] = 0;
			ChassisFrameRate[3] = ChassisFrameCounter[3]*2;
			ChassisFrameCounter[3] = 0;
			
      //底盘抬升电机帧率统计
			LiftChainFrameRate[0] = LiftChainFrameCounter[0]*2;
		  LiftChainFrameCounter[0] = 0;
			LiftChainFrameRate[1] = LiftChainFrameCounter[1]*2;
		  LiftChainFrameCounter[1] = 0;
			
      //机械臂抬升电机帧率统计
			LiftChainFrameRate[2] = LiftChainFrameCounter[2]*2;
		  LiftChainFrameCounter[2] = 0;
			LiftChainFrameRate[3] = LiftChainFrameCounter[3]*2;
		  LiftChainFrameCounter[3] = 0;
			
			
			//导轮电机帧率统计
			GuideMotorFrameRate[0] = GuideMotorFrameCounter[0]*2;
			GuideMotorFrameCounter[0] = 0;
			
			GuideMotorFrameRate[1] = GuideMotorFrameCounter[1]*2;
			GuideMotorFrameCounter[1] = 0;
			
			
			//平移电机帧率统计
			MoveArmMotorFrameRate = MoveArmMotorFrameCounter*2;
			MoveArmMotorFrameCounter = 0;
			
			//翻转电机帧率统计
			FlipArmMotorFrameRate = FlipArmMotorFrameCounter*2;
			FlipArmMotorFrameCounter = 0;
				
      //CAN收发帧率统计
			CAN_Send_FrameRate = CAN_Send_FrameCounter*2;
			CAN_Send_FrameCounter = 0;

			IMUFrameRate = IMUFrameCounter*2;
			IMUFrameCounter = 0;

			CAN_Res_FrameRate = CAN_Res_FrameCounter*2;
			CAN_Res_FrameCounter = 0;
		
			//串口帧率检测
			UART2FrameRate = UART2FrameCounter*2;
			UART2FrameCounter = 0;
}


void ErrorFlagSet(void)//设置错误位
{
	//遥控器帧率过低
		if(DBUSFrameRate < 10)
		{
			Set_Error_Flag(LOST_ERROR_RC);
			error_count[0]++;
		}
		else
		{
			Reset_Error_Flag(LOST_ERROR_RC);
		}
		//底盘电机帧率过低
		if(ChassisFrameRate[0] < 200)
		{
			Set_Error_Flag(LOST_ERROR_MOTOR_CHASSIS1);
			error_count[1]++;
		}
		else
		{
			Reset_Error_Flag(LOST_ERROR_MOTOR_CHASSIS1);
		}
		
		if(ChassisFrameRate[1] < 200)
		{
			Set_Error_Flag(LOST_ERROR_MOTOR_CHASSIS2);
			error_count[2]++;
		}
		else
		{
			Reset_Error_Flag(LOST_ERROR_MOTOR_CHASSIS2);
		}
		
		if(ChassisFrameRate[2] < 200)
		{
			Set_Error_Flag(LOST_ERROR_MOTOR_CHASSIS3);
			error_count[3]++;
		}
		else
		{
			Reset_Error_Flag(LOST_ERROR_MOTOR_CHASSIS3);
		}
		
		if(ChassisFrameRate[3] < 200)
		{
			Set_Error_Flag(LOST_ERROR_MOTOR_CHASSIS4);
			error_count[4]++;
		}
		else
		{
			Reset_Error_Flag(LOST_ERROR_MOTOR_CHASSIS4);
		}
		
    //伸缩腿抬升电机帧率过低
		if(LiftChainFrameRate[0] < 200)
		{
			Set_Error_Flag(LOST_ERROR_MOTOR_LIFT1);
			error_count[5]++;
		}
		else
		{
			Reset_Error_Flag(LOST_ERROR_MOTOR_LIFT1);
		}
		


		if(LiftChainFrameRate[1] < 200)
		{
			Set_Error_Flag(LOST_ERROR_MOTOR_LIFT2);
			error_count[6]++;
		}
		else
		{
			Reset_Error_Flag(LOST_ERROR_MOTOR_LIFT2);
		}
		
    //机械臂抬升电机帧率过低
		if(LiftChainFrameRate[2] < 200)
		{
			Set_Error_Flag(LOST_ERROR_MOTOR_LIFT3);
			error_count[7]++;
		}
		else
		{
			Reset_Error_Flag(LOST_ERROR_MOTOR_LIFT3);
		}
		
		if(LiftChainFrameRate[3] < 200)
		{
			Set_Error_Flag(LOST_ERROR_MOTOR_LIFT4);
			error_count[8]++;
		}
		else
		{
			Reset_Error_Flag(LOST_ERROR_MOTOR_LIFT4);
		}
		
		//导轮电机帧率过低
		if(GuideMotorFrameRate[0] < 200)
		{
			Set_Error_Flag(LOST_ERROR_MOTOR_GUIDE1);
			error_count[9]++;
		}
		else
		{
			Reset_Error_Flag(LOST_ERROR_MOTOR_GUIDE1);
		}
		
		if(GuideMotorFrameRate[1] < 200)
		{
			Set_Error_Flag(LOST_ERROR_MOTOR_GUIDE2);
			error_count[10]++;
		}
		else
		{
			Reset_Error_Flag(LOST_ERROR_MOTOR_GUIDE2);
		}

		//机械臂平移电机帧率过低
		if(MoveArmMotorFrameRate < 200)
		{
			Set_Error_Flag(LOFT_ERROR_MOTOR_MOVE);
			error_count[11]++;
		}
		else
		{
			Reset_Error_Flag(LOFT_ERROR_MOTOR_MOVE);
		}
		
		//翻转电机帧率过低
		if(FlipArmMotorFrameRate < 200)
		{
			Set_Error_Flag(LOST_ERROR_MOTOR_FLIP);
			error_count[12]++;
		}
		else
		{
			Reset_Error_Flag(LOST_ERROR_MOTOR_FLIP);
		}
		
        //CAN收发帧率过低
    if(CAN_Send_FrameRate < 400)
		{
			Set_Error_Flag(LOST_ERROR_CAN_TX);
			error_count[13]++;
		}
		else
		{
			Reset_Error_Flag(LOST_ERROR_CAN_TX);
		}

    if(CAN_Res_FrameRate < 400)
		{
			Set_Error_Flag(LOST_ERROR_CAN_RX);
			error_count[14]++;
		}
		else
		{
			Reset_Error_Flag(LOST_ERROR_CAN_RX);
		}
		
		if(IMUFrameRate < 200)
		{
			Set_Error_Flag(LOST_ERROR_IMU);
			error_count[15]++;
		}
		else
		{
			Reset_Error_Flag(LOST_ERROR_IMU);
		}
}



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


void Supervise_Task(void const * argument)//帧率检测,错误判断
{

  
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

  /* Infinite loop */
  for(;;)
  {

			
			Task_Monitor();
			
			vTaskDelayUntil(&xLastWakeTime,500/portTICK_RATE_MS);//此时处于阻塞态

  }

}


