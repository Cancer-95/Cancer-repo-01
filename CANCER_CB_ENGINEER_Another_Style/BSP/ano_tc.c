/*****************************************************************************************************************
 * @file 		  ano_tc.c
 * @version 	0.0
 * @date 		  2018.10.23
 *
 * @brief     ����һ��Cancer��Ϯһ�ݸ�����ߵ��γ��� ����������λ��д��
 *            ����ʹ���˴������ķ��Ͷ�(�����ж�)
 *						TX		PC6			DMA2_Stream1_ch5
 *  					RX		PC7			DMA2_Stream6_ch5 
 *            ʹ��DMA���д������ݵķ�������գ����ݿ����ǲ�������
 *            �������뷢�ͳ�����U6_TXMAX_len����
 *            �⵱����һ�����⣺ ���ղ���Ҫ��DMA�ж� ������Ҫ��DMA�жϣ�Ϊʲô������
 *****************************************************************************************************************/
#include "ano_tc.h"

u8 USART6_RX_buf[U6_RXMAX_len]={0};
u8 USART6_TX_buf[U6_TXMAX_len]={0};
u8 USART6_RX_updated = 0;
u8 USART6_RX_buf_LENTH = 0;

void ANO_TC_Init(void)	
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	// GPIOCʱ��	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE); //����6ʱ��ʹ��

//	USART_DeInit(USART3);
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6); 
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_High_Speed;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate=115200;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART6,&USART_InitStructure);		
	
	USART_ITConfig(USART6,USART_IT_IDLE,ENABLE);//���������ж�
	
//	NVIC_InitStructure.NVIC_IRQChannel=USART6_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=USART6_IRQn_pre;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=USART6_IRQn_sub;
//	NVIC_Init(&NVIC_InitStructure);
  NVIC_config(USART6_IRQn,USART6_IRQn_pre,USART6_IRQn_sub);

	USART_DMACmd(USART6,USART_DMAReq_Rx,ENABLE);
	USART_DMACmd(USART6,USART_DMAReq_Tx,ENABLE);
	USART_Cmd(USART6,ENABLE);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2ʱ��ʹ��

	DMA_DeInit(DMA2_Stream1);
	DMA_DeInit(DMA2_Stream6);
	
	DMA_InitStructure.DMA_Channel = DMA_Channel_5;  													//ͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(USART6->DR);						//DMA�����ַ
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)USART6_RX_buf;								//DMA�洢����ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;										//���ݴ��䷽ʽ
  DMA_InitStructure.DMA_BufferSize = U6_RXMAX_len;													//���ݴ�����
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;					//���������ģʽ
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;										//�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//�������ݳ��ȣ�8λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;						//�洢�����ݳ��ȣ�8λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;															//ʹ����ͨģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;										//������ȼ�
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                    //
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;             //
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;               //�洢��ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;				//����ͻ�����δ���
	DMA_Init(DMA2_Stream1,&DMA_InitStructure);
	
	DMA_InitStructure.DMA_Channel = DMA_Channel_5;  													//ͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(USART6->DR);						//DMA�����ַ
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)USART6_TX_buf;								//DMA�洢����ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;										//���ݴ��䷽ʽ
  DMA_InitStructure.DMA_BufferSize = U6_TXMAX_len;													//���ݴ�����
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;					//���������ģʽ
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;										//�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//�������ݳ��ȣ�8λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;						//�洢�����ݳ��ȣ�8λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;															//ʹ����ͨģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;										//������ȼ�
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                    //
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;             //
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;               //�洢��ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;				//����ͻ�����δ���
	DMA_Init(DMA2_Stream6,&DMA_InitStructure);
	
//	NVIC_InitStructure.NVIC_IRQChannel=DMA2_Stream6_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=DMA2_Stream6_IRQn_pre;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=DMA2_Stream6_IRQn_sub;
//	NVIC_Init(&NVIC_InitStructure);
	NVIC_config(DMA2_Stream6_IRQn,DMA2_Stream6_IRQn_pre,DMA2_Stream6_IRQn_sub);//���Ϳ�DMA�жϣ���

//	DMA_ITConfig(DMA2_Stream1,DMA_IT_TC,ENABLE);
	DMA_ITConfig(DMA2_Stream6,DMA_IT_TC,ENABLE);

	DMA_Cmd(DMA2_Stream1,ENABLE);
	DMA_Cmd(DMA2_Stream6,ENABLE);
}

//�������ݸ���������վ
void ANO_TC_Report(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw)
{
	u8 i;
	DMA_Cmd(DMA2_Stream6, DISABLE);                              //�ر�DMA����
//	memset(USART6_TX_buf,0,23);
	USART6_TX_buf[0]=0XAA;															 				 //֡ͷ
	USART6_TX_buf[1]=0XAA;															 				 //֡ͷ
	USART6_TX_buf[2]=0XF1;															 				 //������
	USART6_TX_buf[3]=18;																 				 //���ݳ���
	
	USART6_TX_buf[4]=(aacx>>8)&0XFF;
	USART6_TX_buf[5]=aacx&0XFF;
	USART6_TX_buf[6]=(aacy>>8)&0XFF;
	USART6_TX_buf[7]=aacy&0XFF;
	USART6_TX_buf[8]=(aacz>>8)&0XFF;
	USART6_TX_buf[9]=aacz&0XFF; 
	USART6_TX_buf[10]=(gyrox>>8)&0XFF;
	USART6_TX_buf[11]=gyrox&0XFF;
	USART6_TX_buf[12]=(gyroy>>8)&0XFF;
	USART6_TX_buf[13]=gyroy&0XFF;
	USART6_TX_buf[14]=(gyroz>>8)&0XFF;
	USART6_TX_buf[15]=gyroz&0XFF;	
	USART6_TX_buf[16]=(roll>>8)&0XFF;
	USART6_TX_buf[17]=roll&0XFF;
	USART6_TX_buf[18]=(pitch>>8)&0XFF;
	USART6_TX_buf[19]=pitch&0XFF;
	USART6_TX_buf[20]=(yaw>>8)&0XFF;
	USART6_TX_buf[21]=yaw&0XFF;
	USART6_TX_buf[22]=0;																					//У��������
	for(i=0;i<22;i++)USART6_TX_buf[22]+=USART6_TX_buf[i];					//����У���
	
	while (DMA_GetCmdStatus(DMA2_Stream6) != DISABLE){}		        //ȷ��DMA���Ա�����
	DMA_SetCurrDataCounter(DMA2_Stream6,23);          		        //���ݴ�����
	DMA_Cmd(DMA2_Stream6, ENABLE);                      	        //����DMA����
}

void ANO_TC_Report16(short test)
{
	u8 i;
	DMA_Cmd(DMA2_Stream6, DISABLE);                              //�ر�DMA����
//	memset(USART6_TX_buf,0,7);
	USART6_TX_buf[0]=0XAA;															 				 //֡ͷ
	USART6_TX_buf[1]=0XAA;															 				 //֡ͷ
	USART6_TX_buf[2]=0XF1;															 				 //������
	USART6_TX_buf[3]=2;																 				   //���ݳ���
	
	USART6_TX_buf[4]=(test>>8)&0XFF;
	USART6_TX_buf[5]=test&0XFF;
	USART6_TX_buf[6]=0;																			    	//У��������
	for(i=0;i<6;i++)USART6_TX_buf[6]+=USART6_TX_buf[i];				  	//����У���
	
	while (DMA_GetCmdStatus(DMA2_Stream6) != DISABLE){}		        //ȷ��DMA���Ա�����
	DMA_SetCurrDataCounter(DMA2_Stream6,7);          		          //���ݴ�����
	DMA_Cmd(DMA2_Stream6, ENABLE);                      	        //����DMA����
}

void ANO_TC_Report32(int32_t test)
{
	u8 i;
	DMA_Cmd(DMA2_Stream6, DISABLE);                                //�ر�DMA����
//	memset(USART6_TX_buf,0,7);
	USART6_TX_buf[0]=0XAA;															 			  	 //֡ͷ
	USART6_TX_buf[1]=0XAA;															 			  	 //֡ͷ
	USART6_TX_buf[2]=0XF1;															 			  	 //������
	USART6_TX_buf[3]=4;																 				     //���ݳ���
	
	USART6_TX_buf[4]=(test>>24)&0XFF;
  USART6_TX_buf[5]=(test>>16)&0XFF;
	USART6_TX_buf[6]=(test>>8)&0XFF;
	USART6_TX_buf[7]=test&0XFF;
	USART6_TX_buf[8]=0;																				      //У��������
	for(i=0;i<8;i++)USART6_TX_buf[8]+=USART6_TX_buf[i];				    	//����У���
	
	while (DMA_GetCmdStatus(DMA2_Stream6) != DISABLE){}		          //ȷ��DMA���Ա�����
	DMA_SetCurrDataCounter(DMA2_Stream6,9);          		            //���ݴ�����
	DMA_Cmd(DMA2_Stream6, ENABLE);                                	//����DMA����
}

void USART6_IRQHandler(void)
{
	if(USART_GetITStatus(USART6,USART_IT_IDLE) != RESET) 	            //�����ж��ж������Ƿ�ͨ�����ڴ������
	{
		DMA_Cmd(DMA2_Stream1, DISABLE); 										            //�ر�DMA����ֹ�����ڼ�������
		USART6_RX_updated = 1;															         		//��1��ʾһ�ν����������
		USART6->SR;  
		USART6->DR;  																						        //��SR�ٶ�DR ���IDLE
		USART6_RX_buf_LENTH =U6_RXMAX_len - DMA2_Stream1->NDTR;				  //��ô��ڷ��͹������ַ��ĳ���//DMAʣ��
		
		memset(USART6_RX_buf+USART6_RX_buf_LENTH,0,DMA2_Stream1->NDTR);	//�����ȫ����//���¸��ǽ�������//�����׵�ַ  ��һ�仰Ҳ����ֱ��Init
		
		DMA_ClearFlag(DMA2_Stream1, DMA_IT_TCIF1);
		DMA2_Stream1->NDTR = U6_RXMAX_len;		  								        //����д�봫����
		DMA_Cmd(DMA2_Stream1, ENABLE);     									            //���´� 
	}  
}

//void DMA2_Stream1_IRQHandler(void)											        //���ܵ��ж�DMA
//{
//	if(DMA_GetITStatus(DMA2_Stream1, DMA_IT_TCIF1))
//	{
//		DMA_ClearFlag(DMA2_Stream1, DMA_IT_TCIF1);
//		DMA_ClearITPendingBit(DMA2_Stream1, DMA_IT_TCIF1);
//	}
//}

void DMA2_Stream6_IRQHandler(void)										              	//���ڷ��͵�DMA
{
	if(DMA_GetITStatus(DMA2_Stream6, DMA_IT_TCIF6))
	{
		DMA_ClearFlag(DMA2_Stream6, DMA_IT_TCIF6);
		DMA_ClearITPendingBit(DMA2_Stream6, DMA_IT_TCIF6);
	}
}
