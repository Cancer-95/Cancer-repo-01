#include "tim.h"

void servo_PWM_init(void)
{
	TIM_TimeBaseInitTypeDef   tim;
	TIM_OCInitTypeDef          oc;
	
	DBGMCU_APB1PeriphConfig(DBGMCU_TIM4_STOP,ENABLE);					//ʹ��Debugʱ��ϵ�ʹ��ʱ��ͣ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);		  //

	tim.TIM_Prescaler     = 3360-1;
	tim.TIM_CounterMode   = TIM_CounterMode_Up;									//���ϼ���
	tim.TIM_Period        = 1000-1;   																//50hz  25ms ��������
	tim.TIM_ClockDivision = TIM_CKD_DIV1;											//����ʱ�ӷָ��ĳ���˲����й�������ɶ��û��
	TIM_TimeBaseInit(TIM4,&tim);
	
	oc.TIM_OCMode = TIM_OCMode_PWM2;													//ѡ��PWM2�������ϼ���ʱCNTû�ﵽCCRxʱ�����Ч��ƽ ��������LOW֮�󼴵͵�ƽ��Ч���������������
	oc.TIM_OutputState = TIM_OutputState_Enable;							//����OC�������Ӧ����
	oc.TIM_Pulse = 0;																					//���ô�װ�벶��Ƚ���������ֵ ��ʵ��������CCR
	oc.TIM_OCPolarity  = TIM_OCPolarity_Low;										//�����������  Ϊ�����ʾ��Ч��ƽΪ�͵�ƽ
	
	oc.TIM_OutputNState = TIM_OutputNState_Disable;						//ʧ�ܻ������ �ر�OCN�������Ӧ����   �߼���ʱ��ʹ�� ����TIM_CH1
	oc.TIM_OCNPolarity  = TIM_OCNPolarity_High;								//���û����������
	oc.TIM_OCIdleState  = TIM_OCIdleState_Reset;								//ѡ�����״̬�µķǹ���״̬
	oc.TIM_OCNIdleState = TIM_OCNIdleState_Set;								//ѡ�񻥲�����״̬�µķǹ���״̬
	
	TIM_OC1Init(TIM4,&oc);																		//ͨ��1
	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Disable);					//EnableΪ��һ�θ����¼�����Ч��Disable�Ļ�����������Ч
				 
	TIM_ARRPreloadConfig(TIM4,ENABLE);												//ARR������װ�����ﲻ��Ƶ��Ŀ������������û�й�ϵ  �������һ������
	
	TIM_CtrlPWMOutputs(TIM4,ENABLE);													//ʹ��PWM���
	
	TIM_Cmd(TIM4,ENABLE);
}

void dutched_PWM_init(void)
{
	TIM_TimeBaseInitTypeDef   tim;
	TIM_OCInitTypeDef          oc;
	
	DBGMCU_APB1PeriphConfig(DBGMCU_TIM4_STOP,ENABLE);					//ʹ��Debugʱ��ϵ�ʹ��ʱ��ͣ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);		  //

	 /*���� = (Period * Prescaler)/FLK = (2500*84)/84000000=2.5ms*/
	tim.TIM_Prescaler     = 1680-1;
	tim.TIM_CounterMode   = TIM_CounterMode_Up;									//���ϼ���
	tim.TIM_Period        = 1000-1;   																//50hz  ��������
	tim.TIM_ClockDivision = TIM_CKD_DIV1;											//����ʱ�ӷָ��ĳ���˲����й�������ɶ��û��
	TIM_TimeBaseInit(TIM4,&tim);
	
	oc.TIM_OCMode = TIM_OCMode_PWM2;													//ѡ��PWM2�������ϼ���ʱCNTû�ﵽCCRxʱ�����Ч��ƽ ��������LOW֮�󼴵͵�ƽ��Ч���������������
	oc.TIM_OutputState = TIM_OutputState_Enable;							//����OC�������Ӧ����
	oc.TIM_Pulse = 0;																					//���ô�װ�벶��Ƚ���������ֵ ��ʵ��������CCR
	oc.TIM_OCPolarity  = TIM_OCPolarity_Low;										//�����������  Ϊ�����ʾ��Ч��ƽΪ�͵�ƽ
	
	oc.TIM_OutputNState = TIM_OutputNState_Disable;						//ʧ�ܻ������ �ر�OCN�������Ӧ����   �߼���ʱ��ʹ�� ����TIM_CH1
	oc.TIM_OCNPolarity  = TIM_OCNPolarity_High;								//���û����������
	oc.TIM_OCIdleState  = TIM_OCIdleState_Reset;								//ѡ�����״̬�µķǹ���״̬
	oc.TIM_OCNIdleState = TIM_OCNIdleState_Set;								//ѡ�񻥲�����״̬�µķǹ���״̬
	
	TIM_OC3Init(TIM4,&oc);																		//ͨ��3
	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Disable);					//EnableΪ��һ�θ����¼�����Ч��Disable�Ļ�����������Ч
	TIM_OC4Init(TIM4,&oc);																		//ͨ��4
	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Disable);
				 
	TIM_ARRPreloadConfig(TIM4,ENABLE);												//ARR������װ�����ﲻ��Ƶ��Ŀ������������û�й�ϵ  �������һ������
	
	TIM_CtrlPWMOutputs(TIM4,ENABLE);													//ʹ��PWM���
	
	TIM_Cmd(TIM4,ENABLE);

}

void TIM_init(void)
{
  servo_PWM_init();
	dutched_PWM_init();
}
