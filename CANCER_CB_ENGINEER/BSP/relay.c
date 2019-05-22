#include "relay.h"

bool flag_mirror_CCTV;
bool flag_bomb_claw_CCTV_switch;

void relay_init(void)
{
	GPIO_InitTypeDef gpio;   

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | 
	                       RCC_AHB1Periph_GPIOC | 
	                       RCC_AHB1Periph_GPIOD | 
	                       RCC_AHB1Periph_GPIOE,ENABLE);
			
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	gpio.GPIO_Speed = GPIO_Speed_50MHz;	
	gpio.GPIO_Mode = GPIO_Mode_OUT;										
	
//	gpio.GPIO_Pin = GPIO_Pin_1;//PB1:����ɲ�� 	
	gpio.GPIO_Pin = GPIO_Pin_3 
								| GPIO_Pin_4 
								| GPIO_Pin_5
								| GPIO_Pin_6 //PE3:ȡ��צ�� PE4:���ӵ��� PE5:��������ͷ PE6:С�����л�	
								| GPIO_Pin_15
								| GPIO_Pin_14
								| GPIO_Pin_13
								| GPIO_Pin_12;//PE15:����צ�� PE14:ȡ��צ������ PE13:��Ԯ PE12:���ֿ���	
	GPIO_Init(GPIOE,&gpio);	
	
//	gpio.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11;//PE9:ȡ��צ�� PE11:���ӵ��� 		
//	GPIO_Init(GPIOE,&gpio);	
	
	gpio.GPIO_Pin = GPIO_Pin_2
	              | GPIO_Pin_7
								| GPIO_Pin_6
               	| GPIO_Pin_5 
	              | GPIO_Pin_4;//PA2:ȡ��צ�� PA5:���ӵ��� PA4:С�����л� PA67:�ϳ��Ƴ�
	GPIO_Init(GPIOA,&gpio);	
	
	gpio.GPIO_Pin = GPIO_Pin_0;//PC0:��������ͷ 
	GPIO_Init(GPIOC,&gpio);	
	
	gpio.GPIO_Pin = GPIO_Pin_4
               	| GPIO_Pin_5 
	              | GPIO_Pin_6
	              | GPIO_Pin_7;//PD4:����צ�� PD5:ȡ��צ������ PD6:�ϳ����� PD7:���ֿ���
	GPIO_Init(GPIOD,&gpio);	
	
//	gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;//PC6:ȡ��צ������ PC7:��Ԯ PC8����̬���� PC9:С�����л�
//	gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 ;//PC6:ȡ��צ������ PC7:��Ԯ
//	GPIO_Init(GPIOC,&gpio);	
	
  TUBE_CLAW_LOOSE;//����צ���ɿ�
//	TUBE_BRAKE_PULL;//����ɲ���ջ�
	
	BOMB_CLAW_CATCH;//ȡ��צ�Ӻ���
	BOMB_CLAW_PULL;//ȡ��צ���ջ�
	BOMB_EJECT_PULL;//���������ջ�	
	
	TRAILER_HOOK_PULL;//�������׻���
	TRAILER_HOOK_CAPTURE;//�������׻���
//	SUPPORTING_WHEEL_PULL;//����֧�����ջ�

  CCTV_CLIMB_TUBE;//������ҰĬ������
}

void CCTV_switch_task(void)
{
	if(get_bomb.get_bomb_enable == ENABLE)
	{
		if(flag_bomb_claw_CCTV_switch)
		{
			CCTV_FEED_RESCUE;
		}
		else CCTV_GET_BOMB;
	}
	else if(feed_bomb.feed_bomb_enable == ENABLE || 
		      rescue.rescue_enable == ENABLE)
	{
		if(flag_magazine_exit)
		{
			CCTV_GET_BOMB;
		}
	  else CCTV_FEED_RESCUE;
	}
	else if(!check.check_CCTV_enable)//������ռ��
	{
		CCTV_CLIMB_TUBE;
	}
		
}
