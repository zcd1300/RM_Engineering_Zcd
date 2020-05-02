/**
 * @brief  State control
 * @param	State control
 * @author zcd
 * @Time 2020 5 1 
*/
#ifndef __StatusManagement_H
#define __StatusManagement_H

#include "stdint.h"

/*-------------------This part from 2019 engineering robot--------------*/
typedef enum
{
    PREPARE_STATE,          //�ϵ���ʼ״̬
    NORMAL_RC_STATE,        //ң��������״̬
    KEYBOARD_RC_STATE,      //���̿���״̬	
    STOP_STATE,             //ֹͣ״̬
}WorkState_e;	

typedef enum
{
    Stop_Mode,
    NormalRC_Mode,//RC--Remote Control
    KeyMouse_Mode,
    Auto_Mode,//�Զ�ģʽָ�����Զ�����ĳһ�˶�,��ȡ����
}OperateMode_e;

typedef enum  //������Զ�����״̬��ûȷ�����ȷ��Ű�
{
	Auto_NoMovement,
	Auto_Get_Bullet,
	Auto_Get_Bullets,
	Auto_Get_Bullets2,
	Auto_Get_Bulletss,
	Auto_Get_I_Bullet,	
	Auto_Get_I_Bullets,
	Auto_Give_Bullet,
	Auto_Cali_For_Bullet,
}AutoMovement_e;
/*----------------------------------------------------------------------*/

extern WorkState_e 	WorkState;
extern OperateMode_e OperateMode;


#endif


