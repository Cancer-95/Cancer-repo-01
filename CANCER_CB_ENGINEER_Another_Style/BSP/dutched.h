#ifndef __DUTCHED_H__
#define __DUTCHED_H__

#include "system.h"

#define DUCTED_R_PWM		    TIM4->CCR3
#define DUCTED_L_PWM		    TIM4->CCR4

#define DUCTED_R_UNLOCKED		DUCTED_R_PWM = 40;
#define DUCTED_L_UNLOCKED		DUCTED_L_PWM = 40;

typedef enum
{
	DUCTED_ALL_OFF        = 0,//������ͣ
	DUCTED_ALL_RUN        = 1,//��������
	DUCTED_L_OFF 		  		= 2,//�󺭵�ͣ
	DUCTED_L_RUN         	= 3,//�󺭵���
	DUCTED_R_OFF 		  		= 4,//�Һ���ͣ
	DUCTED_R_RUN         	= 5,//�Һ�����


} ducted_mode_e;


typedef __packed struct
{
	ducted_mode_e  ctrl_mode;
	
  int16_t  pwm_set[2];
	int16_t  pwm_out[2];
	
} dutched_t;

extern dutched_t ducted;


void ducted_init(void);  //TIM4

void ducted_task(void);

#endif 

