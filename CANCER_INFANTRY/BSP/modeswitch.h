#ifndef __MODESWITH_H
#define __MODESWITH_H
#include "system.h"




typedef enum
{
	SAFETY_MODE       = 0,//��ȫģʽ
	RC_MACHINE_MODE   = 1,//ң�ػ�еģʽ
	RC_GYRO_MODE      = 2,//ң��������ģʽ
  RC_DODGE_MODE     = 5,//ң��Ť��(�����ʵû���õ�)
	KEY_MACHINE_MODE  = 3,//���̻�еģʽ
	KEY_GYRO_MODE     = 4,//����������ģʽ
	KEY_DODGE_MODE    = 5,//����Ť��
	

} SYSTEM_MODE;//ϵͳģʽ


typedef enum
{
	GIMBAL_STO      = 0,//��̨�����
	GIMBAL_MEC      = 1,//��̨��еģʽ
	GIMBAL_GYO      = 2,//��̨������ģʽ

} GIMBAL_MODE;//��̨ģʽ

typedef enum
{
	CHASSIS_STOP 						= 0,//���̲����
	MANUAL_FOLLOW_CHASSIS 	= 1,//������̨����
	MANUAL_FOLLOW_GIMBAL 		= 2,//������̨����
	DODGE_MODE      				= 3,//ҡ��
	
} CHASSIS_MODE;//����ģʽ

typedef enum
{
	FRIC_WHEEL_OFF 		  		= 0,//Ħ����ͣ
	FRIC_WHEEL_RUN         	= 1,//Ħ���ֿ�
	
} FRIC_MODE;//Ħ����ģʽ

typedef enum
{
	SERVO_CLOSE 		  		= 0,//���ֹ�
	SERVO_OPEN            = 1,//���ֿ�
	
} SERVO_MODE;//���ֶ��ģʽ


extern CHASSIS_MODE chassis_mode;
extern GIMBAL_MODE gimbal_mode;
extern FRIC_MODE fric_mode;
extern SYSTEM_MODE system_mode;



void Ctrl_Mode_Select(void);


#endif 

