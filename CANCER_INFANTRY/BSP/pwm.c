#include "pwm.h"
void PWM_Init()
{
	Fric_Wheel_PWM_Configuration();
	BBox_Servo__PWM_Configuration();
}

void Fric_Wheel_PWM_Configuration()	//TIM4  Ħ����
{
	GPIO_InitTypeDef          gpio;
	TIM_TimeBaseInitTypeDef   tim;
	TIM_OCInitTypeDef          oc;
	
	DBGMCU_APB1PeriphConfig(DBGMCU_TIM4_STOP,ENABLE);					//ʹ��Debugʱ��ϵ�ʹ��ʱ��ͣ
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);		  //

	
  gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB,&gpio);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9, GPIO_AF_TIM4);   
 
	tim.TIM_Prescaler = 84-1;
	tim.TIM_CounterMode = TIM_CounterMode_Up;									//���ϼ���
	tim.TIM_Period = 2500-1;   																//50hz  25ms ��������
	tim.TIM_ClockDivision = TIM_CKD_DIV1;											//����ʱ�ӷָ��ĳ���˲����й�������ɶ��û��
	TIM_TimeBaseInit(TIM4,&tim);
	
	oc.TIM_OCMode = TIM_OCMode_PWM2;													//ѡ��PWM2�������ϼ���ʱCNTû�ﵽCCRxʱ�����Ч��ƽ ��������LOW֮�󼴵͵�ƽ��Ч���������������
	oc.TIM_OutputState = TIM_OutputState_Enable;							//����OC�������Ӧ����
	oc.TIM_Pulse = 0;																					//���ô�װ�벶��Ƚ���������ֵ ��ʵ��������CCR
	oc.TIM_OCPolarity = TIM_OCPolarity_Low;										//�����������  Ϊ�����ʾ��Ч��ƽΪ�͵�ƽ
	
	oc.TIM_OutputNState = TIM_OutputNState_Disable;						//ʧ�ܻ������ �ر�OCN�������Ӧ����   �߼���ʱ��ʹ�� ����TIM_CH1
	oc.TIM_OCNPolarity = TIM_OCNPolarity_High;								//���û����������
	oc.TIM_OCIdleState = TIM_OCIdleState_Reset;								//ѡ�����״̬�µķǹ���״̬
	oc.TIM_OCNIdleState = TIM_OCNIdleState_Set;								//ѡ�񻥲�����״̬�µķǹ���״̬
	
	TIM_OC3Init(TIM4,&oc);																		//ͨ��3
	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Disable);					//EnableΪ��һ�θ����¼�����Ч��Disable�Ļ�����������Ч
	TIM_OC4Init(TIM4,&oc);																		//ͨ��4
	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Disable);
				 
	TIM_ARRPreloadConfig(TIM4,ENABLE);												//ARR������װ�����ﲻ��Ƶ��Ŀ������������û�й�ϵ  �������һ������
	
	TIM_CtrlPWMOutputs(TIM4,ENABLE);													//ʹ��PWM���
	
	TIM_Cmd(TIM4,ENABLE);
	//CCRx/ARR ����ռ�ձ�		ARR����ͨ��TIM_Period����
	TIM4->CCR3 = 1000;																				  //��Ӧ1
	TIM4->CCR4 = 1000;	                                        //��Ӧ2
}

void BBox_Servo__PWM_Configuration(void)	//TIM1 ���ֶ��
{
	GPIO_InitTypeDef          gpio;
	TIM_TimeBaseInitTypeDef   tim;
	TIM_OCInitTypeDef         oc;
	
  DBGMCU_APB1PeriphConfig(DBGMCU_TIM1_STOP,ENABLE);					//ʹ��Debugʱ��ϵ�ʹ��ʱ��ͣ
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);		

	gpio.GPIO_Pin = GPIO_Pin_9;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&gpio);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9, GPIO_AF_TIM1);     
	
	tim.TIM_Prescaler     = 3360-1;
	tim.TIM_CounterMode   = TIM_CounterMode_Up;	 //���ϼ���
	tim.TIM_Period        = 999;                 //20ms	��������
	tim.TIM_ClockDivision = TIM_CKD_DIV1;		     //����ʱ�ӷָ� ��Ϊ1�Ļ����2
	TIM_TimeBaseInit(TIM1,&tim);
	
	oc.TIM_OCMode = TIM_OCMode_PWM2;		            //ѡ��ʱ��ģʽ
	oc.TIM_OutputState = TIM_OutputState_Enable;		//ѡ������Ƚ�״̬
	oc.TIM_OutputNState = TIM_OutputState_Disable;	//ѡ�񻥲�����Ƚ�״̬
	oc.TIM_Pulse = 0;		                            //���ô�װ�벶��Ƚ���������ֵ
	oc.TIM_OCPolarity = TIM_OCPolarity_Low;		      //�����������
	oc.TIM_OCNPolarity = TIM_OCPolarity_High;		    //���û����������
	oc.TIM_OCIdleState = TIM_OCIdleState_Reset;		  //ѡ�����״̬�µķǹ���״̬
	oc.TIM_OCNIdleState = TIM_OCIdleState_Set;		  //ѡ�񻥲�����״̬�µķǹ���״̬

	TIM_OC2Init(TIM1,&oc);		//ͨ��2
	TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
				 
	TIM_ARRPreloadConfig(TIM1,ENABLE);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	
	TIM_Cmd(TIM1,ENABLE);

  TIM1->CCR2 = 0;
}

