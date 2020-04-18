/**
 * @brief  State control
 * @param	State control
 * @author zcd
 * @Time 2020 1 5 
*/
#ifndef __StatusManagement_H
#define __StatusManagement_H

#include "stdint.h"

/*-------------------This part from 2019 engineering robot--------------*/
typedef enum
{
    PREPARE_STATE,          //上电后初始状态
    NORMAL_RC_STATE,        //遥控器控制状态
    KEYBOARD_RC_STATE,      //键盘控制状态
    STOP_STATE,             //停止状态
}WorkState_e;	

typedef enum
{
    Stop_Mode,
    NormalRC_Mode,//RC--Remote Control
    KeyMouse_Mode,
    Auto_Mode,//自动模式指的是自动进行某一运动,如取弹等
}OperateMode_e;

/*----------------------------------------------------------------------*/

extern WorkState_e 	WorkState;
extern OperateMode_e OperateMode;


#endif


