#include "dma.h"

void DMA_Single_Memory_Config(DMA_Stream_TypeDef *DMA_Streamx,
															u32                chx,
															u32                PeripheralBaseAddr,
															u32                MemoryBaseAddr,
															u16                buf_len,
															u32                direction)
{ 
	DMA_InitTypeDef  DMA_InitStructure;
	
	if((u32)DMA_Streamx>(u32)DMA2)																						//�õ���ǰstream����DMA����DMA1
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);											//DMA2ʱ��ʹ��
	else 
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);											//DMA1ʱ��ʹ��
				
  DMA_DeInit(DMA_Streamx);
	DMA_StructInit(&DMA_InitStructure);
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}												//�ȴ�DMA������

	DMA_InitStructure.DMA_Channel            = chx;  																		//ͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr = PeripheralBaseAddr;						//DMA�����ַ
  DMA_InitStructure.DMA_Memory0BaseAddr    = MemoryBaseAddr;										//DMA�洢����ַ
  DMA_InitStructure.DMA_DIR                = direction;																		//���ݴ��䷽��
  DMA_InitStructure.DMA_BufferSize         = buf_len;																//���ݴ�����
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;					//���������ģʽ
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;										//�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//�������ݳ��ȣ�8λ
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;						//�洢�����ݳ��ȣ�8λ
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;															//ʹ�û���ģʽ
	DMA_InitStructure.DMA_Priority           = DMA_Priority_VeryHigh;										//������ȼ�
  DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_1QuarterFull;
  DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;								//�洢��ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;				//����ͻ�����δ���
			
  DMA_Init(DMA_Streamx, &DMA_InitStructure);	
	
	DMA_Cmd(DMA_Streamx, ENABLE);
} 

void DMA_Double_Memory_Config(DMA_Stream_TypeDef *DMA_Streamx,
															u32                chx,
															u32                PeripheralBaseAddr,
															u32                Memory0BaseAddr,//Memory0
                              u32                Memory1BaseAddr,//Memory1
															u16                buf_len,
															u32                direction)
{ 
	DMA_InitTypeDef  DMA_InitStructure;
	
	if((u32)DMA_Streamx>(u32)DMA2)																						//�õ���ǰstream����DMA����DMA1
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);											//DMA2ʱ��ʹ��
	else 
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);											//DMA1ʱ��ʹ��
				
  DMA_DeInit(DMA_Streamx);
	DMA_StructInit(&DMA_InitStructure);
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}												//�ȴ�DMA������

	DMA_InitStructure.DMA_Channel            = chx;  																		//ͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr = PeripheralBaseAddr;						//DMA�����ַ
  DMA_InitStructure.DMA_Memory0BaseAddr    = Memory0BaseAddr;										//DMA�洢����ַ
  DMA_InitStructure.DMA_DIR                = direction;																		//���ݴ��䷽��
  DMA_InitStructure.DMA_BufferSize         = buf_len;																//���ݴ�����
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;					//���������ģʽ
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;										//�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//�������ݳ��ȣ�8λ
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;						//�洢�����ݳ��ȣ�8λ
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;															//ʹ�û���ģʽ
	DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;										//�е����ȼ�
  DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_1QuarterFull;
  DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;								//�洢��ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;				//����ͻ�����δ���
			
  DMA_Init(DMA_Streamx, &DMA_InitStructure);	
	
	//����Memory1,Memory0�ǵ�һ��ʹ�õ�Memory
	DMA_DoubleBufferModeConfig(DMA_Streamx, Memory1BaseAddr, DMA_Memory_0);   //first used memory configuration
  DMA_DoubleBufferModeCmd(DMA_Streamx, ENABLE);
	
	DMA_Cmd(DMA_Streamx, ENABLE);
} 


void RC_DMA_init(void)//usart2
{
	DMA_Double_Memory_Config(RC_USART_RX_DMA_Stream,
													 USART_DMA_Channel,
													 (u32)&(USART2->DR),
													 (u32)&_USART2_DMA_RX_BUF[0][0],
	                         (u32)&_USART2_DMA_RX_BUF[1][0],
								           sizeof(_USART2_DMA_RX_BUF)/2,
													 DMA_DIR_PeripheralToMemory);
}

void judgement_DMA_init(void)//uart5
{
	DMA_Single_Memory_Config(UART5_RX_DMA_Stream,
													 USART_DMA_Channel,
													 (u32)&(UART5->DR),
													 (u32)&_UART5_DMA_RX_BUF[0],
								           sizeof(_UART5_DMA_RX_BUF),
													 DMA_DIR_PeripheralToMemory);
}

void vision_DMA_init(void)//uart4
{
	DMA_Single_Memory_Config(UART4_RX_DMA_Stream,
													 USART_DMA_Channel,
													 (u32)&(UART4->DR),
													 (u32)&_UART4_DMA_RX_BUF[0],
													 sizeof(_UART4_DMA_RX_BUF),
													 DMA_DIR_PeripheralToMemory);
}

void DMA_init(void)
{
	RC_DMA_init();
	judgement_DMA_init();
	vision_DMA_init();
}

