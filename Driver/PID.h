#ifndef __PID_H
#define __PID_H

#define YAW_POSITION_KP_DEFAULTS  1//8
#define YAW_POSITION_KI_DEFAULTS  0
#define YAW_POSITION_KD_DEFAULTS  0

#define YAW_SPEED_KP_DEFAULTS  1//  25
#define YAW_SPEED_KI_DEFAULTS  0
#define YAW_SPEED_KD_DEFAULTS  0

// avoid bang --->  position:20.0  speed:19.0
//big bang   22.5 20.0
#define PITCH_POSITION_KP_DEFAULTS  10.0
#define PITCH_POSITION_KI_DEFAULTS  0
#define PITCH_POSITION_KD_DEFAULTS  0

#define PITCH_SPEED_KP_DEFAULTS  7.0	//7
#define PITCH_SPEED_KI_DEFAULTS  0
#define PITCH_SPEED_KD_DEFAULTS  0

#define CHASSIS_SPEED_KP_DEFAULTS  20.0
#define CHASSIS_SPEED_KI_DEFAULTS  0
#define CHASSIS_SPEED_KD_DEFAULTS  0

#define CHASSIS_ROTATE_KP_DEFAULTS  4
#define CHASSIS_ROTATE_KI_DEFAULTS  0
#define CHASSIS_ROTATE_KD_DEFAULTS  0


#define SHOOT_SPEED_KP_DEFAULTS  5//1.5
#define SHOOT_SPEED_KI_DEFAULTS  0
#define SHOOT_SPEED_KD_DEFAULTS  0

#define PID_TURNTABLE_SPEED_INIT (7)
#define PID_TURNTABLE_SPEED      (1000)	//���̵�� һ������
#define PID_TURNTABLE_CHECK_SPEED (3)

#define CHASSIS_SPEED_ATTENUATION   (1.0f)

#define CHASSIS_MOTOR_ROTATE_PID_DEFAULT \
{\
	0,\
	0,\
	{0,0,0},\
	CHASSIS_ROTATE_KP_DEFAULTS,\
	CHASSIS_ROTATE_KI_DEFAULTS,\
	CHASSIS_ROTATE_KD_DEFAULTS,\
	0,\
	0,\
	0,\
	4900,\
	1000,\
	1500,\
	0,\
	600,\
	0,\
	0,\
	0,\
	&PID_Calc,\
	&PID_Reset,\
}

//gimbal position pid control
//20  19
#define GIMBAL_MOTOR_PITCH_POSITION_PID_DEFAULT \
{\
	0,\
	0,\
	{0,0,0},\
	PITCH_POSITION_KP_DEFAULTS,\
	PITCH_POSITION_KI_DEFAULTS,\
	PITCH_POSITION_KD_DEFAULTS,\
	0,\
	0,\
	0,\
	4900,\
	1000,\
	1500,\
	0,\
	4900,\
	0,\
	0,\
	0,\
	&PID_Calc,\
	&PID_Reset,\
}

//gimbal speed pid control
#define GIMBAL_MOTOR_PITCH_SPEED_PID_DEFAULT \
{\
	0,\
	0,\
	{0,0,0},\
	PITCH_SPEED_KP_DEFAULTS,\
	PITCH_SPEED_KI_DEFAULTS,\
	PITCH_SPEED_KD_DEFAULTS,\
	0,\
	0,\
	0,\
	4900,\
	1000,\
	1500,\
	0,\
	30000,\
	0,\
	0,\
	0,\
	&PID_Calc,\
	&PID_Reset,\
}

//gimbal yaw position pid control
#define GIMBAL_MOTOR_YAW_POSITION_PID_DEFAULT \
{\
	0,\
	0,\
	{0,0,0},\
	YAW_POSITION_KP_DEFAULTS,\
	YAW_POSITION_KI_DEFAULTS,\
	YAW_POSITION_KD_DEFAULTS,\
	0,\
	0,\
	0,\
	4900,\
	1000,\
	1500,\
	0,\
	5000,\
	0,\
	0,\
	0,\
	&PID_Calc,\
	&PID_Reset,\
}

//gimbal yaw speed pid control
#define GIMBAL_MOTOR_YAW_SPEED_PID_DEFAULT \
{\
	0,\
	0,\
	{0,0,0},\
	YAW_SPEED_KP_DEFAULTS,\
	YAW_SPEED_KI_DEFAULTS,\
	YAW_SPEED_KD_DEFAULTS,\
	0,\
	0,\
	0,\
	4900,\
	1000,\
	1500,\
	0,\
	4900,\
	0,\
	0,\
	0,\
	&PID_Calc,\
	&PID_Reset,\
}

//D����ԭ��Ϊ0.4
#define CHASSIS_MOTOR_SPEED_PID_DEFAULT \
{\
	0,\
	0,\
	{0,0,0},\
	CHASSIS_SPEED_KP_DEFAULTS,\
	CHASSIS_SPEED_KI_DEFAULTS,\
	CHASSIS_SPEED_KD_DEFAULTS,\
	0,\
	0,\
	0,\
	4900,\
	1000,\
	1500,\
	0,\
	8000,\
	0,\
	0,\
	0,\
	&PID_Calc,\
	&PID_Reset,\
}

#define SHOOT_MOTOR_POSITION_PID_DEFAULT \
{\
	0,\
	0,\
	{0,0,0},\
	220.f,\
	0.0f,\
	0.0f,\
	0,\
	0,\
	0,\
	4900,\
	3500,\
	1500,\
	0,\
	4950,\
	0,\
	0,\
	0,\
	&PID_Calc,\
	&PID_Reset,\
}\

#define SHOOT_MOTOR_SPEED_PID_DEFAULT \
{\
	0,\
	0,\
	{0,0,0},\
	SHOOT_SPEED_KP_DEFAULTS,\
	SHOOT_SPEED_KI_DEFAULTS,\
	SHOOT_SPEED_KD_DEFAULTS,\
	0,\
	0,\
	0,\
	1000,\
	600,\
	100,\
	0,\
	13000,\
	0,\
	0,\
	0,\
	&PID_Calc,\
	&PID_Reset,\
}

#define PID_CALI_DEFAULT  \
{\
	0,\
	0,\
	0,\
	0,\
}

typedef struct PID_Regulator_t
{
	float ref;//��������������ֵ��
	float fdb;//feedback��������������ֵ��
	
	float err[3];//ƫ��
	
	float kp;
	float ki;
	float kd;
	
	float componentKp;//KP����
	float componentKi;
	float componentKd;
	float componentKpMax;  //������������
	float componentKiMax;  //��������
	float componentKdMax;  //΢������
	
	float output;
	float outputMax;
	
	float kp_offset;   //kp����
	float ki_offset;
	float kd_offset;
	
	void (*Calc)(struct PID_Regulator_t *pid);//����ָ�� //ָ��pid���㺯��
	void (*Reset)(struct PID_Regulator_t *pid);
	int deadband;
}PID_Regulator_t;

void PID_Reset(PID_Regulator_t *pid);
void PID_Calc(PID_Regulator_t *pid);
float PID_Task(PID_Regulator_t *PID_Stucture, float ref, float fdb);
float PID_Task_test(PID_Regulator_t *PID_Stucture, float ref, float fdb);
#endif
