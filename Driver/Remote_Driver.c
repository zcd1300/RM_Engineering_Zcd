/**
 * @brief  Remote controler 
 * @param	It is necessary to pay attention to key chattering, key jumping and ramp function
 * @author zcd
 * @Time 2020 1 6 
*/

#include "Remote_Driver.h"
#include "cmsis_os.h"
//uint8_t Remote_RxBuff[18]={0};	


InputMode_e	InputMode;
Remote_Control RC_CtrlData;



uint8_t Switch_Store = 0;


//这个函数到之后也没用到过了

uint8_t stick1_change(void)
{		
	Switch_Store<<= 2;
	Switch_Store |= RC_CtrlData.rc.switch_left;

		if(((Switch_Store>>2)&0x03) != RC_CtrlData.rc.switch_left)  //这里把00000011改成0x03之后就可以了	
		{		
			return 1;
		}
		else
		{ 
			Switch_Store >>= 2;
			return 0;
		}
}

uint32_t timetick2 = 0;
uint32_t timetick1 = 0;
uint32_t time_diff_1to3 = 0;
uint32_t time_diff_1to2 = 0;
uint8_t S_switch = 0;

uint8_t Action_stick = 0;//stick0 的三次变化值及最早恒定值

uint8_t stick1_action(void)
{
	S_switch = stick1_change();
	if(S_switch != 0)
	{
	 
		time_diff_1to3 = xTaskGetTickCount() - timetick2;
		time_diff_1to2 = xTaskGetTickCount() - timetick1;
		
		
		timetick2 = timetick1;
		timetick1 = xTaskGetTickCount();

		if(time_diff_1to2<NORMAL_TIME_2)
		{
			if(time_diff_1to3<NORMAL_TIME_3)
			{
				
				Action_stick = Switch_Store;
				return THREE_CHANGE;//变三次一定会变两次，三次变换多而且也不难记，还可以改变拨杆位置，所以用变3次之后得到的8位数作为拨杆命令
			}
			return TWO_CHANGE;
		}
	 else 
	 {
		 
			return ONE_CHANGE;
	 }
	}
	else 
//		  abc=xTaskGetTickCount() - timetick1;
			if((xTaskGetTickCount() - timetick1)>NORMAL_TIME_2)
			{
				Switch_Store = RC_CtrlData.rc.switch_left;//若一定时间内未检测到再次跳变，则只变了一次，保存值清空
			}
			return NO_CHANGE;
		
	
}


/**
  * @brief  判断一个键盘键是否发生按下跳变
  * @param  要进行判断的按键，字符大写
  * @retval 1 按下        0 未按下
  */
uint8_t Remote_CheckJumpKey(uint16_t Key_Temp)
{
    if(RC_CtrlData.key.Key & Key_Temp)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


void RemoteDataProcess(uint8_t *pData)
{
    if(pData == NULL)
    {
        return;
    }
    RC_CtrlData.rc.Channel_1 = ((int16_t)pData[0] | ((int16_t)pData[1] << 8)) & 0x07FF; 
    RC_CtrlData.rc.Channel_2 = (((int16_t)pData[1] >> 3) | ((int16_t)pData[2] << 5)) & 0x07FF;
    RC_CtrlData.rc.Channel_3 = (((int16_t)pData[2] >> 6) | ((int16_t)pData[3] << 2) |
                         ((int16_t)pData[4] << 10)) & 0x07FF;
    RC_CtrlData.rc.Channel_4 = (((int16_t)pData[4] >> 1) | ((int16_t)pData[5]<<7)) & 0x07FF;
	
	RC_CtrlData.rc.Channel_1 -= (int16_t)REMOTE_CONTROLLER_STICK_OFFSET;
	RC_CtrlData.rc.Channel_2 -= (int16_t)REMOTE_CONTROLLER_STICK_OFFSET;
	RC_CtrlData.rc.Channel_3 -= (int16_t)REMOTE_CONTROLLER_STICK_OFFSET;
	RC_CtrlData.rc.Channel_4 -= (int16_t)REMOTE_CONTROLLER_STICK_OFFSET;
    
    RC_CtrlData.rc.switch_left = ((pData[5] >> 4) & 0x000C) >> 2;
    RC_CtrlData.rc.switch_right = ((pData[5] >> 4) & 0x0003);

    RC_CtrlData.mouse.x = ((int16_t)pData[6]) | ((int16_t)pData[7] << 8);
    RC_CtrlData.mouse.y = ((int16_t)pData[8]) | ((int16_t)pData[9] << 8);
    RC_CtrlData.mouse.z = ((int16_t)pData[10]) | ((int16_t)pData[11] << 8);    

    RC_CtrlData.mouse.press_l = pData[12];
    RC_CtrlData.mouse.press_r = pData[13];
 
    RC_CtrlData.key.Key = ((int16_t)pData[14]) | ((int16_t)pData[15] << 8);
		stick1_action();
}
