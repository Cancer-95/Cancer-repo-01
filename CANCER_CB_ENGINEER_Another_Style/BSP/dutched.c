#include "dutched.h"

#define DUTCHED_CTRL_PERIOD     5  //ms �����ĸ�ʱ���
#define DUTCHED_ACC_TIME        50  //ms 

//uint8_t MAM_PWM;

dutched_t ducted;

void ducted_init(void)
{
	memset(&ducted, 0, sizeof(dutched_t));
	
////	GPIO_InitTypeDef         gpio;
//	TIM_TimeBaseInitTypeDef   tim;
//	TIM_OCInitTypeDef          oc;
//	
//	DBGMCU_APB1PeriphConfig(DBGMCU_TIM4_STOP,ENABLE);					//ʹ��Debugʱ��ϵ�ʹ��ʱ��ͣ
////	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);		  //

////  gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
////	gpio.GPIO_Mode = GPIO_Mode_AF;
////	gpio.GPIO_Speed = GPIO_Speed_100MHz;
////	GPIO_Init(GPIOB,&gpio);
//	
////	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8, GPIO_AF_TIM4);
////	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9, GPIO_AF_TIM4);  
//	
// /*���� = (Period * Prescaler)/FLK = (2500*84)/84000000=2.5ms*/
//	tim.TIM_Prescaler = 1680-1;
//	tim.TIM_CounterMode = TIM_CounterMode_Up;									//���ϼ���
//	tim.TIM_Period = 1000-1;   																//50hz  ��������
//	tim.TIM_ClockDivision = TIM_CKD_DIV1;											//����ʱ�ӷָ��ĳ���˲����й�������ɶ��û��
//	TIM_TimeBaseInit(TIM4,&tim);
//	
//	oc.TIM_OCMode = TIM_OCMode_PWM2;													//ѡ��PWM2�������ϼ���ʱCNTû�ﵽCCRxʱ�����Ч��ƽ ��������LOW֮�󼴵͵�ƽ��Ч���������������
//	oc.TIM_OutputState = TIM_OutputState_Enable;							//����OC�������Ӧ����
//	oc.TIM_Pulse = 0;																					//���ô�װ�벶��Ƚ���������ֵ ��ʵ��������CCR
//	oc.TIM_OCPolarity = TIM_OCPolarity_Low;										//�����������  Ϊ�����ʾ��Ч��ƽΪ�͵�ƽ
//	
//	oc.TIM_OutputNState = TIM_OutputNState_Disable;						//ʧ�ܻ������ �ر�OCN�������Ӧ����   �߼���ʱ��ʹ�� ����TIM_CH1
//	oc.TIM_OCNPolarity = TIM_OCNPolarity_High;								//���û����������
//	oc.TIM_OCIdleState = TIM_OCIdleState_Reset;								//ѡ�����״̬�µķǹ���״̬
//	oc.TIM_OCNIdleState = TIM_OCNIdleState_Set;								//ѡ�񻥲�����״̬�µķǹ���״̬
//	
//	TIM_OC3Init(TIM4,&oc);																		//ͨ��3
//	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Disable);					//EnableΪ��һ�θ����¼�����Ч��Disable�Ļ�����������Ч
//	TIM_OC4Init(TIM4,&oc);																		//ͨ��4
//	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Disable);
//				 
//	TIM_ARRPreloadConfig(TIM4,ENABLE);												//ARR������װ�����ﲻ��Ƶ��Ŀ������������û�й�ϵ  �������һ������
//	
//	TIM_CtrlPWMOutputs(TIM4,ENABLE);													//ʹ��PWM���
//	
//	TIM_Cmd(TIM4,ENABLE);
//	//CCRx/ARR ����ռ�ձ�		ARR����ͨ��TIM_Period����(2500)

  /*��������*/
	DUCTED_R_UNLOCKED;//PB8																			//��Ӧ1
	DUCTED_L_UNLOCKED;//PB9                                     //��Ӧ2

}

void ducted_pwm(uint16_t pwm_L, uint16_t pwm_R)//Ħ���ֵĿ���
{
	DUCTED_L_PWM = pwm_L + 40;
	DUCTED_R_PWM = pwm_R + 40;
}

void ducted_task(void)
{
	if (system_mode == SAFETY_MODE)
	{
		ducted.pwm_set[L] = 0;
		ducted.pwm_set[R] = 0;		
	}	
	else
	{
		ducted.pwm_set[L] = 83;
		ducted.pwm_set[R] = 83;		
	}

	
	switch(ducted.ctrl_mode)
	{
		case DUCTED_L_RUN:
		{								
		 ducted.pwm_out[L] = ducted.pwm_set[L] * ramp_dutched_calc(&ld_run_ramp, CNT_UP, DUTCHED_ACC_TIME/DUTCHED_CTRL_PERIOD);
		 ducted.pwm_out[R] = ducted.pwm_set[R] * ramp_dutched_calc(&rd_run_ramp, TO_ZERO, DUTCHED_ACC_TIME/DUTCHED_CTRL_PERIOD);;
		}break;

		case DUCTED_R_RUN:
		{						
			ducted.pwm_out[R] = ducted.pwm_set[R] * ramp_dutched_calc(&rd_run_ramp, CNT_UP, DUTCHED_ACC_TIME/DUTCHED_CTRL_PERIOD);
			ducted.pwm_out[L] = ducted.pwm_set[L] * ramp_dutched_calc(&ld_run_ramp, TO_ZERO, DUTCHED_ACC_TIME/DUTCHED_CTRL_PERIOD);;		
		}break;
		
		case DUCTED_ALL_OFF:
		{	
			ducted.pwm_out[L] = ducted.pwm_set[L] * ramp_dutched_calc(&ld_run_ramp, TO_ZERO, DUTCHED_ACC_TIME/DUTCHED_CTRL_PERIOD);;
			ducted.pwm_out[R] = ducted.pwm_set[R] * ramp_dutched_calc(&rd_run_ramp, TO_ZERO, DUTCHED_ACC_TIME/DUTCHED_CTRL_PERIOD);;
		}break;
		
		case DUCTED_ALL_RUN:
		{	
			ducted.pwm_out[L] = ducted.pwm_set[L] * ramp_dutched_calc(&ld_run_ramp, CNT_UP, DUTCHED_ACC_TIME/DUTCHED_CTRL_PERIOD);;
			ducted.pwm_out[R] = ducted.pwm_set[R] * ramp_dutched_calc(&rd_run_ramp, CNT_UP, DUTCHED_ACC_TIME/DUTCHED_CTRL_PERIOD);;
		}break;		
		
		default:																																															
    break;
	}	
	ducted_pwm((int16_t)ducted.pwm_out[L],(int16_t)ducted.pwm_out[R]);
}


