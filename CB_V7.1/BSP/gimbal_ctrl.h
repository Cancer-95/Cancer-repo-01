#ifndef __GIMBALCTRL_H
#define __GIMBALCTRL_H

#include "system.h"

#define Yaw_G_MID 					0


extern float  Yaw_Target;//��Ҫ���ṩ���ⲿ����̨ǿ�ƿ����粹��ʱ��̨ˮƽ


void Gimbal_Init(void);

void Get_GYO_Target(void);

void Gimbal_G_Control_Outter(void);
void Gimbal_G_Control_Inner(void);

void Gimbal_G_Control(void);


void  Gimbal_Control(void);



#endif

