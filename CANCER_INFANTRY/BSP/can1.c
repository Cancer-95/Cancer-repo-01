#include "can1.h"

int16_t Motor_chassis[4][2],Motor_angle[2];

CanRxMsg CAN1_RxMessage;
CanTxMsg CAN1_TxMessage;

void CAN1_Init()
{
	CAN_InitTypeDef        can;
	CAN_FilterInitTypeDef  can_filter;
	GPIO_InitTypeDef          gpio;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	
	gpio.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_11;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&gpio);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_CAN1);
	
	nvic_config(CAN1_RX0_IRQn,CAN1_RX0_IRQn_pre,CAN1_RX0_IRQn_sub);
	
	CAN_DeInit(CAN1);
	CAN_StructInit(&can);
	
	can.CAN_TTCM = DISABLE;		//
	can.CAN_ABOM = DISABLE;		//
	can.CAN_AWUM = DISABLE;		//
	can.CAN_NART = DISABLE;		//
	can.CAN_RFLM = DISABLE;		//
	can.CAN_TXFP = ENABLE;		//
	can.CAN_Mode = CAN_Mode_Normal;

	can.CAN_SJW  = CAN_SJW_1tq;
	can.CAN_BS1 = CAN_BS1_9tq;
	can.CAN_BS2 = CAN_BS2_4tq;
	can.CAN_Prescaler = 3;   //CAN BaudRate 42/(1+9+4)/3=1Mbps
	CAN_Init(CAN1, &can);

	can_filter.CAN_FilterNumber=0;
	can_filter.CAN_FilterMode=CAN_FilterMode_IdMask;
	can_filter.CAN_FilterScale=CAN_FilterScale_32bit;
	can_filter.CAN_FilterIdHigh=0x0000;
	can_filter.CAN_FilterIdLow=0x0000;
	can_filter.CAN_FilterMaskIdHigh=0x0000;
	can_filter.CAN_FilterMaskIdLow=0x0000;
	can_filter.CAN_FilterFIFOAssignment=0;//the message which pass the filter save in fifo0
	can_filter.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&can_filter);
	
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
}

void CAN1_Send(uint32_t Equipment_ID,int16_t Data0,int16_t Data1,int16_t Data2,int16_t Data3)		//���̵��
{		
	
	CAN1_TxMessage.StdId = Equipment_ID;			//ʹ�õ���չID�����820R��ʶ��0X200
	CAN1_TxMessage.IDE = CAN_ID_STD;				 //��׼ģʽ
	CAN1_TxMessage.RTR = CAN_RTR_DATA;			 //����֡RTR=0��Զ��֡RTR=1
	CAN1_TxMessage.DLC = 8;							 	 //���ݳ���Ϊ8�ֽ�

	CAN1_TxMessage.Data[0] = Data0>>8; 
	CAN1_TxMessage.Data[1] = Data0;
	CAN1_TxMessage.Data[2] = Data1>>8; 
	CAN1_TxMessage.Data[3] = Data1;
	CAN1_TxMessage.Data[4] = Data2>>8; 
	CAN1_TxMessage.Data[5] = Data2;
	CAN1_TxMessage.Data[6] = Data3>>8; 
	CAN1_TxMessage.Data[7] = Data3;

	CAN_Transmit(CAN1, &CAN1_TxMessage);	//��������
}

void CAN1_RX0_IRQHandler()
{
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0)!=RESET)
	{
		CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);		//����жϹ���
		CAN_Receive(CAN1, CAN_FIFO0, &CAN1_RxMessage);		//����can����
		
		if(CAN1_RxMessage.StdId == 0x201)//��ǰ
		{    
				Motor_chassis[M1][0] = (int16_t)(CAN1_RxMessage.Data[0]<<8 | CAN1_RxMessage.Data[1]);		//��е�Ƕ�	0~8191(0x1FFF)
				Motor_chassis[M1][1] = (int16_t)(CAN1_RxMessage.Data[2]<<8 | CAN1_RxMessage.Data[3]);		//���ת��
			
		    chassis.wheel_motor_angle[M1] =	Motor_chassis[M1][0];
		    chassis.wheel_speed_fdb[M1]	= Motor_chassis[M1][1]; 
		}
		else if(CAN1_RxMessage.StdId == 0x202)//��ǰ		
		{      
				Motor_chassis[M2][0] = (int16_t)(CAN1_RxMessage.Data[0]<<8 | CAN1_RxMessage.Data[1]);		//��е�Ƕ�	0~8191(0x1FFF)
				Motor_chassis[M2][1] = (int16_t)(CAN1_RxMessage.Data[2]<<8 | CAN1_RxMessage.Data[3]);		//���ת��	

			  chassis.wheel_motor_angle[M2] = Motor_chassis[M2][0];
				chassis.wheel_speed_fdb[M2] = Motor_chassis[M2][1];
		}
		else if(CAN1_RxMessage.StdId == 0x203)//�Һ�
		{     
				Motor_chassis[M3][0] = (int16_t)(CAN1_RxMessage.Data[0]<<8 | CAN1_RxMessage.Data[1]);		//��е�Ƕ�	0~8191(0x1FFF)
				Motor_chassis[M3][1] = (int16_t)(CAN1_RxMessage.Data[2]<<8 | CAN1_RxMessage.Data[3]);		//���ת��
			
			  chassis.wheel_motor_angle[M3] = Motor_chassis[M3][0];
		  	chassis.wheel_speed_fdb[M3] = Motor_chassis[M3][1];
		}
		else if(CAN1_RxMessage.StdId == 0x204)//���
		{   
				Motor_chassis[M4][0] = (int16_t)(CAN1_RxMessage.Data[0]<<8 | CAN1_RxMessage.Data[1]);		//��е�Ƕ�	0~8191(0x1FFF)
				Motor_chassis[M4][1] = (int16_t)(CAN1_RxMessage.Data[2]<<8 | CAN1_RxMessage.Data[3]);		//���ת��

		  	chassis.wheel_motor_angle[M4] = Motor_chassis[M4][0];
		  	chassis.wheel_speed_fdb[M4] = Motor_chassis[M4][1];		
		}
		
		else if(CAN1_RxMessage.StdId == 0x205)			//YAW
		{
				Motor_angle[YAW] = (int16_t)(CAN1_RxMessage.Data[0]<<8|CAN1_RxMessage.Data[1]);	//��е�Ƕ�	0~8191(0x1FFF) ʵ�ʽǶ���ΪPID��������feedback
		}	
		
		else if(CAN1_RxMessage.StdId == 0x206)			//Pitch
		{
			  Motor_angle[PIT] = (int16_t)(CAN1_RxMessage.Data[0]<<8|CAN1_RxMessage.Data[1]); //��е�Ƕ�	0~8191(0x1FFF) ʵ�ʽǶ���ΪPID��������feedback
		}

  }
}

