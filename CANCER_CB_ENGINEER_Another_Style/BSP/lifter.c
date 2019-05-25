#include "lifter.h"

/* get information task period time (ms) */
#define LIFTER_RESET_SPD_PERIOD 1 //�����ĸ�ʱ���

/* key acceleration time */
#define LIFTER_RESET_ACC_TIME   100  //ms

lifter_t lifter;

void lifter_param_init(void)
{	
	for(u8 k = 0; k < 2; k++)
	{
		pidckz_reset(&pid_lifter_position[k]);
		pidckz_reset(&pid_lifter_speed[k]);
	
		pidckz_param_set(&pid_lifter_position[k], 8000, 7000, 7.0f, 0.0f, 0.0f);            
		pidckz_param_set(&pid_lifter_speed[k], 8000, 7000, 7.0f, 0.03f, 0.0f);	            
	}	
	
	ramp_init(&reset_lifter_spd_ramp, LIFTER_RESET_ACC_TIME/LIFTER_RESET_SPD_PERIOD);
	
	memset(lifter.ecd_Record, ENABLE, sizeof(lifter.ecd_Record));
	
	/*��λ��תʱ���¼��־λ*/		
  lifter.stall_time_record = ENABLE;
  /*��λ�ٶȳ�ʼ��*/
	lifter.reset_speed[L] = 2000;
	lifter.reset_speed[R] = -2000;
	/*�Լ���λ���̧��һ���ȡ����λ�õ��ı�־λ*/
	lifter.lift_for_bomb_claw_once = ENABLE;
	/*����Ŀ��λ*/
	lifter.target_tube_entire[L] = -35700;
	lifter.target_tube_entire[R] =  35700;
	lifter.target_tube_bit[L] = -1500;
	lifter.target_tube_bit[R] =  1500;	
	
	lifter.target_bomb_entire[L] = -23000;
	lifter.target_bomb_entire[R] =  23000;
		
	lifter.target_bomb_bit_vision[L] = -17000;
	lifter.target_bomb_bit_vision[R] =  17000;
	
  lifter.target_bomb_bit_normal[L] = -11000;
	lifter.target_bomb_bit_normal[R] =  11000;
	
	lifter.target_bomb_bit_scan[L] = -4100;
	lifter.target_bomb_bit_scan[R] =  4100;
	
	lifter.target_bomb_bit[L] = lifter.target_bomb_bit_normal[L];
	lifter.target_bomb_bit[R] = lifter.target_bomb_bit_normal[R];	

  /*���������ϵ�ʱĦ������̶�*/	
	lifter.target_zero_climb_up[L] = -200;
	lifter.target_zero_climb_up[R] = 200;	
	
	/*���������µ�ʱĦ������̶�*/	
	lifter.target_zero_climb_down[L] = -870;
	lifter.target_zero_climb_down[R] = 870;	
	
	/*��������µ����ֵ*/	
	lifter.target_zero_normal[L] = -600;
	lifter.target_zero_normal[R] = 600;	
	
	lifter.target_zero[L] = lifter.target_zero_normal[L];
	lifter.target_zero[R] = lifter.target_zero_normal[R];	
		
	lifter.target_feed_entire[L] = -20000;
	lifter.target_feed_entire[R] =  20000;
}

void lifter_cas_ctrl(int32_t target_angle[])//��������
{	                                                  
	for(u8 k = 0; k < 2; k++)
	{
		lifter.speed_ref[k] = pidckz_calc(&pid_lifter_position[k], lifter.total_angle[k], target_angle[k]);           //�⻷(λ�û�)PID����
		lifter.current[k] = pidckz_calc(&pid_lifter_speed[k], lifter.speed_fdb[k], lifter.speed_ref[k]);         //PID����
	
		if((abs(lifter.current[k]) > 7000) || lifter.across_stall_protect[k])
		{
			if(lifter.stall_protect_record[k])
			{
				lifter.stall_protect_time[k] = millis();
				lifter.stall_protect_record[k] = DISABLE;
			} 
			if((millis() - lifter.stall_protect_time[k] > 2000) &&
				 (millis() - lifter.stall_protect_time[k] < 2080))
			{
				lifter.across_stall_protect[k] = ENABLE;
				
				if(lifter.current[k] > 0)
				{
				  lifter.current[k] = 3000;
				}
				else lifter.current[k] = -3000;
			}
			else if(millis() - lifter.stall_protect_time[k] > 2080)
			{
		  	lifter.stall_protect_record[k] = ENABLE;
				lifter.across_stall_protect[k] = DISABLE;
				lifter.stall_times[k] ++;//��ɱ���һ�� ��ת����++
			}				
		}
		else 
		{
			lifter.stall_protect_record[k] = ENABLE;
			lifter.across_stall_protect[k] = DISABLE;
		}
	}
}

void lifter_spd_ctrl(int16_t target_speed[])//�ٶȻ�����(���ڸ�λ)
{	                                                  
	for(u8 k = 0; k < 2; k++)
	{
		lifter.current[k] = pidckz_calc(&pid_lifter_speed[k], lifter.speed_fdb[k], target_speed[k]);         //PID����
	}
}

void lifter_data_handler(u8 k)//��Ȧ���ĺ�������CAN�ж���
{	
	if(lifter.reset)
	{
		if(lifter.ecd_Record[k] == ENABLE)
		{
			lifter.ecd_Record[k] = DISABLE;
			lifter.offset_ecd[k] = lifter.angle_fdb[k];  //ֻ��¼һ�γ�ʼƫ��
			lifter.las_ecd[k] = lifter.angle_fdb[k];
			lifter.cur_ecd[k] = lifter.angle_fdb[k];
		}
		
		lifter.las_ecd[k] = lifter.cur_ecd[k];
		lifter.cur_ecd[k] = lifter.angle_fdb[k];
		
		if(lifter.cur_ecd[k] - lifter.las_ecd[k] > 4096)
		{
			lifter.round_cnt[k]--;
		}
		else if(lifter.cur_ecd[k] - lifter.las_ecd[k] < -4096)
		{
			lifter.round_cnt[k]++;
		}		
		lifter.total_ecd[k] = lifter.round_cnt[k] * 8192 + lifter.cur_ecd[k] - lifter.offset_ecd[k];
		lifter.total_angle[k] = lifter.total_ecd[k] / ENCODER_ANGLE_RATIO;//�������� �������Ķ�����ת�ӵ�ת���ܽǶ�
	}
}

bool from_lifter_zero = YES;
bool from_lifter_bomb_entire = YES;//�������ĳ�ȫ�ֱ��� ���Զ���λ��

bool flag_lifter_zero = ENABLE;

bool flag_lifter_tube_entire = ENABLE;
bool flag_lifter_tube_bit = ENABLE;
	
bool flag_lifter_bomb_entire = ENABLE;	
bool flag_lifter_bomb_bit = ENABLE;
bool flag_lifter_feed_entire = ENABLE;	
void lifter_ctrl(void)
{
	static bool flag_lifter_zero = ENABLE;

	static bool flag_lifter_tube_entire = ENABLE;
	static bool flag_lifter_tube_bit = ENABLE;
	
	static bool flag_lifter_bomb_entire = ENABLE;	
	static bool flag_lifter_bomb_bit = ENABLE;
	
  static bool flag_lifter_feed_entire = ENABLE;	
	
	switch(lifter.ctrl_mode)
	{		
/*----------------------------------��������----------------------------------*/		

		case TUBE_LIFTER_ENTIRE://���˿��̧��
		{									
			if(flag_lifter_tube_entire)
			{ /*��¼��ǰ���λ��*/
				lifter.target_angle[L] = lifter.total_angle[L];
   		  lifter.target_angle[R] = lifter.total_angle[R];	
			  /*��־λ����*/
				flag_lifter_tube_entire = 0;
				flag_lifter_zero = 1;
			}
				
			if(lifter.target_angle[L] > lifter.target_tube_entire[L])
			{
				lifter.target_angle[L] = lifter.total_angle[L] - 1000;					
			}
			else lifter.target_angle[L] = lifter.target_tube_entire[L];
			
			if(lifter.target_angle[R] < lifter.target_tube_entire[R])
			{
				lifter.target_angle[R] = lifter.total_angle[R] + 1000;			
			}
			else lifter.target_angle[R] = lifter.target_tube_entire[R];
			
		}break;
		
	  case TUBE_LIFTER_BIT://���˿��̧��һ���
		{
			if(flag_lifter_tube_bit)
			{ /*��¼��ǰ���λ��*/
				lifter.target_angle[L] = lifter.total_angle[L];
   		  lifter.target_angle[R] = lifter.total_angle[R];	
			  /*��־λ����*/				
				flag_lifter_tube_bit = 0;
				flag_lifter_zero = 1;
			}

			if(lifter.target_angle[L] > lifter.target_tube_bit[L])
			{
				lifter.target_angle[L] = lifter.total_angle[L] - 1000;					
			}
			else lifter.target_angle[L] = lifter.target_tube_bit[L];
			
			if(lifter.target_angle[R] < lifter.target_tube_bit[R])
			{
				lifter.target_angle[R] = lifter.total_angle[R] + 1000;			
			}
			else lifter.target_angle[R] = lifter.target_tube_bit[R];
		
		}break;
/*----------------------------------����ȡ��----------------------------------*/		
		
		case BOMB_LIFTER_ENTIRE://ȡ�����̧��
		{						
			if(flag_lifter_bomb_entire)
			{ /*��¼��ǰ���λ��*/
				lifter.target_angle[L] = lifter.total_angle[L];
   		  lifter.target_angle[R] = lifter.total_angle[R];	
			  /*��־λ����*/				
				flag_lifter_bomb_entire = 0;
				flag_lifter_bomb_bit = 1;	
				flag_lifter_zero = 1;
				
				from_lifter_bomb_entire = YES;
				from_lifter_zero = NO;
			}
							
			if(lifter.target_angle[L] > lifter.target_bomb_entire[L])
			{
				lifter.target_angle[L] = lifter.total_angle[L] - 800;					
			}
			else lifter.target_angle[L] = lifter.target_bomb_entire[L];
			
			if(lifter.target_angle[R] < lifter.target_bomb_entire[R])
			{
				lifter.target_angle[R] = lifter.total_angle[R] + 800;			
			}
			else lifter.target_angle[R] = lifter.target_bomb_entire[R];
			
		}break;
		
	  case BOMB_LIFTER_BIT://ȡ�����̧��һ���
		{
			if(flag_lifter_bomb_bit)
			{ /*��¼��ǰ���λ��*/
				lifter.target_angle[L] = lifter.total_angle[L];
   		  lifter.target_angle[R] = lifter.total_angle[R];	
			  /*��־λ����*/				
				flag_lifter_bomb_bit = 0;
				flag_lifter_bomb_entire = 1;	
				flag_lifter_zero = 1;
			}

			if((lifter.target_angle[L] > lifter.target_bomb_bit[L]) && (from_lifter_zero))//��zero��
			{
				lifter.target_angle[L] = lifter.total_angle[L] - 600;					
			}
			else if((lifter.target_angle[L] < lifter.target_bomb_bit[L]) && (from_lifter_bomb_entire))//��entire��
			{
				lifter.target_angle[L] = lifter.total_angle[L] + 600;				
			}
			else lifter.target_angle[L] = lifter.target_bomb_bit[L];
			
			if((lifter.target_angle[R] < lifter.target_bomb_bit[R]) && (from_lifter_zero))//��zero��
			{
				lifter.target_angle[R] = lifter.total_angle[R] + 600;			
			}
			else if((lifter.target_angle[R] > lifter.target_bomb_bit[R]) && (from_lifter_bomb_entire))//��entire��
			{
				lifter.target_angle[R] = lifter.total_angle[R] - 600;						
			}
			else lifter.target_angle[R] = lifter.target_bomb_bit[R];

		}break;		
/*----------------------------------���ڲ���----------------------------------*/		
		
		case FEED_LIFTER_ENTIRE://�������̧��
		{	
			if(flag_lifter_feed_entire)
			{ /*��¼��ǰ���λ��*/
				lifter.target_angle[L] = lifter.total_angle[L];
   		  lifter.target_angle[R] = lifter.total_angle[R];	
			  /*��־λ����*/				
				flag_lifter_feed_entire = 0;
				flag_lifter_zero = 1;
			}
			
			if(lifter.target_angle[L] > lifter.target_feed_entire[L])
			{
				lifter.target_angle[L] = lifter.total_angle[L] - 900;					
			}
			else lifter.target_angle[L] = lifter.target_feed_entire[L];
			
			if(lifter.target_angle[R] < lifter.target_feed_entire[R])
			{
				lifter.target_angle[R] = lifter.total_angle[R] + 900;			
			}
			else lifter.target_angle[R] = lifter.target_feed_entire[R];
			
		}break;				
/*----------------------------------�����----------------------------------*/		

		case LIFTER_ZERO:	
		{		
			if(flag_lifter_zero)
			{ /*��¼��ǰ���λ��*/
				lifter.target_angle[L] = lifter.total_angle[L];
   		  lifter.target_angle[R] = lifter.total_angle[R];	
			  /*��־λ����*/				
				flag_lifter_zero = 0;
				
				flag_lifter_tube_entire = 1;
				flag_lifter_tube_bit = 1;
				
				flag_lifter_bomb_entire = 1;
				flag_lifter_bomb_bit = 1;			

				flag_lifter_feed_entire = 1;	
				
				from_lifter_bomb_entire = NO;
				from_lifter_zero = YES;
			}
							
			if(climb_tube.ctrl_mode == CLIMEB_DOWN_OPTIC_MODE ||
				 climb_tube.ctrl_mode == CLIMEB_DOWN_GYRO_MODE)//�¸˵�ʱ���ն���
  		{
				lifter.target_zero[L] = lifter.target_zero_climb_down[L];
				lifter.target_zero[R] = lifter.target_zero_climb_down[R];	
			}
			else if(climb_tube.ctrl_mode == CLIMEB_UP_OPTIC_MODE ||
				      climb_tube.ctrl_mode == CLIMEB_UP_GYRO_MODE)//���˵�ʱ���ն���
			{
				lifter.target_zero[L] = lifter.target_zero_climb_up[L];
				lifter.target_zero[R] = lifter.target_zero_climb_up[R];	
			}
			else
			{
				lifter.target_zero[L] = lifter.target_zero_normal[L];
				lifter.target_zero[R] = lifter.target_zero_normal[R];	
			}			

		  if(lifter.target_angle[L] < lifter.target_zero[L])
			{
        if(climb_tube.ctrl_mode == CLIMEB_UP_OPTIC_MODE || 
				   climb_tube.ctrl_mode == CLIMEB_UP_GYRO_MODE || 
				   climb_tube.ctrl_mode == CLIMEB_DOWN_OPTIC_MODE ||
					 climb_tube.ctrl_mode == CLIMEB_DOWN_GYRO_MODE)//�����˵�ʱ���յ��ٶ�Ҫ��һ��
				{
				  lifter.target_angle[L] = lifter.total_angle[L] + 2000;
				}
        else lifter.target_angle[L] = lifter.total_angle[L] + 400;				
			}
			else lifter.target_angle[L] = lifter.target_zero[L];
			
			if(lifter.target_angle[R] > lifter.target_zero[R])
			{			 
				if(climb_tube.ctrl_mode == CLIMEB_UP_OPTIC_MODE || 
					 climb_tube.ctrl_mode == CLIMEB_UP_GYRO_MODE ||
				   climb_tube.ctrl_mode == CLIMEB_DOWN_OPTIC_MODE ||
					 climb_tube.ctrl_mode == CLIMEB_DOWN_GYRO_MODE)//�����˵�ʱ���յ��ٶ�Ҫ��һ��
				{
					lifter.target_angle[R] = lifter.total_angle[R] - 2000;	
				}
				else lifter.target_angle[R] = lifter.total_angle[R] - 400;
			}
			else lifter.target_angle[R] = lifter.target_zero[R];

		
		}break;
							
		default: break;
	}  	
	
  /*��������֢*/			
//	VAL_LIMIT(lifter.target_angle[L], lifter.target_tube_entire[L], lifter.target_zero[L]);
//	VAL_LIMIT(lifter.target_angle[R], lifter.target_zero[R], lifter.target_tube_entire[R]);
	
	if(system_mode == SAFETY_MODE)
	{
		memset(lifter.current,0,sizeof(lifter.current));//�⼸���������ֵ����
  }
	else lifter_cas_ctrl(lifter.target_angle);			  //��������			
}	

void lifter_reset(void)
{
	
	for(u8 k = 0; k < 2; k++)
	{
		lifter.target_speed[k] = lifter.reset_speed[k] * ramp_calc(&reset_lifter_spd_ramp);
	}
	lifter_spd_ctrl(lifter.target_speed);	

	
	if(!lifter.reset)
	{
		if((abs(lifter.speed_fdb[L]) < 50) && 
			 (abs(lifter.speed_fdb[R]) < 50) &&
		   (abs(lifter.current[L]) > 7500) &&
		   (abs(lifter.current[R]) > 7500))
		{
			if(lifter.stall_time_record)
			{
				lifter.stall_time = millis();
				lifter.stall_time_record = DISABLE;
			}
			if(millis() - lifter.stall_time > 200)
			{
			  lifter.reset = ENABLE;
		  	lifter.reset_speed[L] = 0;
	      lifter.reset_speed[R] = 0;				
			}		
		}
		else lifter.stall_time_record = ENABLE;
	}
		
}

void lifter_task(void)
{
	if(lifter.reset)//̧����λ��� ̧����һ���ȡ����λ
	{
		lifter_ctrl();
		
		if(!bomb_claw.ALL_reset)
		{
			lifter.ctrl_mode = BOMB_LIFTER_BIT;
			if((abs(lifter.target_bomb_bit[L] - lifter.total_angle[L]) < 100) || 
				 (abs(lifter.target_bomb_bit[R] - lifter.total_angle[R]) < 100))//̧����Ŀ��Ƕ�
			{
				lifter.flag_bomb_claw_reset = ENABLE;
//				GIM_SERVO_ZERO;
			}			
		}
//		else if(lifter.lift_for_bomb_claw_once && (abs(bomb_claw.total_angle[Y] - bomb_claw.target_half[Y]) < 100))//	ȡ��Ҳ��λ����		
//		{
//			lifter.ctrl_mode = LIFTER_ZERO;//�������������
//			lifter.lift_for_bomb_claw_once = DISABLE;
//			sys_all_reset = YES;
//			
//		}
	}
	else lifter_reset();
}
