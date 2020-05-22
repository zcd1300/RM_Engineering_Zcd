/**
 * @brief  Control codes for all motors
 * @param	Control codes for all motors
 * @retval
 * @ZCD
 * @Time 2019 12 7 
*/

#include "CAN_BUS_Driver.h"
#include "cmsis_os.h"
#include "PID.h"
#include "Msg_Frame.h"
#include "Remote_Driver.h"
#include "Motor_ConttrolTask.h" 
#include "User_Math.h"
#include "Fuzzy_Controller.h"
#include "StatusManagement.h"
#include "Flash_Operation.h"
//#include "Friction.h"
//#include "usart.h"
//#include "XDU_USB_HID_Z.h"




uint16_t GMSpeeedTest=200;
uint8_t YAW_Initial_Angle_FLAG=0;
uint8_t PITCH_Initial_Angle_FLAG=0;

volatile float YAW_Initial_Angle=-101;	//= 
volatile float YAW_Target_Angle=-101;
volatile float PITCH_Initial_Angle=120;//185
volatile float PITCH_Target_Angle=120;//185

int16_t Yaw_InitAngle_ABS=-242;				//������̨Yawʱˮƽ���ԽǶ�-99 - -102֮��
int16_t Pitch_InitAngle_ABS=185;			//������̨Pitchʱˮƽ���ԽǶ�184-196֮��

Ramp_Init_e Yaw_Ramp_Init={-101,20,50,0,0,0};		//���������Ǳ߽Ƕ��ǵ���������ʹ�õȿ����Ǳ߽Ƕ���д��
Ramp_Init_e Pitch_Ramp_Init={-101,20,50,0,0,0};


PID_Regulator_t GM6020_Yaw_PositionPID = GIMBAL_MOTOR_YAW_POSITION_PID_DEFAULT;
PID_Regulator_t GM6020_Yaw_SpeedPID =GIMBAL_MOTOR_YAW_SPEED_PID_DEFAULT;
PID_Regulator_t GM6020_Pitch_PositionPID=GIMBAL_MOTOR_PITCH_POSITION_PID_DEFAULT;
PID_Regulator_t GM6020_Pitch_SpeedPID=GIMBAL_MOTOR_PITCH_SPEED_PID_DEFAULT;


//������Χ��δ���ԣ��������һ��
Fuzzy Fuzzy_YAW_Speed={0,{0,0,0},0,0,0,0,20,0,1,10,0,1,5,0,1};
Fuzzy Fuzzy_YAW_Position={0,{0,0,0},0,0,0,0,20,0,1,10,0,1,5,0,1};

Fuzzy Fuzzy_PITCH_Speed={0,{0,0,0},0,0,0,0,20,0,0.1,10,0,0.1,5,0,0.1};
Fuzzy Fuzzy_PITCH_Position={0,{0,0,0},0,0,0,0,20,0,0.1,10,0,0.1,5,0,0.1};
/**
 * @brief gimbal prepare task 
 * @retval
 * @ZCD
 * @Time 2020 1 13
*/
void GM_prepare(void)
{
	GM6020_Pitch_SpeedPID.outputMax = 6000;
	//��ʱ���ܲ���Ч�����ȷ���ɣ��������˺������÷������������������������̨�Ҷ�
	Connect_PID_FUZZY(&Fuzzy_YAW_Position,&GM6020_Yaw_PositionPID,YAW_Target_Angle);//����������⣬�ᵼ��PID������
	PID_Task(&GM6020_Yaw_PositionPID,YAW_Target_Angle,YAW_GM6020Encoder.ecd_angle);
	
	Connect_PID_FUZZY(&Fuzzy_YAW_Speed,&GM6020_Yaw_SpeedPID,GM6020_Yaw_PositionPID.output);
	PID_Task(&GM6020_Yaw_SpeedPID,GM6020_Yaw_PositionPID.output*40,YAW_GM6020Encoder.filter_rate);
	//ֱ����������Ա������ܵ������Ծ�ȷ���ƣ�����ȥ��������Kp�ĵ��ڷ�Χ
	
	Connect_PID_FUZZY(&Fuzzy_PITCH_Position,&GM6020_Pitch_PositionPID,PITCH_Target_Angle);
	PID_Task(&GM6020_Pitch_PositionPID,PITCH_Target_Angle,PITCH_GM6020Encoder.ecd_angle);

	Connect_PID_FUZZY(&Fuzzy_PITCH_Speed,&GM6020_Pitch_SpeedPID,GM6020_Pitch_PositionPID.output);
	PID_Task(&GM6020_Pitch_SpeedPID,GM6020_Pitch_PositionPID.output*4,PITCH_GM6020Encoder.filter_rate);

	CAN1_Tx_Buff_Ext[0] = (int16_t)GM6020_Yaw_SpeedPID.output>>8;
	CAN1_Tx_Buff_Ext[1] = (unsigned char)GM6020_Yaw_SpeedPID.output;
	CAN1_Tx_Buff_Ext[2] = (int16_t)GM6020_Pitch_SpeedPID.output>>8;
	CAN1_Tx_Buff_Ext[3] = (unsigned char)GM6020_Pitch_SpeedPID.output;	
	
	CAN1_Send(CAN1_Tx_Buff_Ext,0x1FF);
}

/**
 * @brief  Virsion gimbal test
 * @retval
 * @ZCD
 * @Time 2019 12 7 
*/
void Vision_GMMoto_Control(void)
{
	
	YAW_Target_Angle = NumRangeLimit(USB_Vision_Decoding.YAW_ChangeAngle-165,-165,-40);
	PITCH_Target_Angle = NumRangeLimit((-1*USB_Vision_Decoding.PITCH_ChangeAngle)+160,160,215);
/*
//	GM6020_Yaw_PositionPID.kp=1;
//	GM6020_Yaw_PositionPID.ki=0.02;
//	GM6020_Yaw_PositionPID.kd=0.01;
//	
//	GM6020_Yaw_SpeedPID.kp=100;
//	GM6020_Yaw_SpeedPID.ki=20;
//	GM6020_Yaw_SpeedPID.kd=50;
*/	
		
	PID_Task(&GM6020_Yaw_PositionPID,YAW_Target_Angle,YAW_GM6020Encoder.ecd_angle);
	PID_Task(&GM6020_Yaw_SpeedPID,GM6020_Yaw_PositionPID.output*200,YAW_GM6020Encoder.filter_rate);

//	PID_Task(&GM6020_Pitch_PositionPID,193,PITCH_GM6020Encoder.ecd_angle);
	PID_Task(&GM6020_Pitch_PositionPID,PITCH_Target_Angle,PITCH_GM6020Encoder.ecd_angle);
	PID_Task(&GM6020_Pitch_SpeedPID,GM6020_Pitch_PositionPID.output*8,PITCH_GM6020Encoder.filter_rate);
	
	CAN1_Tx_Buff_Ext[0] = (int16_t)GM6020_Yaw_SpeedPID.output>>8;
	CAN1_Tx_Buff_Ext[1] = (unsigned char)GM6020_Yaw_SpeedPID.output;
	CAN1_Tx_Buff_Ext[2] = (int16_t)GM6020_Pitch_SpeedPID.output>>8;
	CAN1_Tx_Buff_Ext[3] = (unsigned char)GM6020_Pitch_SpeedPID.output;
	
	CAN1_Send(CAN1_Tx_Buff_Ext,0x1FF);
	
}

/**
 * @brief  Remote control gimbal test
 * @retval
 * @ZCD
 * @Time 2020 1 8 
*/
void RC_GM_Control(void)
{
	if(YAW_Initial_Angle_FLAG<5)
	{
		YAW_Initial_Angle=YAW_GM6020Encoder.ecd_angle;
		YAW_Initial_Angle_FLAG++;
		PITCH_Initial_Angle=PITCH_GM6020Encoder.ecd_angle;
		PITCH_Initial_Angle_FLAG++;
		
	}
	
	YAW_Initial_Angle=YAW_Initial_Angle - ((float)RC_CtrlData.rc.Channel_3*0.0066f);
	PITCH_Initial_Angle=PITCH_Initial_Angle - ((float)RC_CtrlData.rc.Channel_4*0.0066f);

	YAW_Initial_Angle = NumRangeLimit(YAW_Initial_Angle,-165,-40);//-165 -40
	PITCH_Initial_Angle = NumRangeLimit(PITCH_Initial_Angle,90,130);//160 215	
	
	YAW_Target_Angle = YAW_Initial_Angle;
	PITCH_Target_Angle = PITCH_Initial_Angle;
	
	GM6020_Pitch_SpeedPID.outputMax = 6000;
	
	PID_Task(&GM6020_Yaw_PositionPID,YAW_Target_Angle,YAW_GM6020Encoder.ecd_angle);
	PID_Task(&GM6020_Yaw_SpeedPID,GM6020_Yaw_PositionPID.output*400,YAW_GM6020Encoder.filter_rate);
	
	Connect_PID_FUZZY(&Fuzzy_PITCH_Position,&GM6020_Pitch_PositionPID,PITCH_Target_Angle);	
	PID_Task(&GM6020_Pitch_PositionPID,PITCH_Target_Angle,PITCH_GM6020Encoder.ecd_angle);
	Connect_PID_FUZZY(&Fuzzy_PITCH_Speed,&GM6020_Pitch_SpeedPID,GM6020_Pitch_PositionPID.output);	
	PID_Task(&GM6020_Pitch_SpeedPID,GM6020_Pitch_PositionPID.output*4,PITCH_GM6020Encoder.filter_rate);

	
	CAN1_Tx_Buff_Ext[0] = (int16_t)GM6020_Yaw_SpeedPID.output>>8;
	CAN1_Tx_Buff_Ext[1] = (unsigned char)GM6020_Yaw_SpeedPID.output;
	CAN1_Tx_Buff_Ext[2] = (int16_t)GM6020_Pitch_SpeedPID.output>>8;
	CAN1_Tx_Buff_Ext[3] = (unsigned char)GM6020_Pitch_SpeedPID.output;
	
	CAN1_Send(CAN1_Tx_Buff_Ext,0x1FF);
}
/**
 * @brief  Mouse control gimbal test
 * @retval
 * @ZCD
 * @Time 2020 4 14 
*/
void Mouse_GM_Control(void)//��δ��������
{
	if(YAW_Initial_Angle_FLAG<5)
	{
		YAW_Initial_Angle=YAW_GM6020Encoder.ecd_angle;
		YAW_Initial_Angle_FLAG++;
		PITCH_Initial_Angle=PITCH_GM6020Encoder.ecd_angle;
		PITCH_Initial_Angle_FLAG++;
	}


	YAW_Target_Angle = YAW_Initial_Angle + RC_CtrlData.mouse.x/16;
	PITCH_Target_Angle = PITCH_Initial_Angle - RC_CtrlData.mouse.y/16;

	YAW_Target_Angle = NumRangeLimit(YAW_Target_Angle,-165,-40);
	PITCH_Target_Angle = NumRangeLimit(PITCH_Target_Angle,160,215);	
	
	PID_Task(&GM6020_Yaw_PositionPID,YAW_Target_Angle,YAW_GM6020Encoder.ecd_angle);
	PID_Task(&GM6020_Yaw_SpeedPID,GM6020_Yaw_PositionPID.output*100,YAW_GM6020Encoder.filter_rate);
	
	PID_Task(&GM6020_Pitch_PositionPID,PITCH_Target_Angle,PITCH_GM6020Encoder.ecd_angle);
	PID_Task(&GM6020_Pitch_SpeedPID,GM6020_Pitch_PositionPID.output,PITCH_GM6020Encoder.filter_rate);

	CAN1_Tx_Buff_Ext[0] = (int16_t)GM6020_Yaw_SpeedPID.output>>8;
	CAN1_Tx_Buff_Ext[1] = (unsigned char)GM6020_Yaw_SpeedPID.output;
	CAN1_Tx_Buff_Ext[2] = (int16_t)GM6020_Pitch_SpeedPID.output>>8;
	CAN1_Tx_Buff_Ext[3] = (unsigned char)GM6020_Pitch_SpeedPID.output;
	
	CAN1_Send(CAN1_Tx_Buff_Ext,0x1FF);
}

	
/**
 * @brief  Gimbal calibration
 * @retval	void
 * @ZCD
 * @Time 2020 5 20 //�����ռǣ�5.20 �� ����ܲ����ģ��ռ䡢����Ȧ�ﶼ��������ġ���ֻ��һ�������ש��
*/
uint16_t GimbalCalibrationKEY_ExitTime = 0;
uint16_t GimbalCalibrationLED_Disp = 0;
uint8_t GimbalCalibrationLED_DispInitFlag = 0;

// const uint8_t FlashWrite_Buff3[]={"1234567890"};

void GimbalCalibration_Control(void)
{
	CAN1_Tx_Buff_Ext[0] = 0;
	CAN1_Tx_Buff_Ext[1] = 0;
	CAN1_Tx_Buff_Ext[2] = 0;
	CAN1_Tx_Buff_Ext[3] = 0;	
	CAN1_Send(CAN1_Tx_Buff_Ext,0x1FF);//���ֹͣ���
	GimbalCalibrationLED_Disp++;//�����������ʵ�ֶ�Ӧ��LED��˸
	
	FlashWrite_Buff[0] = 0xaa;//��̨�Ѿ�У׼��־
	FlashWrite_Buff[1] = (int16_t)YAW_GM6020Encoder.ecd_angle>>8;
	FlashWrite_Buff[2] = (int16_t)YAW_GM6020Encoder.ecd_angle;//YAWУ׼ֵ
	
	FlashWrite_Buff[3] = (int16_t)PITCH_GM6020Encoder.ecd_angle>>8;
	FlashWrite_Buff[4] = (int16_t)PITCH_GM6020Encoder.ecd_angle;//PITCHУ׼ֵ
	if(GimbalCalibrationLED_DispInitFlag == 0)
	{
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_RESET);	
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_14,GPIO_PIN_SET);	
		GimbalCalibrationLED_DispInitFlag = 1;
	}
	if(((GimbalCalibrationLED_Disp%50)?0:1))
	{
		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_11);
		HAL_GPIO_TogglePin(GPIOF,GPIO_PIN_14);		
	}
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2) == GPIO_PIN_SET)
	{
		GimbalCalibrationKEY_ExitTime++;
	}
	if(GimbalCalibrationKEY_ExitTime>600)
	{
		GimbalCalibrationLED_DispInitFlag = 0;	
		YAW_Initial_Angle = YAW_GM6020Encoder.ecd_angle;
		PITCH_Initial_Angle = PITCH_GM6020Encoder.ecd_angle;

		//vTaskSuspendAll();

		Flash_Write(Gimbal_Flash_SaveAddr,2,(uint32_t*)FlashWrite_Buff); 

		//xTaskResumeAll();
		
		//����Ҫ��̨�л���׼��ģʽ����ʼ��	

		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_RESET);	
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_14,GPIO_PIN_RESET);	
		HAL_Delay(500);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_14,GPIO_PIN_SET);
		HAL_Delay(1000);

		GimbalCalibrationKEY_ExitTime = 0 ;			
		Gimbal_Debug_Flag = 0;	//У׼״̬�ͷ�
	}

}

/**
 * @brief  Gimbal State switch
 * @param void
 * @retval void
 * @ZCD
 * @Time 2020 5 12 
*/
void GimbalMode_Switch(void)
{
	switch(Gimbal_Mode)
	{
		case Gimbal_Prepare:
		{
			GM_prepare();
		}break;
		case Gimbal_Stop:
		{
			GM_prepare();//��ֹ̨ͣ��Ϊ�˷�ֹ��̨�ղ����źŷ�����л���׼��״̬
		}break;
		case Gimbal_RC_Mode:
		{
			RC_GM_Control();
		}break;
		case Gimbal_Mouse_Mode:
		{
//			Mouse_GM_Control();//�����ƻ�û��ϸ����
		}break;
		case Gimbal_Follow:
		{
			Vision_GMMoto_Control();//�Ӿ�����ģʽ���Ӿ����������е�����
		}break;
		case Gimbal_Debug:
		{
		//��̨У׼�����Զ�ӦLED��˸
			GimbalCalibration_Control();
		}break;
		default:
		{
			GM_prepare();
		}break;
	}
}


void MOTOR_CONTROL(void const* argument)
{
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	for(;;)
	{	
		GimbalMode_Switch();

		osDelayUntil(&xLastWakeTime,5/portTICK_RATE_MS);
	}
}

osThreadId Motor_ControlHandle;

void Motor_ControlThraedCreate (osPriority taskPriority)
{
	osThreadDef(Motor_ControlThread,MOTOR_CONTROL,taskPriority,0,256);
	Motor_ControlHandle=osThreadCreate(osThread(Motor_ControlThread),NULL);
//	vTaskPrioritySet(Motor_ControlHandle,taskPriority);
}

