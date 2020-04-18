/**
 * @brief  Friction motor and Bullet plate Control
 * @param	Control the friction wheel motor, transmission and dial motor
 * @author zcd
 * @Time 2020 1 9 
*/

#include "Friction.h"
#include "CAN_BUS_Driver.h"
#include "cmsis_os.h"
#include "PID.h"
#include "Remote_Driver.h"

uint8_t Single_Shoot_Flag = 1;

BulletPlate_ref_t BulletPlate_ref={0,0};
Shoot_State_e Shoot_State = NOSHOOTING;
FrictionWheelState_e friction_wheel_state;
FrictionWheelSpeed_Mode_e FrictionWheel_Speed;


PID_Regulator_t FrictionMotor_Speed_Left_3508 = CHASSIS_MOTOR_SPEED_PID_DEFAULT;
PID_Regulator_t FrictionMotor_Speed_Right_3508 = CHASSIS_MOTOR_SPEED_PID_DEFAULT;

PID_Regulator_t BulletPlate_Speed_2006 = SHOOT_MOTOR_SPEED_PID_DEFAULT;
PID_Regulator_t BulletPlate_Position_2006 = SHOOT_MOTOR_POSITION_PID_DEFAULT;

int BulletPlate_Angle_Init=0;
int BulletPlate_Speed_Init=0;
int16_t FrictionMotor_Speed_Low = 500;
int16_t FrictionMoter_Speed_Hight = 700;
int16_t FrictionMotor_Start_TimeStamp=0;

/**
 * @brief  Friction motor PID update
 * @author zcd
 * @Time 2020 1 9 
*/
  void Prepare_time_pid_update(void)
{
	FrictionMotor_Speed_Left_3508.kp = 20;
	FrictionMotor_Speed_Left_3508.ki = 0.6;
	FrictionMotor_Speed_Left_3508.kd = 5;

	FrictionMotor_Speed_Right_3508.kp = 20;
	FrictionMotor_Speed_Right_3508.ki = 0.6;
	FrictionMotor_Speed_Right_3508.kd = 5;
}

void Shoot_time_pid_update(void)
{
	FrictionMotor_Speed_Left_3508.kp = 80;
	FrictionMotor_Speed_Left_3508.ki = 0.3;
	FrictionMotor_Speed_Left_3508.kd = 5;

	FrictionMotor_Speed_Right_3508.kp = 80;
	FrictionMotor_Speed_Right_3508.ki = 0.3;
	FrictionMotor_Speed_Right_3508.kd = 5;
}

/**
 * @brief  Friction wheel control
 * @param  friction_wheel_state
 * @author zcd
 * @Time 2020 1 9 
*/
void Frition_Control(void)
{
	switch(friction_wheel_state)
	{
		case FRICTION_WHEEL_OFF :
		{
			Prepare_time_pid_update();
			FrictionMotor_Start_TimeStamp = 0;
			PID_Task(&FrictionMotor_Speed_Left_3508,0,Friction_Speed_Left_Measure.speed_rpm/10.0);
			PID_Task(&FrictionMotor_Speed_Right_3508,0,Friction_Speed_Right_Measure.speed_rpm/10.0);		
		}break;
		
		case FRICTION_WHEEL_ON :
		{
			FrictionMotor_Start_TimeStamp++;
			if(FrictionMotor_Start_TimeStamp>1000)
			{
				Shoot_time_pid_update();
			}
			if(FrictionWheel_Speed==Speed_Low)
			{
				PID_Task(&FrictionMotor_Speed_Left_3508,FrictionMotor_Speed_Low,Friction_Speed_Left_Measure.speed_rpm/10.0);
				PID_Task(&FrictionMotor_Speed_Right_3508,-FrictionMotor_Speed_Low,Friction_Speed_Right_Measure.speed_rpm/10.0);	
			}
			else if(FrictionWheel_Speed== Speed_Hight)
			{
				PID_Task(&FrictionMotor_Speed_Left_3508,FrictionMoter_Speed_Hight,Friction_Speed_Left_Measure.speed_rpm/10.0);
				PID_Task(&FrictionMotor_Speed_Right_3508,-FrictionMoter_Speed_Hight,Friction_Speed_Right_Measure.speed_rpm/10.0);					
			}
			else
			{
				PID_Task(&FrictionMotor_Speed_Left_3508,0,Friction_Speed_Left_Measure.speed_rpm/10.0);
				PID_Task(&FrictionMotor_Speed_Right_3508,0,Friction_Speed_Right_Measure.speed_rpm/10.0);				
			}	
				
		}break;
		
		case FRICTION_WHEEL_START_TURNNING:
		{
			friction_wheel_state = FRICTION_WHEEL_ON;//强行将状态转化
		}break;
		
		default:break;
	}
	CAN1_Tx_Buff_Std[4]=(int16_t)(FrictionMotor_Speed_Left_3508.output)>>8;
	CAN1_Tx_Buff_Std[5]=(unsigned char)(FrictionMotor_Speed_Left_3508.output);

	CAN1_Tx_Buff_Std[6]=(int16_t)(FrictionMotor_Speed_Right_3508.output)>>8;
	CAN1_Tx_Buff_Std[7]=(unsigned char)(FrictionMotor_Speed_Right_3508.output);

	CAN1_Send(CAN1_Tx_Buff_Std,0x200);//测试
}
/**
 * @brief  BulletPlate control
 * @param  BulletPlate 
 * @author zcd
 * @Time 2020 1 9 
*/
void BulletPlate_Control(void)
{
	BulletPlate_Speed_2006.kp = 5;
	BulletPlate_Speed_2006.ki = 0.1;		//AppParamRealUsed.TurntableSpeedPID.ki_offset/1000.0;
	BulletPlate_Speed_2006.kd = 0;		//AppParamRealUsed.TurntableSpeedPID.kd_offset;
	BulletPlate_Position_2006.kp = 200;
	BulletPlate_Position_2006.ki = 0;	//AppParamRealUsed.TurntablePositionPID.ki_offset/1000.0;
	BulletPlate_Position_2006.kd = 0;	//AppParamRealUsed.TurntablePositionPID.kd_offset;
	BulletPlate_ref.Speed = 1000;

	if(friction_wheel_state==FRICTION_WHEEL_ON)
	{
		if(Shoot_State==SHOOTING)
		{
			if(RC_CtrlData.rc.switch_right == SWITCH_UP	)
			{
				if((Single_Shoot_Flag==0)&&(RC_CtrlData.rc.switch_right==SWITCH_UP))
				{
					Single_Shoot_Flag=1;
					BulletPlate_Angle_Init=BulletPlate_Angle_Init+45;//1620;
				}
				else
				{
				}
			}
			else 
			{
				Single_Shoot_Flag=0;
				BulletPlate_Angle_Init=BulletPlate_CM2006Encoder.ecd_angle/36.0f;
				BulletPlate_Speed_Init=0;
				goto BulletPlateStop;
			}
		}
		else
		{
			BulletPlate_Speed_Init=0;
			goto BulletPlateStop;
		}
		
		PID_Task(&BulletPlate_Position_2006,BulletPlate_Angle_Init,BulletPlate_CM2006Encoder.ecd_angle/36.0f);
		BulletPlate_Speed_Init=BulletPlate_Position_2006.output;
		BulletPlateStop:
		PID_Task(&BulletPlate_Speed_2006,BulletPlate_Speed_Init,BulletPlate_Measure.speed_rpm);	
	
	}
	else
	{
		PID_Task(&BulletPlate_Speed_2006,0,BulletPlate_Measure.speed_rpm);	
	}
		

	CAN1_Tx_Buff_Std[2]=(int16_t)(BulletPlate_Speed_2006.output)>>8;
	CAN1_Tx_Buff_Std[3]=(unsigned char)(BulletPlate_Speed_2006.output);	

}

osThreadId FrictionThread_Handle;
void FrictionTask(void const* argument)
{
	
	FrictionWheel_Speed=Speed_Low;
	
	for(;;)
	{
		if(RC_CtrlData.rc.switch_left==SWITCH_CENTRAL)
		{
			friction_wheel_state=FRICTION_WHEEL_OFF;

		}
		else if(RC_CtrlData.rc.switch_left==SWITCH_UP)
		{
			friction_wheel_state=FRICTION_WHEEL_ON;
			Shoot_State=SHOOTING;
		}
		else if(RC_CtrlData.rc.switch_left==SWITCH_DOWN)
		{
			friction_wheel_state=FRICTION_WHEEL_OFF;		
		}
		else
		{
			friction_wheel_state=FRICTION_WHEEL_OFF;
		
		}
	BulletPlate_Control();
	Frition_Control();
	osDelay(10);		
	}

}

void FrictionThreadCreate(osPriority taskPriority)
{
	osThreadDef(Friction_ControlThread,FrictionTask,taskPriority,0,256);
	FrictionThread_Handle=osThreadCreate(osThread(Friction_ControlThread),NULL);

}





