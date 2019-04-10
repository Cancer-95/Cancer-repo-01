#include "chassis.h"

/* chassis task global parameter */
chassis_t chassis;

void chassis_param_init()
{
	memset(&chassis, 0, sizeof(chassis_t));
	
	pidckz_reset(&pid_chassis_speed);
	pidckz_reset(&pid_chassis_angle);
	pidckz_param_set(&pid_chassis_speed, 400, 300, 0.2f, 0.0f, 0.0f);                      //������ģʽYAW�ڻ������趨
	pidckz_param_set(&pid_chassis_angle, 4000, 50, 130.0f, 0.0f, 0.0f);	                //������ģʽYAW�⻷�����趨
	
	pidckz_reset(&pid_vision_chassis_vy);
//	pidckz_param_set(&pid_vision_chassis_vy, 500, 100, 2.0f, 0.0045f, 1.8f);   

	
	  for(u8 k = 0; k < 4; k++)
	{
	  pidckz_reset(&pid_wheel_speed[k]);		
		pidckz_param_set(&pid_wheel_speed[k], 10000, 1000, 9.0f, 0.4f, 16.0f);	//for���̳���
	}
}

void chassis_task()
{
	keyboard_chassis_handler();
	remote_ctrl_chassis_handler();
	
	switch (chassis.ctrl_mode)
	{
		case CHASSIS_STOP://�ٶ�����ס	
		{	
      chassis_stop_handler();
		}break;
			
		case MOVE_FORWARD://��ǰ��
		{	
      chassis_forward_handler();
		}break;
		
		case MOVE_BACKWARD://�����
		{	
      chassis_backward_handler();
		}break;
		
		case VISION_GET_BOMB://�Ӿ���λ(ȡ��)λ�û�ģʽ
		{	
      chassis_vision_get_bomb_handler();
		}break;
			
		case VISION_FEED_BOMB://�Ӿ���λ(����)
		{	     
			chassis_vision_feed_bomb_handler();
		}break;
		
		case MOVE_CLOCKWISE://����˳ʱ��
		{	
      chassis_clockwise_handler();
		}break;
		
		case MOVE_ANTICLOCKWISE://������ʱ��
		{	
      chassis_anticlockwise_handler();
		}break;
		
		case CHASSIS_SEPARATE://��еģʽ
		{	
      chassis_separate_handler();
		}break;
						
		case CHASSIS_FOLLOW://������ģʽ
    {
			if(flag_gyro_blow_up)//������ը��ִ�л�еģʽ
	    {
				chassis_separate_handler();
			}
			else chassis_follow_handler();
    }break;	
		
		default://�ٶ�����ס																																									
		{
      chassis_stop_handler();
		}break;
	}
	
	  mecanum_calc(chassis.vx, chassis.vy, chassis.vw, chassis.wheel_speed_ref);    //�����������ٶȵ�Ŀ��ֵ�ֽ�õ��ĸ����ӵ�Ŀ��ֵ
		
	  for(u8 k = 0; k < 4; k++)
	  chassis.current[k] = pidckz_calc(&pid_wheel_speed[k], chassis.wheel_speed_fdb[k], chassis.wheel_speed_ref[k]);                            //PID����
	
		if (system_mode == SAFETY_MODE)
	  {
      memset(chassis.current, 0, sizeof(chassis.current));
  	}	
}

/**
  * @brief mecanum chassis velocity decomposition
  * @param input : ?=+vx(mm/s)  ?=+vy(mm/s)  ccw=+vw(deg/s)
  *        output: every wheel speed(rpm)
  * @note  1=FR 2=FL 3=BL 4=BR
  */
void mecanum_calc(float vx, float vy, float vw, int16_t Wheel_speed[])//���ֹ�ʽ����
{
  static bool rotation_center_gimbal = 0;//��̨�Ƿ��ڵ������ĵı�־λ
  static float rotate_ratio_fr;
  static float rotate_ratio_fl;
  static float rotate_ratio_bl;
  static float rotate_ratio_br;
  static float wheel_rpm_ratio;
	int16_t wheel_rpm[4];
  float   max = 0;
	
	/*�����ߵ���ת�ж�*/
//	if(chassis.ctrl_mode == MOVE_CLOCKWISE || 
//		 chassis.ctrl_mode == MOVE_ANTICLOCKWISE || flag_tube_claw_catch) 
	if(flag_tube_claw_catch) 
	{
		rotation_center_gimbal = 1;
	}
	else rotation_center_gimbal = 0;
	
  /* ��̨��������������Ҫ���в���*/
  if (rotation_center_gimbal)
  { 
		rotate_ratio_fr = ((WHEELBASE + WHEELTRACK) / 2.0f - GIMBAL_X_OFFSET + GIMBAL_Y_OFFSET)/RADIAN_COEF;
    rotate_ratio_fl = ((WHEELBASE + WHEELTRACK) / 2.0f - GIMBAL_X_OFFSET - GIMBAL_Y_OFFSET)/RADIAN_COEF;
    rotate_ratio_bl = ((WHEELBASE + WHEELTRACK) / 2.0f + GIMBAL_X_OFFSET - GIMBAL_Y_OFFSET)/RADIAN_COEF;
    rotate_ratio_br = ((WHEELBASE + WHEELTRACK) / 2.0f + GIMBAL_X_OFFSET + GIMBAL_Y_OFFSET)/RADIAN_COEF;
  }
  else
  {
    rotate_ratio_fr = ((WHEELBASE + WHEELTRACK) / 2.0f) / RADIAN_COEF;
    rotate_ratio_fl = rotate_ratio_fr;
    rotate_ratio_bl = rotate_ratio_fr;
    rotate_ratio_br = rotate_ratio_fr;
  }
  wheel_rpm_ratio = 60.0f/(PERIMETER*CHASSIS_DECELE_RATIO);		 																							//	60/�ܳ�*���ٱ�
  
	VAL_LIMIT(vx, -MAX_CHASSIS_VX_SPEED, MAX_CHASSIS_VX_SPEED);  																							//mm/s
  VAL_LIMIT(vy, -MAX_CHASSIS_VY_SPEED, MAX_CHASSIS_VY_SPEED);  																							//mm/s
  VAL_LIMIT(vw, -MAX_CHASSIS_VR_SPEED, MAX_CHASSIS_VR_SPEED);  																							//deg/s
  
	/*�ĸ����ת��*/   //Ĭ����������Ͻǿ�ʼ˳ʱ��0123 �ɸ���ʵ�������
  wheel_rpm[0] = (-vx - vy + vw * rotate_ratio_fr) * wheel_rpm_ratio;
  wheel_rpm[1] = ( vx - vy + vw * rotate_ratio_fl) * wheel_rpm_ratio;
  wheel_rpm[2] = ( vx + vy + vw * rotate_ratio_bl) * wheel_rpm_ratio;
  wheel_rpm[3] = (-vx + vy + vw * rotate_ratio_br) * wheel_rpm_ratio;

  /*�ҵ��ĸ����*/
  for (u8 i = 0; i < 4; i++)
  {
    if (abs(wheel_rpm[i]) > max)
      max = abs(wheel_rpm[i]);
  }
  /* equal proportion */
  if (max > MAX_WHEEL_RPM)
  {
    float rate = MAX_WHEEL_RPM / max;
    for (u8 i = 0; i < 4; i++)
      wheel_rpm[i] *= rate;
  }
  memcpy(Wheel_speed, wheel_rpm, 4*sizeof(int16_t));
}

/*--------------------------------�����������---------------------------------*/

void chassis_stop_handler(void)//�����ٶȻ���ס
{	
  chassis.vy = chassis.vx = chassis.vw = 0;
	if(chassis.force_push_in_enable)//�����ȡ���ƽ���
	{
	  chassis.vx = 100;
	}
  
	chassis.position_ref = imu.yaw;//��¼�����ǵĽǶȸ�������ģʽ��
}


void chassis_forward_handler(void)//������ǰ��
{
  chassis.vy = chassis.vw = 0;;
  chassis.vx = 200;
  	
  chassis.position_ref = imu.yaw;//��¼�����ǵĽǶȸ�������ģʽ��
}

void chassis_backward_handler(void)//���������
{
  chassis.vy = chassis.vw = 0;;
  chassis.vx = -200;
  	
  chassis.position_ref = imu.yaw;//��¼�����ǵĽǶȸ�������ģʽ��
}

void chassis_vision_get_bomb_handler(void)//λ�û������Ӿ���λ(ȡ��)  
{
  pidckz_calc(&pid_vision_chassis_vy, vision.y_ref, vision.y_fdb);
	
	if(pid_vision_chassis_vy.err > 32)
	{
		if(climb_tube.location == ON_GROUND) chassis.vy = 170;
		else chassis.vy = 150;
	}
	else if(pid_vision_chassis_vy.err < -32)
	{
	 if(climb_tube.location == ON_GROUND) chassis.vy = 170;
		chassis.vy = -150;
	}
	else chassis.vy = pid_vision_chassis_vy.out;
	
  chassis.vx = 80;
  chassis.vw = 0;
  chassis.position_ref = imu.yaw;//��¼�����ǵĽǶȸ�������ģʽ��
}

void chassis_vision_feed_bomb_handler(void)//�����Ӿ���λ(����)(����ȫ��еģʽ)
{
	pidckz_calc(&pid_vision_chassis_vy, vision.y_ref, vision.y_fdb);
	
	if(pid_vision_chassis_vy.err > 30)
	{
		chassis.vy = 170;
	}
	else if(pid_vision_chassis_vy.err < -30)
	{
		chassis.vy = -170;
	}
	else chassis.vy = pid_vision_chassis_vy.out;
	
//	chassis.vy = 0;
	
  chassis.vx = rc.vx * CHASSIS_RC_MOVE_RATIO_X + kb.vx * CHASSIS_KB_MOVE_RATIO_X;
	
  if(chassis.ass_mode_enable)//��βģʽ
	{
    chassis.vw = -(rc.vw * CHASSIS_RC_MOVE_RATIO_R + kb.vw * CHASSIS_KB_MOVE_RATIO_R);
	}
	else//��ͷģʽ
	{
    chassis.vw = rc.vw * CHASSIS_RC_MOVE_RATIO_R + kb.vw * CHASSIS_KB_MOVE_RATIO_R;
	}
  chassis.position_ref = imu.yaw;//��¼�����ǵĽǶȸ�������ģʽ��
}

void chassis_clockwise_handler(void)//��������˳ʱ��
{
  chassis.vy = chassis.vx = 0;  
  chassis.vw = 150;
	
  chassis.position_ref = imu.yaw;//��¼�����ǵĽǶȸ�������ģʽ��
}

void chassis_anticlockwise_handler(void)//����������ʱ��
{
  chassis.vy = chassis.vx = 0;
  chassis.vw = -150;
	
  chassis.position_ref = imu.yaw;//��¼�����ǵĽǶȸ�������ģʽ��
}

/*--------------------------------���̻�еģʽ��������ģʽ---------------------------------*/

void chassis_separate_handler(void)//��еģʽ
{
  chassis.vy = rc.vy * CHASSIS_RC_MOVE_RATIO_Y + kb.vy * CHASSIS_KB_MOVE_RATIO_Y;
  chassis.vx = rc.vx * CHASSIS_RC_MOVE_RATIO_X + kb.vx * CHASSIS_KB_MOVE_RATIO_X;
	
	if(chassis.force_push_in_enable)//�����ȡ���ƽ���
	{
	  chassis.vx += 80 ;
	}
	
  if(chassis.ass_mode_enable)//��βģʽ
	{
    chassis.vw = -(rc.vw * CHASSIS_RC_MOVE_RATIO_R + kb.vw * CHASSIS_KB_MOVE_RATIO_R);
	}
	else//��ͷģʽ
	{
    chassis.vw = rc.vw * CHASSIS_RC_MOVE_RATIO_R + kb.vw * CHASSIS_KB_MOVE_RATIO_R;
	}
	if(flag_spin_around)
	{
		chassis.vw = 80;
	}
	
	chassis.position_ref = imu.yaw;//��¼�����ǵĽǶȸ�������ģʽ��
}

void chassis_follow_handler(void)//������ģʽ
{  
	chassis.vy = rc.vy * CHASSIS_RC_MOVE_RATIO_Y + kb.vy * CHASSIS_KB_MOVE_RATIO_Y;
	chassis.vx = rc.vx * CHASSIS_RC_MOVE_RATIO_X + kb.vx * CHASSIS_KB_MOVE_RATIO_X;
	
	if(chassis.force_draft_out_enable)//������ϳ��γ������õ�
	{
		chassis.vx = -200 + chassis.vx;
	}
	
	if(flag_spin_around)
	{
		chassis.vw = 80;
		chassis.position_ref = imu.yaw;//��¼�����ǵĽǶȸ�������ģʽ��
	}
	else chassis_yaw_handler();//YAW�ᴦ��ó�vwֵ
}

void chassis_yaw_target(void)//��ȡY���Ŀ��ֵ
{	 
//	if(chassis.ass_mode_enable)//��βģʽ
//	{
//		if(rc.rc_enable) chassis.position_ref += (RC_Ctl.rc.ch0-RC_CH_VALUE_OFFSET)*0.0005;//ң����vw	
//		else if(kb.kb_enable) chassis.position_ref += RC_Ctl.mouse.x * 0.01;//����vw
//	}
//	else//��ͷģʽ
//	{
//		if(rc.rc_enable) chassis.position_ref += -(RC_Ctl.rc.ch0-RC_CH_VALUE_OFFSET)*0.0005;//ң����vw	
//		else if(kb.kb_enable) chassis.position_ref += -RC_Ctl.mouse.x * 0.01;//����vw
//	}
	
	if(rc.rc_enable) 
	{
		chassis.position_ref += -(RC_Ctl.rc.ch0-RC_CH_VALUE_OFFSET)*0.0007;//ң����vw			
	}
	else if(kb.kb_enable) 
	{
		if(kb.move_speed == SLOW_SPEED) 
		{
		  chassis.position_ref += -RC_Ctl.mouse.x * 0.0033;//����vw
		}
		else chassis.position_ref += -RC_Ctl.mouse.x * 0.01;//����vw
		
	}
	/*YAW��Ŀ��ֵȷ��Խ���ٽ�ֵ*/	
	if(chassis.position_ref >= 180) 
	{
		chassis.position_ref = chassis.position_ref  - 360;
	}
	else if(chassis.position_ref <= -180)
	{
		chassis.position_ref = 360 + chassis.position_ref;
	}	

}

void chassis_outter_loop(void)//������ģʽ�⻷
{
	//yaw����
  chassis.yaw_angle_ref = chassis.position_ref;                                      //�⻷(�ǶȻ�)Ŀ��ֵ
	chassis.yaw_angle_fdb = imu.yaw;                                                   //��ȡ�����ǽǶ���Ϊ��ǰ�Ƕ�
	critical_value_treatment(chassis.yaw_angle_fdb, &chassis.yaw_angle_ref,0,360);   //�ٽ�ֵ����
	pidckz_calc(&pid_chassis_angle, chassis.yaw_angle_fdb, chassis.yaw_angle_ref);      //PID����õ��⻷���(�Խ��ٶ���ʽ���)	
}

void chassis_inner_loop(void)//������ģʽ�ڻ�
{
	//yaw����
	chassis.yaw_speed_ref = pid_chassis_angle.out;                                //�⻷�����ΪĿ��ֵ
	chassis.yaw_speed_fdb = imu.gz;                                             //��ȡ�����ǵĽ��ٶ�ֵ
	pidckz_calc(&pid_chassis_speed, chassis.yaw_speed_ref, chassis.yaw_speed_fdb);    //PID����õ��������(ע��˭��˭)
	chassis.vw = pid_chassis_speed.out;
}

void chassis_yaw_handler(void)
{
	chassis_yaw_target();
	chassis_outter_loop();
	chassis_inner_loop();
}

