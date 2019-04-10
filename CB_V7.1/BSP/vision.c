#include "vision.h"

#define    UART4_TX_LEN_HEADER    sizeof(extuart4_sendheader_t)    //֡ͷ��
#define    UART4_TX_LEN_DATA      sizeof(extuart4_senddata_t)      //���Զ������ݶγ���,
#define    UART4_TX_LEN_TAIL      sizeof(extuart4_sendtail_t)	     //֡βCRC16
#define    UART4_TX_LEN_PACKED    UART4_TX_LEN_HEADER + UART4_TX_LEN_DATA + UART4_TX_LEN_TAIL       //��������

#define    UART4_RX_LEN_HEADER    UART4_TX_LEN_HEADER
#define    UART4_RX_LEN_PACKED    sizeof(extuart4_recvdata_t)

//�Ƿ��������ݸ���,FALSEû��,TRUE�����µ�
uint8_t UART4_Data_Update = FALSE;

//���յ��������ݴ�������
unsigned char uart4_rx_buffer[100] = {0};

/*���͵���ؽṹ��*/
extuart4_sendheader_t    uart4_sendheader;  //ͷ
extuart4_senddata_t      uart4_senddata;    //���ͽṹ��
extuart4_sendtail_t      uart4_sendtail;    //β

/*���յ���ؽṹ��*/
extuart4_recvdata_t      uart4_recvdata;    //���սṹ��
 
/*�Ӿ���ؽṹ��*/
vision_t                 vision;            //�Ӿ��ṹ��

void vision_init(void)
{
	// GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE); 

	// ����4��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_UART4);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_UART4);

	// UART4�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 		//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 		//����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	// UART4��ʼ������
	USART_InitStructure.USART_BaudRate = 115200;					//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	    //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;			//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ
	USART_Init(UART4, &USART_InitStructure); 										            //��ʼ������5
	
	USART_Cmd(UART4, ENABLE);  						                                  //ʹ�ܴ���1
	USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);                           //����IDLE����ж�
//	USART_ClearFlag(UART4, USART_FLAG_TC);

	//uart4 NVIC ����
	nvic_config(UART4_IRQn,UART4_IRQn_pre,UART4_IRQn_sub);// Configure NVIC 
//	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;		//����4�ж�ͨ��
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2; //��ռ���ȼ�2
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//�����ȼ�1
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);							        //����ָ���Ĳ�����ʼ��VIC�Ĵ���

	//DMA���������ж�
	USART_DMACmd( UART4, USART_DMAReq_Rx, ENABLE );
	USART_DMACmd( UART4, USART_DMAReq_Tx, ENABLE );

//UART4_DMA_Init:

	DMA_DeInit( DMA1_Stream2 );
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;

	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//�������赽�洢��

	DMA_InitStructure.DMA_PeripheralBaseAddr  = (uint32_t)&(UART4->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr     = (uint32_t)uart4_rx_buffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 100;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	DMA_Init( DMA1_Stream2, &DMA_InitStructure );	
	DMA_Cmd( DMA1_Stream2, ENABLE);  //stream2

}

/***********************************************************************/
/*                            ���պ���		                           */
/***********************************************************************/

void uart4_read_data(uint8_t *data)
{
	//�ж�֡ͷ�����Ƿ�Ϊ0xA5
	if(data[0] == 0xA5)
	{
		//֡ͷCRC8У��		
		if(Verify_CRC8_Check_Sum( data, UART4_RX_LEN_HEADER ) == TRUE)
		{
			//֡βCRC16У��
			if(Verify_CRC16_Check_Sum( data, UART4_RX_LEN_PACKED ) == TRUE)
			{
				//�������ݿ���
				memcpy( &uart4_recvdata, data, sizeof(extuart4_recvdata_t));	
				UART4_Data_Update = YES;//������ݸ�����
			}
			else UART4_Data_Update = NO;
		}
		else UART4_Data_Update = NO;
	}
	else UART4_Data_Update = NO;
}

int uart4_datalength = 0;
void UART4_IRQHandler(void)
{
  if(USART_GetITStatus(UART4,USART_IT_IDLE)!=RESET)//��⵽������·
	{		
		
		UART4->SR ;
		UART4->DR ;
			
		DMA_Cmd(DMA1_Stream2,DISABLE );
		
		uart4_datalength = 100 - DMA_GetCurrDataCounter(DMA1_Stream2);

		/*��ȡ�븳ֵ*/
		uart4_read_data(uart4_rx_buffer);//��ȡ����������	
		if(UART4_Data_Update)
		{
			vision.RX_Cmd = uart4_recvdata.CmdID;
			vision.captured = uart4_recvdata.captured;
			vision.y_fdb = uart4_recvdata.y_fdb;		
		}

				
		memset(uart4_rx_buffer+uart4_datalength, 0, DMA1_Stream2->NDTR);		//�����ȫ����//���¸��ǽ�������//�����׵�ַ
 		DMA1_Stream2->NDTR=100;
		
		DMA_Cmd(DMA1_Stream2,ENABLE);//D1S2
	}
	
}

/***********************************************************************/
/*                            ���ͺ���		                             */
/***********************************************************************/
void uart4_sendchar(u8 data)
{
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET); 
	USART_SendData(UART4,data);   
} 

uint8_t uart4_send_pack[UART4_TX_LEN_PACKED] = {0};//����UART4_LEN_PACKED����
void uart4_send_data( uint8_t CmdID, uint8_t data_0)
{
	int i;    //ѭ�����ʹ���

	uart4_sendheader.SOF = 0xA5;
	uart4_sendheader.CmdID = CmdID;//����ID 00��Ҫ���ҷ������� 01���� 02����
	
	//д��֡ͷ
	memcpy( uart4_send_pack, &uart4_sendheader, UART4_TX_LEN_HEADER );
	
	//֡ͷCRC8У��Э��
	Append_CRC8_Check_Sum( uart4_send_pack, UART4_TX_LEN_HEADER );//��֡ͷ����ĩβ
	
	//���͵�����
	uart4_senddata.data_0 = data_0;

	memcpy( uart4_send_pack + UART4_TX_LEN_HEADER, &uart4_senddata, UART4_TX_LEN_DATA);
	
	//֡βCRC16У��Э��
	Append_CRC16_Check_Sum( uart4_send_pack, UART4_TX_LEN_PACKED );//��֡β
	
	//������õ�����ͨ��������λ���ͳ�ȥ
	for (i = 0; i < UART4_TX_LEN_PACKED; i++)
	{
		uart4_sendchar( uart4_send_pack[i] );
	}
}

/***********************************************************************/
/*                            ����������		                         */
/***********************************************************************/
//void vision_param_init(void)
//{
//  vision.y_ref = 602;	
//}

void vision_send_task(void)
{
	if(vision.enable)
	{
		uart4_send_data(vision.TX_Cmd, 0);//���Ӿ������� ָ�00��Ҫ���ҷ������� 01���� 02����
	}
}

float ppp;
float iii;
float ddd;
void vision_task(void)
{
	if(vision.enable)
	{		
		chassis.ass_mode_enable = ENABLE;        //������βģʽ
		
		if(climb_tube.location == ON_GROUND)
		{
			if(feed_bomb.ctrl_mode == AIM_FEED_MODE)
			{
			  pidckz_param_set(&pid_vision_chassis_vy, 500, 100, 5.0f, 0.001, 10);   
        vision.y_ref = 140;	
				LED_CAMERA_ON;
				vision.TX_Cmd = 3;
			}
			else
			{
				pidckz_param_set(&pid_vision_chassis_vy, 500, 100, 1.8f, 0.0045f, 1.8f);   
				vision.y_ref = 680;	
				LED_CAMERA_ON;
				vision.TX_Cmd = 1;
				
				lifter.target_bomb_bit[L] = lifter.target_bomb_bit_vision[L];
				lifter.target_bomb_bit[R] = lifter.target_bomb_bit_vision[R];	
				from_lifter_zero = YES;
				from_lifter_bomb_entire = NO; //�������Ŀ��ֵ				
				lifter.ctrl_mode = BOMB_LIFTER_BIT;//���̧��һ��(�޸�)	
				
				if(vision.delay_record && ((abs(lifter.target_bomb_bit[L] - lifter.total_angle[L]) < 100) || (abs(lifter.target_bomb_bit[R] - lifter.total_angle[R]) < 100)))
				{
					vision.delay_record = DISABLE;
					vision.light_delay_time = millis();
					vision.quit_delay_time = millis();
				}			
				if((millis() - vision.light_delay_time > 0) && vision.captured &&
					((abs(lifter.target_bomb_bit[L] - lifter.total_angle[L]) < 100) || 
					 (abs(lifter.target_bomb_bit[R] - lifter.total_angle[R]) < 100)))
				{
					chassis.ctrl_mode = VISION_GET_BOMB;
					if((millis() - vision.quit_delay_time > 3000) && (abs(pid_vision_chassis_vy.err)) < 20)
					{
						flag_bomb_claw_vision = 0;
				  	vision.enable = DISABLE;  //�����Ӿ���λ
					}
					
				}				
			}

		}
		else if(climb_tube.location == ON_ISLAND)
		{
			if(feed_bomb.ctrl_mode != AIM_FEED_MODE)//�����ڲ�����׼������°���shift+V��û���õ�
			{
				pidckz_param_set(&pid_vision_chassis_vy, 500, 100, 2.0f, 0.0045f, 1.8f);   
				vision.y_ref = 602;	
				LED_CAMERA_ON;
				vision.TX_Cmd = 2;
				
				if(vision.delay_record)
				{
					vision.delay_record = DISABLE;
					vision.light_delay_time = millis();
				}	
				if((millis() - vision.light_delay_time > 180) && vision.captured)
				{
					chassis.ctrl_mode = VISION_GET_BOMB;
				}					
			}
		}


	}
	else 
	{
		/*�������Ŀ��ֵ*/
    if(get_bomb.ctrl_mode != PUSH_AIM_SCAN_MODE)//��ɨ��ģʽ�¾Ͳ�Ҫ�Ҷ����ֵ��
		{
			if(lifter.target_bomb_bit[L] != lifter.target_bomb_bit_normal[L] || 
				 lifter.target_bomb_bit[R] != lifter.target_bomb_bit_normal[R])		
			{					
				lifter.target_bomb_bit[L] = lifter.target_bomb_bit_normal[L];
				lifter.target_bomb_bit[R] = lifter.target_bomb_bit_normal[R];
				
				if((lifter.total_angle[L] > lifter.target_bomb_bit[L]) && 
					 (lifter.total_angle[R] < lifter.target_bomb_bit[R]))
				{
					from_lifter_zero = YES;
					from_lifter_bomb_entire = NO; 			
				}
				else if((lifter.total_angle[L] < lifter.target_bomb_bit[L]) && 
					      (lifter.total_angle[R] > lifter.target_bomb_bit[R]))
				{
					from_lifter_zero = NO;
					from_lifter_bomb_entire = YES; 			
				}
			}		
		}			

		LED_CAMERA_OFF;
		vision.delay_record = ENABLE;
		vision.TX_Cmd = 0;//��Ҫ���ҷ�������
		vision.captured = 0;//ûץסͼ
	}

}
