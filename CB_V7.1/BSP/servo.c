#include "servo.h"

//uint8_t MAM_PWM;

/*���ڶ���ĸ�����ʱ*/
bool servo_get_bomb_delay_record = 1;
bool servo_get_bomb_scan_delay_record = 1;

bool servo_feed_bomb_start_delay_record = 1;
bool servo_feed_bomb_lift_up_delay_record = 1;
bool servo_feed_bomb_lift_down_delay_record = 1;
bool servo_feed_bomb_end_delay_record = 1;

uint32_t servo_get_bomb_delay_time;
uint32_t servo_get_bomb_scan_delay_time;

uint32_t servo_feed_bomb_start_delay_time;
uint32_t servo_feed_bomb_lift_up_delay_time;
uint32_t servo_feed_bomb_lift_down_delay_time;
uint32_t servo_feed_bomb_end_delay_time;


void servo_init()	//TIM4  
{
	GPIO_InitTypeDef          gpio;
	TIM_TimeBaseInitTypeDef   tim;
	TIM_OCInitTypeDef          oc;
	
	DBGMCU_APB1PeriphConfig(DBGMCU_TIM4_STOP,ENABLE);					//ʹ��Debugʱ��ϵ�ʹ��ʱ��ͣ
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);		  //

	
  gpio.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD,&gpio);
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13, GPIO_AF_TIM4);   
 
	tim.TIM_Prescaler = 3360-1;
	tim.TIM_CounterMode = TIM_CounterMode_Up;									//���ϼ���
	tim.TIM_Period = 1000-1;   																//50hz  25ms ��������
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
	
	TIM_OC1Init(TIM4,&oc);																		//ͨ��1
	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Disable);					//EnableΪ��һ�θ����¼�����Ч��Disable�Ļ�����������Ч
	TIM_OC2Init(TIM4,&oc);																		//ͨ��2TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Disable);
				 
	TIM_ARRPreloadConfig(TIM4,ENABLE);												//ARR������װ�����ﲻ��Ƶ��Ŀ������������û�й�ϵ  �������һ������
	
	TIM_CtrlPWMOutputs(TIM4,ENABLE);													//ʹ��PWM���
	
	TIM_Cmd(TIM4,ENABLE);

	TIM4->CCR1 = 0;																				    //��Ӧ��̨���
	TIM4->CCR2 = 0;	                                          //��Ӧ���ֶ��
  
	GIM_SERVO_ZERO;	
  MAGAZINE_CLOSE;
//	GIM_SERVO_BACK;	
//  MAGAZINE_CLOSE;
}

//void servo_task()
//{
////	if((lifter.total_angle[R] < lifter.target_tube_bit[R] + 100) ||
////		 (lifter.total_angle[R] > lifter.target_bomb_bit[R] - 100) ||
////	   (lifter.total_angle[L] > lifter.target_tube_bit[L] - 100) ||
////	   (lifter.total_angle[L] < lifter.target_bomb_bit[L] + 100))
////	{
////		if(get_bomb.get_bomb_enable == ENABLE || climb_tube.climb_enable == ENABLE)//ȡ��������ģʽ�¿���С����
////		{	
////			GIM_SERVO_SIDE;  
////		}
////		else if(chassis.ass_mode_enable == ENABLE)//������βģʽ
////		{
////			GIM_SERVO_BACK;
////		}
////		else if(chassis.ass_mode_enable == DISABLE)//�رճ�βģʽʱ
////		{
////			GIM_SERVO_ZERO;
////		}	
////	}
//}  

