#include "feed_bomb.h"

feed_bomb_t feed_bomb;

void feed_bomb_param_init(void)
{
  feed_bomb.turn_around_record = ENABLE;
	feed_bomb.turn_around_enable = ENABLE;
	
	feed_bomb.exit_enable = DISABLE;
	MAGAZINE_CLOSE;
}

void feed_bomb_task(void)
{
	keyboard_feed_bomb_handler();//keyboard	
		
	if(feed_bomb.feed_bomb_enable == ENABLE)	
	{
		switch(feed_bomb.ctrl_mode)
		{	
			case QUIT_FEED_MODE://�˳�ι��ģʽ
			{
				feed_bomb.turn_around_record = ENABLE;
				feed_bomb.turn_around_enable = ENABLE;
				
				feed_bomb.sight_enable = DISABLE;
				GIM_SERVO_ZERO;	
				flag_feed_bomb_sight = 0;
				
//				servo_feed_bomb_start_delay_record = ENABLE;	
//				servo_feed_bomb_lift_up_delay_record = ENABLE;	
				chassis.ass_mode_enable = DISABLE;       //�رճ�βģʽ	
				chassis.ctrl_mode = CHASSIS_FOLLOW;  //���̻�еģʽ
       		
				feed_bomb.vision_enable = DISABLE;
				vision.enable = DISABLE;
			  flag_feed_bomb_vision = 0;//�´�Ĭ�ϴ��Ӿ�
				
        feed_bomb.lifter_enable = DISABLE;	
        flag_magazine_lifter = 0;//�´�Ĭ��̧����			
				
				feed_bomb.exit_enable = DISABLE;				
				MAGAZINE_CLOSE;
				
//				/*���µ�ʱ����ʱһ������̨���ת�������ٽ���� ���ϾͲ��ù���*/
				if(climb_tube.location == ON_GROUND)
				{
//					if(servo_feed_bomb_end_delay_record)//��ʱһС��ʱ���ö��ת������
//					{					
//						servo_feed_bomb_end_delay_time = millis();
//						servo_feed_bomb_end_delay_record = 0;
//					}
//					else if(millis() - servo_feed_bomb_end_delay_time > 1000)
//					{
						lifter.ctrl_mode = LIFTER_ZERO;	   //��ܻر궨��λ��					
						feed_bomb.feed_bomb_enable = DISABLE;	
//					}				
				}
				else feed_bomb.feed_bomb_enable = DISABLE;	
				
			}break;	
			
			case AIM_FEED_MODE://��׼ι��ģʽ
			{	
//				servo_feed_bomb_end_delay_record = 1;	
				chassis.ass_mode_enable = ENABLE;       //������βģʽ	
				
				if(feed_bomb.turn_around_record && feed_bomb.turn_around_enable)		//ģ�ⰴ��
				{		
					flag_turn_around = 1;
					chassis.position_temp = imu.yaw;
					ramp_init(&turn_around_ramp, TUR_ARO_TIME/INFO_GET_PERIOD);		
					feed_bomb.turn_around_record = DISABLE;
				}
				
				feed_bomb.sight_enable = ENABLE;        //�ɿ���̨ Ĭ�Ͽ�С���� ���ֶ��������Ҫ������Ŀ��

					
//				/*���ϵ�ʱ���ܲ�̧�� ���µ�ʱ����̧��*/
//				if(servo_feed_bomb_start_delay_record)//��ʱһС��ʱ���ö��ת������
//				{					
//					servo_feed_bomb_start_delay_time = millis();
//					servo_feed_bomb_start_delay_record = 0;
//				}
//				else if(millis() - servo_feed_bomb_start_delay_time > 1000)
//				{				
					if(climb_tube.location == ON_GROUND)
					{
						feed_bomb.lifter_enable = ENABLE;//ʹ�ܿ��̧��
						
						if(((abs(lifter.target_feed_entire[L] - lifter.total_angle[L]) < 100) || 
			          (abs(lifter.target_feed_entire[R] - lifter.total_angle[R]) < 100)) && 
						    (ramp_calc(&turn_around_ramp) == 1))//̧����Ŀ��Ƕ�//̧�����������Ӿ����ڵ���ͷ��
						{
							feed_bomb.vision_enable = ENABLE;
							
							if(vision.enable == DISABLE)
							{
								chassis.ctrl_mode = CHASSIS_SEPARATE;  //���̻�еģʽ
							}
							else if(vision.captured)//ץ��ͼ����
							{
								chassis.ctrl_mode = VISION_FEED_BOMB;
						  }	
							else chassis.ctrl_mode = CHASSIS_SEPARATE;  //���̻�еģʽ
						}
						else 
						{
							feed_bomb.vision_enable = DISABLE;
							vision.enable = DISABLE;
							flag_feed_bomb_vision = 0; //�´�Ĭ�ϴ��Ӿ�
						}
						
						if(lifter.total_angle[R] > lifter.target_bomb_bit[R] || 
							 lifter.total_angle[L] < lifter.target_bomb_bit[L])
						{//̧��һ���߶ȾͿ���ʹ�ܳ��ڿ�����
							feed_bomb.exit_enable = ENABLE;
						}	

//					}			
				}	
				else //������������
				{
					feed_bomb.vision_enable = DISABLE;//���ϾͲ����Ӿ��˰� 
					vision.enable = DISABLE;
			    flag_feed_bomb_vision = 0; //�´�Ĭ�ϴ��Ӿ�
					
					feed_bomb.sight_enable = ENABLE;
				  feed_bomb.exit_enable = ENABLE;
				}	
			}break;			
			
			default: break;
		}
	}
}

