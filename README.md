# RM_Engineering_Zcd
RoboMaster engineering



# 目录

* ##   [可能覆盖的部分](#可能会被cube重新生成覆盖的部分)

* ## [尚未进行调试部分](#还未调试的部分)

* ## [代码留空部分](#因为条件限制暂时不能留空的代码部分)

* ## [已知的逻辑冲突、错误](#已知的逻辑冲突、错误)

* ## [报错标志对照](# 报错标志对照)

  
  
  
  
  
  
  
  
  
  
  







## 可能会被cube重新生成覆盖的部分 ##



* 串口：USART
* can通信：
* DMA：
* USB通信：
* stm32f4xx_it.c

​	

## 还未调试的部分 ##

* 模糊pid效果以及参数：motocontrol.c\h 	Fuzzy.c\h

* 键鼠控制云台：![image-20200502213542347](C:\Users\lenovo\AppData\Roaming\Typora\typora-user-images\image-20200502213542347.png)

* 状态机自动控制部分状态：![image-20200502213634020](C:\Users\lenovo\AppData\Roaming\Typora\typora-user-images\image-20200502213634020.png)

## 因为条件限制暂时不能留空的代码部分 ##

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
				
				if(RC_CtrlData.rc.Channel_1 >600)
				{
					AutoMovement = Auto_Get_Bullet;
				}
				//其他通道同理，通过if判断即可
			
			}	
		}break;
		case KEYBOARD_RC_STATE:
		{
			OperateMode = KeyMouse_Mode;
			if(AutoMovement != Auto_NoMovement)//其实这些东西可以考虑加在mode下
			{
			//不变
			}
			/********以上是保持动作不变**********/
			
			//检测按键按下进行状态切换以及参数设定
			else if(Remote_CheckJumpKey(KEY_Z))
			{
				AutoMovement = Auto_Give_Bullet;
			}
			//其他按键同理
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
void StatusMachine_Init(void)
static void WorkstateInit(void)
void State_Update(void)
void OperateMode_Select(void)
~~~

* **Spuervise还没有对应的错误显示函数（原来的是用蜂鸣器）**

## 已知的逻辑冲突、错误

* 底盘解算时高速模式切换条件和云台发弹**可能**存在冲突

## 报错标志对照

* 正常工作：	复位键下方绿灯常亮

* 云台未校准： 复位键下方红绿LED同时闪烁

* 云台校准模式：	复位键下方红绿LED交替闪烁

  *（从上到下依次为高到低位，最高的对应bit8；错误检测为线性检测，即上一个错误排除之前不会继续执行检测）*

| 左侧LED（从上到下，红色LED常亮） |    对应错误类型     |
| :------------------------------: | :-----------------: |
|            0000 0000             |      工作正常       |
|            0000 0001             |     遥控器掉线      |
|            0000 0010             |     YAW电机掉线     |
|            0000 0100             |    PITCH电机掉线    |
|            0000 1000             |   左侧摩擦轮掉线    |
|            0001 0000             |   右侧摩擦轮掉线    |
|            0010 0000             |    拨盘电机掉线     |
|            0100 0000             | 妙算USB接收帧率异常 |
|            1000 0000             | 妙算USB发送帧率异常 |
|            0000 0011             |     IMU数据异常     |
|            0000 0101             |    CAN1接收异常     |
|            0000 1001             |    CAN2接收异常     |
|            0001 0001             |    CAN1发送异常     |
|            0010 0001             |    CAN2发送异常     |
|            0100 0001             |        留空         |
|            1000 0000             |        留空         |
