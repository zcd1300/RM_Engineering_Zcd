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
#include "usart.h"
#include "XDU_USB_HID_Z.h"
#include "Msg_Frame.h"
#include "Remote_Driver.h"
#include "Motor_ConttrolTask.h" 
#include "Friction.h"
#include "User_Math.h"

uint16_t GMSpeeedTest=200;
uint8_t YAW_Initial_Angle_FLAG=0;
uint8_t PITCH_Initial_Angle_FLAG=0;

volatile float YAW_Initial_Angle=-101;	//= 
volatile float YAW_Target_Angle=-101;
volatile float PITCH_Initial_Angle=185;
volatile float PITCH_Target_Angle=185;

int16_t Yaw_InitAngle_ABS=-101;				//������̨Yawʱˮƽ���ԽǶ�-99 - -102֮��
int16_t Pitch_InitAngle_ABS=185;			//������̨Pitchʱˮƽ���ԽǶ�184-196֮��

Ramp_Init_e Yaw_Ramp_Init={-101,20,50,0,0,0};		//���������Ǳ߽Ƕ��ǵ���������ʹ�õȿ����Ǳ߽Ƕ���д��
Ramp_Init_e Pitch_Ramp_Init={-101,20,50,0,0,0};


PID_Regulator_t GM6020_Yaw_PositionPID = GIMBAL_MOTOR_YAW_POSITION_PID_DEFAULT;
PID_Regulator_t GM6020_Yaw_SpeedPID =GIMBAL_MOTOR_YAW_SPEED_PID_DEFAULT;
PID_Regulator_t GM6020_Pitch_PositionPID=GIMBAL_MOTOR_PITCH_POSITION_PID_DEFAULT;
PID_Regulator_t GM6020_Pitch_SpeedPID=GIMBAL_MOTOR_PITCH_SPEED_PID_DEFAULT;

/**
 * @brief gimbal prepare task 
 * @retval
 * @ZCD
 * @Time 2020 1 13
*/
void GM_prepare(void)
{
//	YAW_Initial_Angle=YAW_GM6020Encoder.ecd_angle;	
//	PITCH_Initial_Angle=PITCH_GM6020Encoder.ecd_angle;
	PID_Task(&GM6020_Yaw_PositionPID,YAW_Target_Angle,YAW_GM6020Encoder.ecd_angle);
	PID_Task(&GM6020_Yaw_SpeedPID,GM6020_Yaw_PositionPID.output*400,YAW_GM6020Encoder.filter_rate);
	
	PID_Task(&GM6020_Pitch_PositionPID,PITCH_Target_Angle,PITCH_GM6020Encoder.ecd_angle);
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

	YAW_Initial_Angle = NumRangeLimit(YAW_Initial_Angle,-165,-40);
	PITCH_Initial_Angle = NumRangeLimit(PITCH_Initial_Angle,160,215);	
	
	YAW_Target_Angle = YAW_Initial_Angle;
	PITCH_Target_Angle = PITCH_Initial_Angle;
	
	
	PID_Task(&GM6020_Yaw_PositionPID,YAW_Target_Angle,YAW_GM6020Encoder.ecd_angle);
	PID_Task(&GM6020_Yaw_SpeedPID,GM6020_Yaw_PositionPID.output*400,YAW_GM6020Encoder.filter_rate);
	
	PID_Task(&GM6020_Pitch_PositionPID,PITCH_Target_Angle,PITCH_GM6020Encoder.ecd_angle);
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
void Mouse_GM_Control(void)//��δ����
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

void MOTOR_CONTROL(void const* argument)
{

	for(;;)
	{	
	
		if(RC_CtrlData.rc.switch_left==SWITCH_DOWN)
		{	
			Vision_GMMoto_Control();		
		}
		else if(RC_CtrlData.rc.switch_left==SWITCH_UP)
		{	
			RC_GM_Control();			
		}
		else
		{
			GM_prepare();
		}
		osDelay(10/portTICK_RATE_MS);
	
	}
}

osThreadId Motor_ControlHandle;

void Motor_ControlThraedCreate (osPriority taskPriority)
{
	osThreadDef(Motor_ControlThread,MOTOR_CONTROL,taskPriority,0,256);
	Motor_ControlHandle=osThreadCreate(osThread(Motor_ControlThread),NULL);

}




