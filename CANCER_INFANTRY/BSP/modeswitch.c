#include "modeswitch.h"

GIMBAL_MODE gimbal_mode;
CHASSIS_MODE chassis_mode;
FRIC_MODE fric_mode;
SYSTEM_MODE system_mode;

u8 Key_MG_flag = 1;
bool Timeout_Tri_CTRL = 1;
u8 Key_DD_flag = 0;
bool Timeout_Tri_Q = 1;

void Ctrl_Mode_Select()//����S1��ģʽ�л�
{
	/*ϵͳģʽѡ��*/
	switch(RC_Ctl.rc.s1)
  {
		case RC_SW_UP:		
		{
			system_mode = RC_MACHINE_MODE;
		}break;
		
		case RC_SW_MID:		
		{
			system_mode = RC_GYRO_MODE;
		}break;
		
		case RC_SW_DOWN:	//����ģʽ
		{
			/*�����л���е������ģʽ*/
			if(KEY_CTRL_PRESSED  && Timeout_Tri_CTRL == 1)
			{
				Timeout_Tri_CTRL = 0;
				Key_MG_flag ++;
				Key_MG_flag %= 2;
			}
			else if(KEY_CTRL_NOT_PRESSED) Timeout_Tri_CTRL = 1;
		
			if(Key_MG_flag)
			{
				system_mode = KEY_MACHINE_MODE;
			}
			else
			{
				system_mode = KEY_GYRO_MODE;
			}
			
			/*���̽���Ť��ģʽ*/
			if(KEY_Q_PRESSED  && Timeout_Tri_Q == 1)
			{
				Timeout_Tri_Q = 0;
				Key_DD_flag ++;
				Key_DD_flag %= 2;
			}
			else if(KEY_Q_NOT_PRESSED) Timeout_Tri_Q = 1;
			
			if(Key_DD_flag)
			{
				system_mode = KEY_DODGE_MODE;
			}


		}break;
		
		default:					
		{
			system_mode = SAFETY_MODE;
		}break;
	}
	
	/*ϵͳģʽ��Ҫ��Щʲô*/
	switch(system_mode)
	{
	  case RC_MACHINE_MODE://ң�ػ�еģʽ
		{
		  gimbal_mode = GIMBAL_MEC;
			chassis_mode = MANUAL_FOLLOW_CHASSIS;//��еģʽ��̨�������
			fric_mode = FRIC_WHEEL_OFF;	
		}break;
		
		case RC_GYRO_MODE://ң��������ģʽ
		{
		  gimbal_mode = GIMBAL_GYO;	
			fric_mode = FRIC_WHEEL_RUN;
			
			if(RC_Ctl.rc.s2 == RC_SW_DOWN) //ң����������ģʽ��S2�²�
			{
				chassis_mode = DODGE_MODE;
			}
			else 
			{
		   	chassis_mode = MANUAL_FOLLOW_GIMBAL;//������ģʽ���̸�����̨
			}

		}break;
		
		case KEY_MACHINE_MODE://���̻�еģʽ
		{
		  gimbal_mode = GIMBAL_MEC;
			chassis_mode = MANUAL_FOLLOW_CHASSIS;//��еģʽ��̨�������
			fric_mode = FRIC_WHEEL_OFF;	
		}break;
		
		case KEY_GYRO_MODE://����������ģʽ
		{
		  gimbal_mode = GIMBAL_GYO;	
			chassis_mode = MANUAL_FOLLOW_GIMBAL;//������ģʽ���̸�����̨
			fric_mode = FRIC_WHEEL_RUN;
		}break;
		
		case KEY_DODGE_MODE://����Ť��
		{  
	  	gimbal_mode = GIMBAL_GYO;	
			chassis_mode = DODGE_MODE;//������ģʽ���̸�����̨
			fric_mode = FRIC_WHEEL_OFF;
		}break;
		
		case SAFETY_MODE://��ȫģʽ
		{
			gimbal_mode = GIMBAL_STO;
			chassis_mode = CHASSIS_STOP;
			fric_mode = FRIC_WHEEL_OFF;	
			

			
		}break;
		
		default:		
		break;
	
	}
	
	
	
	
	
	
	
}

