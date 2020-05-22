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
 * @brief  �ϵ��LED��������Ҫ�����������û�л�����LED���Լ���ʾ����
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
 * @brief  ��������ж���̨�Ƿ��б����ù���ֵ��������ܣ�
		   �����ж�������1 ��ȡflash��ı�־��2 �ж�YAW��PITCH��Ϊo��
		   ��Ȼ���ڳ�ֵ����0�Ŀ��ܣ����Ǿ��󲿷�����¶�Ϊ0ʱ��û��У׼���ġ�
		   ���û��У׼������̨��һֱִ��У׼ģʽ������
 * @param	void
 * @retval	void
 * @author ZCD
 * @Time 2020 5 21 //�����ռǣ����������� ��ת���ת������ �� �����������ǹ�������̫���˰ɣ����춼������ȥ�ˡ���˵�˹�ͷ���Ұ�ש�ˡ�
*/

void GimbalCalibrationState_Judge(void)
{
	Flash_Read(Gimbal_Flash_SaveAddr,2,(uint32_t *)GimbalCalibrationState_ReadTemp);
	
}

/**
 * @brief  �ϵ��ʼ���������ϵ�����һ�ξ͹��ˣ���Ž�ѭ���
 * @param	void
 * @retval	void
 * @author ZCD
 * @Time 2020 5 22 //���㰾��ȥǰ�����ˣ��ռǾͲ�д��??
*/
void PowerUP_Init(void)
{
	LED_GPIO_Init();
	GimbalCalibrationState_Judge();


}

