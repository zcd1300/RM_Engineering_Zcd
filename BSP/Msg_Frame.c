/**
 * @brief  Frame structure processing
 * @param	Message frame structure processing of serial port and USB communication
 * @retval
 * @author ZCD
 * @Time 2019 12 10 
*/

#include "XDU_USB_HID_Z.h"
#include "CAN_BUS_Driver.h"
#include "usart.h"
#include "cmsis_os.h"
#include "Msg_Frame.h"
#include "cmsis_os.h" 
#include "Motor_ConttrolTask.h"

float Temp_Yaw=0;
float Temp_Pitch=0;
uint8_t Tx_SendTemp_USB[64]={0};
uint8_t Tx_temp_USB[4]={0};

osThreadId USB_TransmitTaskHandle;
osThreadId SendTo_ChassisTaskHandle;

USB_Decoding USB_Vision_Decoding={0,0,0};

/**
 * @brief USB Frame structure Check
 * @param	Message frame structure processing of serial port and USB communication
 * @retval
 * @ZCD
 * @Time 2019 12 10 
*/
HAL_StatusTypeDef USB_FrameCheck(uint8_t* USB_RxBuff)
{
	switch (USB_RxBuff[0])
	{
		case 0xAA:
		{
			if(USB_RxBuff[1]==USB_Version_2)
			{
				VisionDataDecoding();
			}
					
			break;
		}

		default :
		{
			return HAL_ERROR;
		}
	}


	return HAL_OK;
	
}

/**
 * @brief Decoding and sorting of received data
 * @param	The received data is decoded, classified and stored in the structure.
 * @author ZCD
 * @Time 2020 1 5 
*/
void VisionDataDecoding(void)
{		
    USB_Vision_Decoding.Bool=USB_Rx_DataBuf[2];
	USB_Vision_Decoding.YAW_ChangeAngle=((int16_t)(USB_Rx_DataBuf[4] << 8) | (USB_Rx_DataBuf[3]))/100.0f;
	USB_Vision_Decoding.PITCH_ChangeAngle=((int16_t)(USB_Rx_DataBuf[6] << 8) | (USB_Rx_DataBuf[5]))/100.0f;
	USB_Vision_Decoding.Target_Distance=USB_Rx_DataBuf[7];
}

/**
 * @brief USB Send
 * @param void
 * @retval void
 * @Time 2020 1 5
*/
void VisionData_Transmit_USB(void)
{
	Tx_temp_USB[0]=((int16_t)((YAW_GM6020Encoder.ecd_angle+165)*100))>>8;
	Tx_temp_USB[1]=(int16_t)((YAW_GM6020Encoder.ecd_angle+165)*100);	
	Tx_temp_USB[2]=((int16_t)((PITCH_GM6020Encoder.ecd_angle-160)*100))>>8;
	Tx_temp_USB[3]=(int16_t)((PITCH_GM6020Encoder.ecd_angle-160)*100);	
	
	
	Tx_SendTemp_USB[0]=0xAA;
	Tx_SendTemp_USB[1]=0x55;
	
	Tx_SendTemp_USB[2]=Tx_temp_USB[0];
	Tx_SendTemp_USB[3]=Tx_temp_USB[1];
	
	Tx_SendTemp_USB[4]=Tx_temp_USB[2];
	Tx_SendTemp_USB[5]=Tx_temp_USB[3];
	
	Tx_SendTemp_USB[6]=0xAB;
	
	usb_hid_Tx(Tx_SendTemp_USB);
	
}

/**
 * @brief Communication with chassis
 * @param EnCoder * ToSend
 * @retval void
 * @Time 2020 4 14
*/
void SendTo_Chassis(void)
{
	CAN2_Tx_Buff_Ext[0]= YAW_GM6020Encoder.ecd_value >>24;
	CAN2_Tx_Buff_Ext[1]= YAW_GM6020Encoder.ecd_value >>16;
	CAN2_Tx_Buff_Ext[2]= YAW_GM6020Encoder.ecd_value >>8;
	CAN2_Tx_Buff_Ext[3]= YAW_GM6020Encoder.ecd_value ;
	CAN2_Tx_Buff_Ext[4]= (int16_t)(YAW_GM6020Encoder.ecd_angle*100) >>8;
	CAN2_Tx_Buff_Ext[5]= (int16_t)(YAW_GM6020Encoder.ecd_angle*100) ;
	CAN2_Tx_Buff_Ext[6]= (int16_t)(YAW_GM6020Encoder.ecd_angle*100) >>8;
	CAN2_Tx_Buff_Ext[7]= (int16_t)(YAW_GM6020Encoder.ecd_angle*100) ;
	
	CAN2_Send(CAN2_Tx_Buff_Ext,0x300);
}


void USB_TransmitTask(void const* argument)
{

	for(;;)
	{
		VisionData_Transmit_USB();
		osDelay(10/portTICK_RATE_MS);
	}

}

void USB_TransmitTaskCreate(osPriority taskPriority)
{
	osThreadDef(USB_TransmitThread,USB_TransmitTask,taskPriority,0,256);
	USB_TransmitTaskHandle=osThreadCreate(osThread(USB_TransmitThread),NULL);

}
void SendTo_ChassisTask(void const* argument)
{

	for(;;)
	{
		SendTo_Chassis();
		osDelay(10/portTICK_RATE_MS);
	}

}

void SendTo_ChassisTaskCreate(osPriority taskPriority)
{
	osThreadDef(SendTo_ChassisThread,SendTo_ChassisTask,taskPriority,0,256);
	SendTo_ChassisTaskHandle=osThreadCreate(osThread(SendTo_ChassisThread),NULL);
	
}


