/**
 * @brief  Frame structure processing
 * @param	Message frame structure processing of serial port and USB communication
 * @retval
 * @author ZCD
 * @Time 2019 12 10 
*/

#ifndef __Msg_Frame_
#define __Msg_Frame_

#include <stdint.h>
#include "usart.h"
#include "cmsis_os.h"
#include "CAN_BUS_Driver.h"
//******************************Frame head define*******************************//

//First frame of other processing units  ------ 0xAA
//First frame of other sensor unit  ----------- 0xBB

//Version communication
#define USB_Version_1 0xAA
#define USB_Version_2 0x55

//Another main control board
#define USART_Con_board_1 0xAA
#define USART_Con_board_2 0x11

//Host computer
#define USART_Host_computer_1 0xAA
#define USART_Host_computer_2 0x44		//这个帧头待定



void SendTo_ChassisTaskCreate(osPriority taskPriority);

//**********************Frame head structural morphology*************************//

//Content after frame parsing
//typedef struct 
//{
// int 



//}Frame_Content;
//	







//*****************************USB data structure********************************//
typedef struct{
	uint8_t Bool;						//Target detected
	float YAW_ChangeAngle;				//Vision data of yaw axis platform
	float PITCH_ChangeAngle;			//Vision data of pitch axis platform
	uint16_t Target_Distance;			//Vision

}USB_Decoding;							//USB_DATA_Decoding

//******************************External variables*******************************//
extern  USB_Decoding USB_Vision_Decoding;


//*****************************Function declaration******************************//
void VisionDataDecoding(void);
void VisionData_Transmit_USB(void);
void USB_TransmitTaskCreate(osPriority taskPriority);
HAL_StatusTypeDef USB_FrameCheck(uint8_t* USB_RxBuff);
//*******************************************************************************//



#endif

