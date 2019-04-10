#include "trailer.h"

/* get information task period time (ms) */
#define TRAILER_RESET_SPD_PERIOD 3 //�����ĸ�ʱ���

/* key acceleration time */
#define TRAILER_RESET_ACC_TIME   100  //ms

trailer_t trailer;

void trailer_param_init(void)
{
	memset(&trailer, 0, sizeof(trailer_t));
	
	ramp_init(&reset_trailer_spd_ramp, TRAILER_RESET_ACC_TIME/TRAILER_RESET_SPD_PERIOD);
	
	pidckz_reset(&pid_trailer_position);
	pidckz_reset(&pid_trailer_speed);
	
	pidckz_param_set(&pid_trailer_position, 8000, 1000, 7.5f, 0.02f, 0.0f);            //���̵��λ�û�
	pidckz_param_set(&pid_trailer_speed, 8000, 1000, 15.2f, 0.0f, 0.0f);	            //���̵���ٶȻ�

  /*��λ��תʱ���¼��־λ*/		
  trailer.stall_time_record = ENABLE;
	trailer.stall_time_record = ENABLE;
  /*��λ�ٶȳ�ʼ��*/
	trailer.reset_speed = -2000;
  /*���ֱ�־λ��1*/
  trailer.ecd_record = ENABLE;
	/*λ�ó�ʼ��*/
	trailer.target_entire = 16500;
	trailer.target_zero = 1000;

}

void trailer_spd_ctrl(int16_t target_speed)
{
	trailer.current = pidckz_calc(&pid_trailer_speed, trailer.speed_fdb, target_speed);         //PID����
}	

void trailer_cas_ctrl(int32_t target_angle)
{	
	trailer.speed_ref = pidckz_calc(&pid_trailer_position, trailer.total_angle, target_angle);           //�⻷(λ�û�)PID����
	trailer.current = pidckz_calc(&pid_trailer_speed, trailer.speed_fdb, trailer.speed_ref);            //PID����

		if(abs(trailer.current) > 8000)
		{
			if(trailer.stall_protect_record)
			{
				trailer.stall_protect_time = millis();
				trailer.stall_protect_record = DISABLE;
			}
			if(millis() - trailer.stall_protect_time > 888)
			{
				if(trailer.current > 0)
				{
				  trailer.current = 2333;
				}
				else trailer.current = -2333;
			}	     	
		}
	  else trailer.stall_protect_record = ENABLE;
}

void trailer_data_handler(void)//��Ȧ���ĺ�������CAN�ж���
{	
	if(trailer.reset)	
	{
		if(trailer.ecd_record == ENABLE)
		{
			trailer.ecd_record = 0;
			trailer.offset_ecd = trailer.angle_fdb;  //ֻ��¼һ�γ�ʼƫ��
			trailer.las_ecd = trailer.angle_fdb;
			trailer.cur_ecd = trailer.angle_fdb;
		}
		
		trailer.las_ecd = trailer.cur_ecd;
		trailer.cur_ecd = trailer.angle_fdb;
		
		if(trailer.cur_ecd - trailer.las_ecd > 4096)
		{
			trailer.round_cnt--;
		}
		else if(trailer.cur_ecd - trailer.las_ecd < -4096)
		{
			trailer.round_cnt++;
		}
		
		trailer.total_ecd = trailer.round_cnt * 8192 + trailer.cur_ecd - trailer.offset_ecd;
		trailer.total_angle = trailer.total_ecd / ENCODER_ANGLE_RATIO;//�������� �������Ķ�����ת�ӵ�ת���ܽǶ�
	}
}


void trailer_ctrl()
{
	static bool flag_trailer_zero = 1;
	static bool flag_trailer_entire = 1;
	
	switch(trailer.ctrl_mode)
	{
		case TRAILER_PULL:
		{			
			if(flag_trailer_zero)
			{ /*��¼��ǰ���λ��*/
				trailer.target_angle = trailer.total_angle;
			  /*��־λ����*/
				flag_trailer_entire = 1;
				flag_trailer_zero = 0;
			}
			
			if(trailer.target_angle > trailer.target_zero)
			{
				trailer.target_angle = trailer.total_angle - 300;			
			}
			else trailer.target_angle = trailer.target_zero;
			
		}break;
		
		case TRAILER_PUSH:
		{
			if(flag_trailer_entire)
			{ /*��¼��ǰ���λ��*/
				trailer.target_angle = trailer.total_angle;
			  /*��־λ����*/
				flag_trailer_entire = 0;
				flag_trailer_zero = 1;
			}
			
			if(trailer.target_angle < trailer.target_entire)
			{
				trailer.target_angle = trailer.total_angle + 300;			
			}
			else trailer.target_angle = trailer.target_entire;				

		}break;
			
		default: break;//Default do nothing
	}
	
	if(system_mode == SAFETY_MODE || trailer.ctrl_mode == TRAILER_RELAX) trailer.current = 0; //
	else trailer_cas_ctrl(trailer.target_angle);			                                        //
   	
}	

void trailer_reset(void)
{
	trailer.target_speed = trailer.reset_speed * ramp_calc(&reset_trailer_spd_ramp);
	trailer_spd_ctrl(trailer.target_speed);	

	if(!trailer.reset)
	{
		if((abs(trailer.speed_fdb) < 50) && (abs(trailer.current) > 5000))
		{
			if(trailer.stall_time_record)
			{
				trailer.stall_time = millis();
				trailer.stall_time_record = DISABLE;
			}
			if(millis() - trailer.stall_time > 150)
			{
			  trailer.reset = ENABLE;
		  	trailer.reset_speed = 0;
			}		
		}
		else trailer.stall_time_record = ENABLE;
	}	
}

void trailer_task(void)
{
	if(trailer.reset)
	{
		trailer_ctrl();
	}
	else trailer_reset();
}
