#include "main.h"
#include "SuperviseTask.h"
#include "cmsis_os.h"
#include "task.h"

uint32_t lost_err = 0;     //ÿһλ����һ������

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
//uint32_t lost_counter[40] = {0};		//���Ź�����(�������û�õ�,��ע�͵���)
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

////��ȡ�����ĵ�ַ
//uint32_t *GetLostCounter(uint32_t index)
//{
//	return (uint32_t *)&lost_counter[index];
//}
//��ȡ��������
uint16_t GetLostCounter_max(uint32_t index)
{
	uint16_t *pmax =(uint16_t *)(&lost_counter_max + index*8*sizeof(uint16_t));
	return *pmax;
}
//���ĳһλ�Ƿ����
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
��֪��2020��Ĺ��̻᲻���õ���������ע�͵�*/

/**
  * @brief  ���ڼ���̵߳�ռ�����
  * @param  obj   Mode(THREAD_IN�����߳̿�ͷ THREAD_OUT���ڽ�β,����)
  * @retval None
  */
void ThreadMonitor(ThreadMonitor_t* obj,uint8_t Mode)
{
	uint32_t currentTime = xTaskGetTickCount();			//�����ϰ��õ����Լ�д��Get_Time_Micros()��ֱ�Ӷ�ȡTIM2->CNT
	if(Mode == THREAD_IN)
	{
		obj->deltaTime = xTaskGetTickCount() - obj->previousTime;	//��������ϴ����е�ʱ��
		obj->previousTime = currentTime;							//�߳��ϴν����ʱ��
	}
	else if(Mode == THREAD_OUT)
	{
		obj->excutionTime = currentTime - obj->previousTime;		//�̵߳�ִ��ʱ��
	}	
}

//portBASE_TYPE Control_Stack = 1;
//portBASE_TYPE DataStream_Stack = 1;
portBASE_TYPE SuperviseTask_Stack = 1;

void Task_Monitor(void)
{		
	FrameGet();
	ErrorFlagSet();
		
//-----------------------------------����ϵͳ��ջ���------------------------------------
//	DataStream_Stack=uxTaskGetStackHighWaterMark(Data_StreamHandle);
//	Control_Stack=uxTaskGetStackHighWaterMark(Task_ControlHandle);
	SuperviseTask_Stack = uxTaskGetStackHighWaterMark(SuperviseHandle);
//	uxTaskGetStackHighWaterMark����Ҫ��CubeMX�й�ѡEnable,�������ֶ�ʹ�ܺ�����
}		

void FrameGet(void)//���֡����Ϣ
{
	//DBUS֡��ͳ��(��)
	DBUSFrameRate = DBUSFrameCounter*2;	
	DBUSFrameCounter = 0;
	
	//CAN�շ�֡��ͳ��(��)
	CAN_Send_FrameRate[0] = CAN_Send_FrameCounter[0]*2;
	CAN_Send_FrameCounter[0] = 0;
	CAN_Send_FrameRate[1] = CAN_Send_FrameCounter[1]*2;
	CAN_Send_FrameCounter[1] =0;
	
	CAN_Res_FrameRate[0] = CAN_Res_FrameCounter[0]*2;
	CAN_Res_FrameCounter[0] = 0;
	CAN_Res_FrameRate[1] = CAN_Res_FrameCounter[1]*2;
	CAN_Res_FrameCounter[1] = 0;
	
	//������֡��
	IMUFrameRate = IMUFrameCounter*2;
	IMUFrameCounter = 0;

	//����2֡�ʼ��(��),���ڻ�û�õ�,������,�Ժ���ܻ���
	UART2FrameRate = UART2FrameCounter*2;
	UART2FrameCounter = 0;
	
	//��̨YAW��PITCH(��)
	YAWFrameRate = YAWFrameCounter*2;
	YAWFrameCounter = 0;	
	PITCHFrameRate = PITCHFrameCounter*2;
	PITCHFrameCounter = 0;
	//miniPC(��)
	MiniPCFrameRate_USBRx = MiniPCFrameCounter_USBRx*2;
	MiniPCFrameCounter_USBRx = 0;	
	MiniPCFrameRate_USBTx = MiniPCFrameCounter_USBTx*2;
	MiniPCFrameCounter_USBTx = 0;
	//Ħ����(��)
	FrictionFrameRate_Left = FrictionFrameCounter_Left*2;
	FrictionFrameCounter_Left = 0;
	FrictionFrameRate_Right = FrictionFrameCounter_Right*2;
	FrictionFrameCounter_Right = 0;
	//����(��)
	BulletPlateFrameRate = BulletPlateFrameCounter*2;
	BulletPlateFrameCounter = 0;	
}

void ErrorFlagSet(void)//���ô���λ
{
	//ң����֡�ʹ���(��)
	if(DBUSFrameRate < 10)
	{
		Set_Error_Flag(LOST_ERROR_RC);
		error_count[0]++;
	}
	else
	{
		Reset_Error_Flag(LOST_ERROR_RC);
	}
       //CAN�շ�֡�ʹ���(��)
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
	
	//�����Ǽ��
	if(IMUFrameRate < 200)
	{
		Set_Error_Flag(LOST_ERROR_IMU);
		error_count[15]++;
	}
	else
	{
		Reset_Error_Flag(LOST_ERROR_IMU);
	}
	//��̨(��)
	//�����̨���Ƶ���е�ͣ�ʵ��Ҫ������
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
	//MiniPC(��)
	//usb�ǵ�һ��ʹ�ã�����Ƶ�ʻ�Ҫ���ԡ�����200
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
	//Ħ����(��)
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
	//����(��)
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
*/

void Supervise(void const * argument)
{
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
  /* Infinite loop */
	for(;;)
	{
		Task_Monitor();	
		osDelayUntil(&xLastWakeTime,500/portTICK_RATE_MS);//���м��0.5s
	}
}
osThreadId SuperviseHandle;
void SuperviseThreadCreate(osPriority taskPriority)
{
	osThreadDef(SuperviseThread,Supervise,taskPriority,0,256);
	SuperviseHandle = osThreadCreate(osThread(SuperviseThread),NULL);
}
