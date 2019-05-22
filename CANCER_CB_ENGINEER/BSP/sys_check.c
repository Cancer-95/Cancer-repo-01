#include "sys_check.h"

check_t check;

void sys_check_init(void)
{
	memset(&check, 0, sizeof(check_t));
	check.next_step_enable = ENABLE;
//	check.step_run_once = ENABLE;
	
	check.motor_check_time_record = ENABLE;
	check.motor_relax_time_record = ENABLE;
}

static void sensor_check(void)
{
	if(optic_switch.value[S]) 
	{
		LED_RED_OFF;
	  LED_BLUE_OFF;
	}
	else 
	{
		LED_RED_ON;
		LED_BLUE_ON;	
	}
	
	if(optic_switch.value[M])
	{
	  LED_ORANGE_ON;
	  LED_GREEN_ON;
	}
	else 
	{
		LED_ORANGE_OFF;
		LED_GREEN_OFF;	
	}
}

static void actuator_check(void) //һ���������绢
{
	kb.tube_claw_enable = DISABLE;           //����צ��ʧ��
	kb.bomb_claw_enable = DISABLE;           //ȡ��צ��ʧ��
	
	switch(check.check_step)
	{
		case 2:
		{
			check.next_step_enable = DISABLE;
			lifter.ctrl_mode = TUBE_LIFTER_ENTIRE;	    //���̧��  
			CCTV_FEED_RESCUE;
			
			if((abs(lifter.target_tube_entire[L] - lifter.total_angle[L]) < 100) || 
			   (abs(lifter.target_tube_entire[R] - lifter.total_angle[R]) < 100))    //̧����Ŀ��Ƕ�
			{
				GIM_SERVO_BACK;
				TUBE_CLAW_CATCH;                          //����צ��ǿ�ƺ���		
				MAGAZINE_OPEN;                           	//���ָǴ�
			  trailer.ctrl_mode = TRAILER_PUSH;           //�ϳ��Ƴ�  				
//				if(abs(trailer.total_angle - trailer.target_entire) < 100) 
//				{
					TRAILER_HOOK_RELEASE; 
					TRAILER_HOOK_PUSH; 
					check.next_step_enable = ENABLE;
//				}					
			}
		}break;
		
		case 3:
		{
			check.next_step_enable = DISABLE;
	    lifter.ctrl_mode = LIFTER_ZERO;            //��ܻر궨��λ��
			TUBE_CLAW_LOOSE;                           //����צ��ǿ���ſ�	
			MAGAZINE_CLOSE;                            //���ָǺ���
			GIM_SERVO_ZERO;	
			trailer.ctrl_mode = TRAILER_PULL;                                //�ϳ��ջ�		
      TRAILER_HOOK_CAPTURE;                                               //�������׻���		
			TRAILER_HOOK_PULL;                                               //�������׻���		
			CCTV_CLIMB_TUBE;
			if((abs(lifter.target_zero[L] - lifter.total_angle[L]) < 100) || 
			   (abs(lifter.target_zero[R] - lifter.total_angle[R]) < 100))    //̧����Ŀ��Ƕ�
			{
				check.next_step_enable = ENABLE;
				
			}
		}break;
		
		case 4:
		{			
			check.next_step_enable = DISABLE;
			CCTV_GET_BOMB;
			if((abs(lifter.target_bomb_entire[L] - lifter.total_angle[L]) < 100) || 
			   (abs(lifter.target_bomb_entire[R] - lifter.total_angle[R]) < 100))    //̧����Ŀ��Ƕ�
			{
		
				if((abs(bomb_claw.target_half[L] - bomb_claw.total_angle[L]) < 100) || 
					 (abs(bomb_claw.target_half[R] - bomb_claw.total_angle[R]) < 100))   //ȡ��צ�ӷ�ת����λ��
				{
					ducted.ctrl_mode = DUCTED_ALL_RUN;      //������	
					BOMB_CLAW_MOVE_RIGHT;
					if(abs(bomb_claw.target_entire[Y] - bomb_claw.total_angle[Y]) < 100) //ȡ��צ��Ų����λ��
					{
						BOMB_CLAW_PUSH;                         //�Ƴ�ȡ��צ��	
            BOMB_CLAW_LOOSE;                        //ȡ��צ�ӿ�					
					  BOMB_EJECT_PUSH;				                //ȡ�����俪							
						ducted.ctrl_mode = DUCTED_ALL_OFF;      //�غ���		
			    	check.next_step_enable = ENABLE;						
					}
				}	
				else BOMB_CLAW_ROLL_HALF; 
			}
			else lifter.ctrl_mode = BOMB_LIFTER_ENTIRE;	 //���̧��		
		}break;
		
		case 5:
		{
			check.next_step_enable = DISABLE;
			BOMB_CLAW_MOVE_LEFT;
			if(abs(bomb_claw.target_zero[Y] - bomb_claw.total_angle[Y]) < 100) //ȡ��צ��Ų����λ��
			{ 
				BOMB_CLAW_ROLL_ZERO;                     //ȡ��צ�ӷ�ת����	
//				if((abs(bomb_claw.target_zero[L] - bomb_claw.total_angle[L]) < 100) || 
//					 (abs(bomb_claw.target_zero[R] - bomb_claw.total_angle[R]) < 100))   //ȡ��צ�ӷ�ת����λ��
				if(bomb_claw.zero_correct == FINISHED)
				{
					BOMB_CLAW_CATCH;                         //ȡ��צ��ץ
					BOMB_EJECT_PULL;				                 //ȡ�������ջ�		
					check.next_step_enable = ENABLE;	
				}					
			}
		}break;
		
		case 6:
		{
			check.next_step_enable = DISABLE;
			CCTV_CLIMB_TUBE;
			BOMB_CLAW_MOVE_MID;
			BOMB_CLAW_ROLL_ENTIRE; 
			BOMB_CLAW_PULL;                          //�ջ�ȡ��צ��
      lifter.ctrl_mode = LIFTER_ZERO;            //��ܻر궨��λ��	
			if((abs(lifter.target_zero[L] - lifter.total_angle[L]) < 100) || 
			   (abs(lifter.target_zero[R] - lifter.total_angle[R]) < 100))    //̧����Ŀ��Ƕ�
			{
				check.next_step_enable = ENABLE;
			}
		}break;
		
		default:break;	
	}
}
/*������߼��*/
void motor_offline_check(void)
{
	CAN1_motor_offline_check();
	CAN2_motor_offline_check();
}
/*�����ת���*/
void motor_stall_check(void)
{
/*��⵽20s��������ת��������8��*/
	if(check.motor_check_time_record)//ÿʮ�� ���һ�¶�ת���� ��һ�ζ�ת����
	{	
    trailer.stall_times = 0;
		
		memset(lifter.stall_times, 0, sizeof(lifter.stall_times));
		
		memset(bomb_claw.stall_times, 0, sizeof(bomb_claw.stall_times));
		
		check.motor_check_time = millis();
		check.motor_check_time_record = DISABLE;
	}
	else if(millis() - check.motor_check_time > 6000)//6s�������ζ�ת
	{
		check.motor_check_time_record = ENABLE;		
		
		if((trailer.stall_times > 1)      ||
			 (lifter.stall_times[0] > 1)    ||
		   (lifter.stall_times[1] > 1)    ||
			 (bomb_claw.stall_times[0] > 1) ||
		   (bomb_claw.stall_times[1] > 1) ||
		   (bomb_claw.stall_times[2] > 1) ||
		   (bomb_claw.stall_times[3] > 1))
		{
			check.motor_serious_stall = YES;
		}
		else check.motor_serious_stall = NO;
	}
	
//	else if(trailer.stall_times > 4)
//	{
//	  check.motor_serious_stall = YES;
//	}
//	else 
//	{
//		check.motor_serious_stall = NO;
//	}
	
}

void sys_check(void)
{
	if(check.check_enable)
	{
		sensor_check();//���������
	  actuator_check();//ִ��Ԫ�����
	}
	else if(check.step_run_once) //S2�����²�״̬��ԭ
	{
		CCTV_CLIMB_TUBE;
		MAGAZINE_CLOSE;                          //���ָǺ���
		GIM_SERVO_ZERO;	
		TUBE_CLAW_LOOSE;                         //����צ��ǿ���ſ�	
		
		BOMB_CLAW_MOVE_MID;
	  BOMB_CLAW_ROLL_ENTIRE;        
    BOMB_CLAW_PULL;                          //�ջ�ȡ��צ��	
		BOMB_CLAW_CATCH;                         //ȡ��צ��ץ
		BOMB_EJECT_PULL;				                 //ȡ�������ջ�				
		trailer.ctrl_mode = TRAILER_PULL;        //�ϳ��ջ�		
    TRAILER_HOOK_CAPTURE;                       //�������׻���		
		TRAILER_HOOK_PULL;                       //�������׻���		
    ducted.ctrl_mode = DUCTED_ALL_OFF;       //�غ���			
    lifter.ctrl_mode = LIFTER_ZERO;          //��ܻر궨��λ��	
		
    climb_tube_param_init();//���˲�����ʼ��		
		
		memset(&check, 0, sizeof(check_t));
		check.next_step_enable = ENABLE;
//		check.step_run_once = DISABLE;//ֻ��һ��
//		check.check_step = 0;
	}
	
  motor_offline_check();//���У�
	motor_stall_check();//���У�
}
