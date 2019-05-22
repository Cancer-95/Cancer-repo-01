#include "modeswitch.h"

SYSTEM_MODE system_mode;


void mode_switch_task()//����S1��ģʽ�л�
{
	static bool once_task_init = ENABLE;	
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
			system_mode = KB_MODE;
			
		}break;
		
		default:					
		{
	    system_mode = SAFETY_MODE;
		}break;
	}
			
	switch(RC_Ctl.rc.s2)//��Ҫ������ǰһ��ɧ����
  {
		case RC_SW_UP:		
		{
			if(check.step_run_once && check.next_step_enable)
			{
				check.step_run_once = DISABLE;
				check.check_step ++;
				if(check.check_step > 6) check.check_step = 1;
			}
			check.check_enable = ENABLE;
			climb_tube.climb_enable = DISABLE;
			get_bomb.get_bomb_enable = DISABLE;
			rescue.rescue_enable = DISABLE;
			feed_bomb.feed_bomb_enable = DISABLE;			

		}break;
		
		case RC_SW_MID:		
		{
      check.step_run_once = ENABLE;
			check.check_enable = ENABLE;
			check.check_CCTV_enable = ENABLE;
			
			climb_tube.climb_enable = DISABLE;
			get_bomb.get_bomb_enable = DISABLE;
			rescue.rescue_enable = DISABLE;
			feed_bomb.feed_bomb_enable = DISABLE;			
		  once_task_init = ENABLE;
		}break;
		
		case RC_SW_DOWN:
		{
			check.check_enable = DISABLE;
			if(once_task_init)
			{
				climb_tube.climb_enable = ENABLE;
				get_bomb.get_bomb_enable = ENABLE;	
				rescue.rescue_enable = ENABLE;
				feed_bomb.feed_bomb_enable = ENABLE;	

				once_task_init = DISABLE;
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
		  kb.kb_enable = DISABLE; //ʧ�ܼ���
			rc.rc_enable = ENABLE;  //ʹ��ң����
			
			chassis.ctrl_mode = CHASSIS_SEPARATE;
		}break;
		
		case RC_GYRO_MODE://ң��������ģʽ
		{
			kb.kb_enable = DISABLE; //ʧ�ܼ���
			rc.rc_enable = ENABLE;  //ʹ��ң����	
			
			chassis.ctrl_mode = CHASSIS_FOLLOW;
			flag_gyro_blow_up = 0;//���������ûը
		}break;		
		
		case KB_MODE://����ģʽ
		{			
			kb.kb_enable = ENABLE; //ʹ�ܼ���
			rc.rc_enable = DISABLE;//ʧ��ң����

		}break;
		
		case SAFETY_MODE://��ȫģʽ
		{		
			ducted.ctrl_mode = DUCTED_ALL_OFF;	      //�����������ص�
			assist_wheel.ctrl_mode = WHEEL_RELAX;     //Ħ���������			
			optic_switch.ctrl_mode = DETECT_ENABLE;   //����ǿ����1
			climb_tube.climb_enable = DISABLE;        //ʧ������(�����������еĿ��ƶ�����)
			get_bomb.get_bomb_enable = DISABLE;
			rescue.rescue_enable = DISABLE;
			feed_bomb.feed_bomb_enable = DISABLE;			
		
			rc.rc_enable = DISABLE;                   //ʧ��ң����
			kb.kb_enable = DISABLE;                   //ʧ�ܼ���		
			kb.tube_claw_enable = DISABLE;            //צ�Ӳ��ɿ�
			
			check.check_enable = DISABLE;             //ϵͳ���ʧ��
			
		}break;
		
		default:		
		break;
	
	}
	

	
	
	
	
}

