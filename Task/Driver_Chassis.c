/*������Ҫʹ�õĽṹ��*/
#include "DriverLib_Ramp.h"
#include "stdlib.h"
#include "Driver_Chassis.h"
#include "Remote_Driver.h"
#include "cmsis_os.h"

float CM_SPEED_C = 1;
float CM_OMEGA_C = 1;
float underpan_acc = 30;

ChassisDataTypeDef ChassisData; 
ChassisMode_e ChassisMode;


/*ʹ�ü��̿��Ƶ�ʱ��õ����ٶȽ��㺯��*/ 
void Key2Speed(int16_t FB, int16_t LR)
{
	//����ģʽ
	int16_t tmp_FB = 0;
	int16_t tmp_LR = 0;

	tmp_FB = FB ;
	tmp_LR = LR ;

	//����ĸ���ģʽ���°湤�̵ķ����������߼���ͻ
	if(Remote_CheckJumpKey(KEY_W) == 1&&RC_CtrlData.mouse.press_r == 0 &&RC_CtrlData.mouse.press_l == 0)
	{
		ChassisData.ChassisSpeedRef.Y = tmp_FB*FBSpeedRamp.Calc(&FBSpeedRamp);
	}
	else if(Remote_CheckJumpKey(KEY_S) == 1&&RC_CtrlData.mouse.press_r == 0 &&RC_CtrlData.mouse.press_l == 0)
	{
		ChassisData.ChassisSpeedRef.Y = -tmp_FB*FBSpeedRamp.Calc(&FBSpeedRamp);
	}
	else
	{
		FBSpeedRamp.ResetCounter(&FBSpeedRamp);
		FBSpeedRamp.SetCounter(&FBSpeedRamp, MOUSR_FB_RAMP_TICK_COUNT *0.2);
		ChassisData.ChassisSpeedRef.Y = 0;
	}

	if(Remote_CheckJumpKey(KEY_D) == 1&&RC_CtrlData.mouse.press_r == 0 &&RC_CtrlData.mouse.press_l == 0)
	{
		ChassisData.ChassisSpeedRef.X = tmp_LR*LRSpeedRamp.Calc(&LRSpeedRamp);
	}
	else if(Remote_CheckJumpKey(KEY_A) == 1&&RC_CtrlData.mouse.press_r == 0 &&RC_CtrlData.mouse.press_l == 0)
	{
		ChassisData.ChassisSpeedRef.X = -tmp_LR*LRSpeedRamp.Calc(&LRSpeedRamp);
	}
	else
	{
		LRSpeedRamp.ResetCounter(&LRSpeedRamp);
		LRSpeedRamp.SetCounter(&LRSpeedRamp, MOUSE_LR_RAMP_TICK_COUNT *0.2);
		ChassisData.ChassisSpeedRef.X = 0;
	}
}


/*�õ�x��y,omega�ĺ���*/ 
//�������ݽ���ֻ�õ����������������
void CM_Get_SpeedRef(void)
{
	if(ChassisMode == Chassis_Locked)
	{
		ChassisData.ChassisSpeedRef.Y = 0;
		ChassisData.ChassisSpeedRef.X = 0;
		ChassisData.ChassisSpeedRef.Omega  = 0;
		ChassisData.ChassisAngle = 0;
	}
	else if(ChassisMode == Chassis_NormalRCMode)//ң�������� 
	{
		ChassisData.ChassisSpeedRef.Y = RC_CtrlData.rc.Channel_2;
		ChassisData.ChassisSpeedRef.X = RC_CtrlData.rc.Channel_1;
		
		

		ChassisData.ChassisAngle = 0;
		
		static float Y_temp = 0;
		static float X_temp = 0;
		//����ң�������ٶ�
		//�����ǿ��ת�����ܴ������⣬����absһֱ��ʾ����̫���ˣ��������ɣ�VS��������û����
		if(abs(RC_CtrlData.rc.Channel_2 - (int16_t)Y_temp) > underpan_acc)
		{
			if(RC_CtrlData.rc.Channel_2 >Y_temp && RC_CtrlData.rc.Channel_2 > 0)
				Y_temp+=underpan_acc;
			else if (RC_CtrlData.rc.Channel_2 <Y_temp && RC_CtrlData.rc.Channel_2 < 0)
				Y_temp-=underpan_acc;	
			else
				Y_temp = RC_CtrlData.rc.Channel_2;
		}
		else
		{
			Y_temp = RC_CtrlData.rc.Channel_2;
		}
		
		if(abs(RC_CtrlData.rc.Channel_1 - (int16_t)X_temp) > underpan_acc)
		{
			if(RC_CtrlData.rc.Channel_1 >X_temp && RC_CtrlData.rc.Channel_1 > 0)
				X_temp+=underpan_acc;
			else if (RC_CtrlData.rc.Channel_1 <X_temp && RC_CtrlData.rc.Channel_1 < 0)
				X_temp-=underpan_acc;	
			else
				X_temp = RC_CtrlData.rc.Channel_1;
		}
		else
		{
			X_temp = RC_CtrlData.rc.Channel_1;
		}
		ChassisData.ChassisSpeedRef.Y		= Y_temp * CM_SPEED_C;
		ChassisData.ChassisSpeedRef.X   	= X_temp * CM_SPEED_C; 
		ChassisData.ChassisSpeedRef.Omega  = RC_CtrlData.rc.Channel_3/2*CM_OMEGA_C;
	}
	else if(ChassisMode == Chassis_KeyMouseMode)//������� 
	{
		
	
		Key2Speed(NORMAL_FORWARD_BACK_SPEED, NORMAL_LEFT_RIGHT_SPEED);
		//����������ʹ���������������ٶȵģ��ȱ����Ű� 
		if(Remote_CheckJumpKey(KEY_SHIFT))
		{
				Key2Speed(HIGH_FORWARD_BACK_SPEED, HIGH_LEFT_RIGHT_SPEED);
		}
		else if(Remote_CheckJumpKey(KEY_CTRL))
		{
				Key2Speed(LOW_FORWARD_BACK_SPEED, LOW_LEFT_RIGHT_SPEED);
		}
		
		
		ChassisData.ChassisSpeedRef.Omega = RC_CtrlData.mouse.x*KEY_FACTOR_NORMAL;				
	}
	else
	{
		ChassisData.ChassisSpeedRef.Y = 0;
		ChassisData.ChassisSpeedRef.X = 0;
		ChassisData.ChassisSpeedRef.Omega  = 0;
		ChassisData.ChassisAngle = 0;
	
	}
}


//���̳�ʼ��,�ٶȽǶ�ȫΪ0
void ChassisData_Init(void)
{
	ChassisData.ChassisSpeedRef.Y = 0;
	ChassisData.ChassisSpeedRef.X = 0;
	ChassisData.ChassisSpeedRef.Omega  = 0;
	ChassisData.ChassisAngle = 0;
}

void Chassis_Decode(void const* argument)
{
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	for(;;)
	{
		CM_Get_SpeedRef();
		osDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}
}
osThreadId Chassis_DecodeHandle;
void Chassis_DecodeThreadCreate(osPriority taskPriority)
{
	osThreadDef(Chassis_DecodeThread,Chassis_Decode,taskPriority,0,256);
	Chassis_DecodeHandle = osThreadCreate(osThread(Chassis_DecodeThread),NULL);
	
}
