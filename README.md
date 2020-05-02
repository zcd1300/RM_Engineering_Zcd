# RM_Engineering_Zcd
RoboMaster engineering



# 目录

* ##   [可能覆盖的部分](#index1)

* ## [尚未进行调试部分](index2)

* ## [代码留空部分](index3)

  









## 可能会被cube重新生成覆盖的部分{#index1} ##



* 串口：USART

* can通信：

* DMA：

* USB通信：

​	

## 还未调试的部分{#index2} ##

* 模糊pid效果以及参数：motocontrol.c\h 	Fuzzy.c\h

* 键鼠控制云台：![image-20200502213542347](C:\Users\lenovo\AppData\Roaming\Typora\typora-user-images\image-20200502213542347.png)

* 状态机自动控制部分状态：![image-20200502213634020](C:\Users\lenovo\AppData\Roaming\Typora\typora-user-images\image-20200502213634020.png)

## 因为条件限制暂时不能留空的代码部分{#index3} ##

~~~ c
void OperateMode_Select(void)
{
	switch(WorkState)
	{
		case PREPARE_STATE:
		{
			OperateMode = Stop_Mode;
		}break;
		case NORMAL_RC_STATE:
		{
			OperateMode = NormalRC_Mode;
			AutoMovement = Auto_NoMovement;
			
			if(RC_CtrlData.rc.switch_left == SWITCH_UP)
			{
				OperateMode = NormalRC_Mode;
				AutoMovement = Auto_NoMovement;
				//这里写一下左侧键在上右侧在上应该对应的初始化值啥的，
				//先空着，不知道等疫情开学后和他们联调的时候整。
				
			}
			if(RC_CtrlData.rc.switch_left == SWITCH_CENTRAL)
			{
				OperateMode = NormalRC_Mode;
				AutoMovement = Auto_NoMovement;	
				//这里同样空着，具体实现啥看需求（我可能会写成单纯控制运动）
				//
				
			}
			if(RC_CtrlData.rc.switch_left == SWITCH_DOWN)
			{
				OperateMode = NormalRC_Mode;
				AutoMovement = Auto_NoMovement;	
				//这可能做成电机停止运动，或者当遥控器Channel值大于600时手动触发自动控制
			
			
			}
			
		}break;
		case KEYBOARD_RC_STATE:
		{
		
		}break;
		case STOP_STATE:
		{
			OperateMode = Stop_Mode;
		}break;
		default:
		{
			OperateMode = Stop_Mode;
		}break;
	}
}
~~~



~~~ c
void State_Update(void)
{
	LastWorkState = WorkState;
	//遥控器错误停止
	if()
	{
		WorkState = STOP_STATE;
		return;
	}
	//云台、陀螺仪校准
	else if()
	{
	
		return;
	}
	switch(WorkState)
	{
		case PREPARE_STATE:
		{
		
		
		}break;
		case NORMAL_RC_STATE:
		{
		
		
		
		}break;
		case KEYBOARD_RC_STATE:
		{
		
		
		}break;
		case STOP_STATE:
		{
		
		}break;
		
	}
	WorkstateInit();
}
~~~

