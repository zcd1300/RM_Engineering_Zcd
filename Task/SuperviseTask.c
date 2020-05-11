#include "main.h"
#include "SuperviseTask.h"
#include "cmsis_os.h"
#include "task.h"
#include "Driver_Beep.h"
uint32_t *GetLostCounter(uint32_t index);
uint16_t GetLostCounter_max(uint32_t index);
uint32_t lost_err = 0;     //ÿһλ����һ������
uint32_t lost_counter[40] = {0};			//���Ź�����

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

//��־����λ
void Set_Error_Flag(uint32_t index)
{
	lost_err |= (uint32_t)(1<<index);
}

//���ô���λ
void Reset_Error_Flag(uint32_t index)
{
	lost_err &= (uint32_t)~(1<<index);
}                                    

//��ȡ�����ĵ�ַ
uint32_t *GetLostCounter(uint32_t index)
{
	return (uint32_t *)&lost_counter[index];
}
//��ȡ��������
uint16_t GetLostCounter_max(uint32_t index)
{
	uint16_t *pmax =(uint16_t *)(&lost_counter_max + index*8*sizeof(uint16_t));
	return *pmax;
}
//�Ƿ����
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
��֪��2019��Ĺ��̻᲻���õ���������ע�͵�*/

/**
  * @brief  ���ڼ���̵߳�ռ�����
  * @param  obj   Mode(THREAD_IN�����߳̿�ͷ THREAD_OUT���ڽ�β,����)
  * @retval None
  */
void ThreadMonitor(ThreadMonitor_t* obj,uint8_t Mode)
{
	uint32_t currentTime = xTaskGetTickCount();//18�경���õ����Լ�д��Get_Time_Micros()������ʱ����֪������
	if(Mode == THREAD_IN)
	{
		obj->deltaTime = xTaskGetTickCount() - obj->previousTime;	//��������ϴ����е�ʱ��
		obj->previousTime = currentTime;						//�߳��ϴν����ʱ��
	}
	else if(Mode == THREAD_OUT)
	{
		obj->excutionTime = currentTime - obj->previousTime;	//�̵߳�ִ��ʱ��
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
		
		
		
//-----------------------------------����ϵͳ��ջ���------------------------------------
		DataStream_Stack=uxTaskGetStackHighWaterMark(Data_StreamHandle);
		Control_Stack=uxTaskGetStackHighWaterMark(Task_ControlHandle);
		SuperviseTask_Stack=uxTaskGetStackHighWaterMark(Supervise_TaskHandle);
}		



void FrameGet(void)//���֡����Ϣ
{
			//DBUS֡��ͳ��
			DBUSFrameRate = DBUSFrameCounter*2;	
			DBUSFrameCounter = 0;
			
			//���̵��֡��ͳ��
			ChassisFrameRate[0] = ChassisFrameCounter[0]*2;
			ChassisFrameCounter[0] = 0;
			ChassisFrameRate[1] = ChassisFrameCounter[1]*2;
			ChassisFrameCounter[1] = 0;
			ChassisFrameRate[2] = ChassisFrameCounter[2]*2;
			ChassisFrameCounter[2] = 0;
			ChassisFrameRate[3] = ChassisFrameCounter[3]*2;
			ChassisFrameCounter[3] = 0;
			
      //����̧�����֡��ͳ��
			LiftChainFrameRate[0] = LiftChainFrameCounter[0]*2;
		  LiftChainFrameCounter[0] = 0;
			LiftChainFrameRate[1] = LiftChainFrameCounter[1]*2;
		  LiftChainFrameCounter[1] = 0;
			
      //��е��̧�����֡��ͳ��
			LiftChainFrameRate[2] = LiftChainFrameCounter[2]*2;
		  LiftChainFrameCounter[2] = 0;
			LiftChainFrameRate[3] = LiftChainFrameCounter[3]*2;
		  LiftChainFrameCounter[3] = 0;
			
			
			//���ֵ��֡��ͳ��
			GuideMotorFrameRate[0] = GuideMotorFrameCounter[0]*2;
			GuideMotorFrameCounter[0] = 0;
			
			GuideMotorFrameRate[1] = GuideMotorFrameCounter[1]*2;
			GuideMotorFrameCounter[1] = 0;
			
			
			//ƽ�Ƶ��֡��ͳ��
			MoveArmMotorFrameRate = MoveArmMotorFrameCounter*2;
			MoveArmMotorFrameCounter = 0;
			
			//��ת���֡��ͳ��
			FlipArmMotorFrameRate = FlipArmMotorFrameCounter*2;
			FlipArmMotorFrameCounter = 0;
				
      //CAN�շ�֡��ͳ��
			CAN_Send_FrameRate = CAN_Send_FrameCounter*2;
			CAN_Send_FrameCounter = 0;

			IMUFrameRate = IMUFrameCounter*2;
			IMUFrameCounter = 0;

			CAN_Res_FrameRate = CAN_Res_FrameCounter*2;
			CAN_Res_FrameCounter = 0;
		
			//����֡�ʼ��
			UART2FrameRate = UART2FrameCounter*2;
			UART2FrameCounter = 0;
}


void ErrorFlagSet(void)//���ô���λ
{
	//ң����֡�ʹ���
		if(DBUSFrameRate < 10)
		{
			Set_Error_Flag(LOST_ERROR_RC);
			error_count[0]++;
		}
		else
		{
			Reset_Error_Flag(LOST_ERROR_RC);
		}
		//���̵��֡�ʹ���
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
		
    //������̧�����֡�ʹ���
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
		
    //��е��̧�����֡�ʹ���
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
		
		//���ֵ��֡�ʹ���
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

		//��е��ƽ�Ƶ��֡�ʹ���
		if(MoveArmMotorFrameRate < 200)
		{
			Set_Error_Flag(LOFT_ERROR_MOTOR_MOVE);
			error_count[11]++;
		}
		else
		{
			Reset_Error_Flag(LOFT_ERROR_MOTOR_MOVE);
		}
		
		//��ת���֡�ʹ���
		if(FlipArmMotorFrameRate < 200)
		{
			Set_Error_Flag(LOST_ERROR_MOTOR_FLIP);
			error_count[12]++;
		}
		else
		{
			Reset_Error_Flag(LOST_ERROR_MOTOR_FLIP);
		}
		
        //CAN�շ�֡�ʹ���
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
		Beep(1,10);//ң��������
		test_count[0]++;
	}
	else if(lost_err & 0x001e)
	{
		Beep(2,10);//���̵������
		test_count[1]++;
	}
	else if(lost_err & 0x01e0)
	{
		Beep(3,10);//̧���������
		test_count[2]++;
	}
	else if(lost_err & 0x0600)
	{
		Beep(4,10);//���ֵ������
		test_count[3]++;
	}
//	else if(lost_err & 0x1800)
//	{
//		Beep(5,10);//��е�۵������
//		test_count[4]++;
//	}
	else if(lost_err & 0x6000)
	{
		Beep(6,10);//CAN���շ����쳣
		test_count[5]++;
	}
	else if(lost_err & 0x8000)
	{
		Beep(7,10);//�������쳣
		test_count[6]++;
	}
	else
	{
		TIM12->CCR1 = 0;
	}
}


void Supervise_Task(void const * argument)//֡�ʼ��,�����ж�
{

  
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

  /* Infinite loop */
  for(;;)
  {

			
			Task_Monitor();
			
			vTaskDelayUntil(&xLastWakeTime,500/portTICK_RATE_MS);//��ʱ��������̬

  }

}


