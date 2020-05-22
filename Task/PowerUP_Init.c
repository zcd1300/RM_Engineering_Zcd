/**
 * @brief  Device power up init function
 * @param	
 * @retval
 * @author ZCD
 * @Time 2020 4 18
*/

#include "PowerUP_Init.h"
#include "StatusManagement.h"
#include "gpio.h" 
#include "StatusManagement.h"
#include "Flash_Operation.h"
#include "Motor_ConttrolTask.h" 


//GimbalOffset_SAVE Gimbal_Init={0,0,0};
uint8_t GimbalCalibrationState_ReadTemp [8];
/**
 * @brief  上电后LED操作，主要是用来检查有没有坏掉的LED，以及提示开机
 * @param	void
 * @retval	void
 * @author ZCD
 * @Time 2020 5 17
*/
void LED_GPIO_Init(void)
{
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOF,GPIO_PIN_14,GPIO_PIN_RESET);
	
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_1,GPIO_PIN_RESET);
	HAL_Delay(150);
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_2,GPIO_PIN_RESET);
	HAL_Delay(150);	
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_3,GPIO_PIN_RESET);
	HAL_Delay(150);
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_4,GPIO_PIN_RESET);
	HAL_Delay(150);	
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_5,GPIO_PIN_RESET);
	HAL_Delay(150);	
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_6,GPIO_PIN_RESET);
	HAL_Delay(150);
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_7,GPIO_PIN_RESET);
	HAL_Delay(150);
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_8,GPIO_PIN_RESET);
	HAL_Delay(150);
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8,GPIO_PIN_SET);	
	HAL_Delay(500);
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8,GPIO_PIN_RESET);	
	HAL_Delay(1000);
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8,GPIO_PIN_SET);	

	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET);	
}
/**
 * @brief  这个用来判断云台是否有被设置过初值，免得乱跑；
		   关于判断条件：1 读取flash里的标志、2 判断YAW和PITCH都为o，
		   虽然存在初值都是0的可能，但是绝大部分情况下都为0时是没有校准过的。
		   如果没有校准过，云台会一直执行校准模式！！！
 * @param	void
 * @retval	void
 * @author ZCD
 * @Time 2020 5 21 //单身狗日记（二）：天气 晴转大风转雷阵雨 ； 看看看，你们狗粮撒的太多了吧，老天都看不过去了。不说了工头叫我搬砖了。
*/

void GimbalCalibrationState_Judge(void)
{
	Flash_Read(Gimbal_Flash_SaveAddr,2,(uint32_t *)GimbalCalibrationState_ReadTemp);
	
}

/**
 * @brief  上电初始化函数，上电后调用一次就够了，别放进循环里。
 * @param	void
 * @retval	void
 * @author ZCD
 * @Time 2020 5 22 //可算熬过去前两天了，日记就不写了??
*/
void PowerUP_Init(void)
{
	LED_GPIO_Init();
	GimbalCalibrationState_Judge();


}

