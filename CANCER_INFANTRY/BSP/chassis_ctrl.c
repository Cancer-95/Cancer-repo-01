#include "chassis_ctrl.h"

/* chassis task global parameter */
chassis_t chassis;

void Chassis_Init()
{
	  PIDCKZ_param_set(&pid_chassis_angle, 300, 50, 8.0f, 0.0f, 10.0f);	//forŤ��
	
	  for(u8 k = 0; k < 4; k++)
  	PIDCKZ_param_set(&pid_chassis_speed[k], 10000, 1000, 8.0f, 0.2f, 20.0f);	//for���̳���
}

void Chassis_Control()
{
	switch (chassis_mode)
	{
		case CHASSIS_STOP://STOP(��ֵ�������治����PID����)
		{	
			chassis.vy = 0;chassis.vx = 0;chassis.vw = 0;
			chassis.current[M1] = chassis.current[M2] = chassis.current[M3] = chassis.current[M4] = 0;
		}break;
		
		case MANUAL_FOLLOW_CHASSIS://��̨������� FOR ��еģʽ
		{	
			if(system_mode == KEY_MACHINE_MODE)
			{
				chassis.vw =   RC_Ctl.mouse.x * 10;
				Chassis_WSAD_Keyboard_Ctrl(3, 3, 20, 3000);			
			}
			else if(system_mode == RC_MACHINE_MODE)
			{
				chassis.vx = -(RC_Ctl.rc.ch3 - RC_CH_VALUE_OFFSET) / RC_RESOLUTION * CHASSIS_RC_MAX_SPEED_X;				//ң����ֵ������x���ٶȵ�ת��
				chassis.vy = -(RC_Ctl.rc.ch2 - RC_CH_VALUE_OFFSET) / RC_RESOLUTION * CHASSIS_RC_MAX_SPEED_Y;				//ң����ֵ������y���ٶȵ�ת��
				chassis.vw =  (RC_Ctl.rc.ch0 - RC_CH_VALUE_OFFSET) / RC_RESOLUTION * CHASSIS_RC_MAX_SPEED_R;				//ң����ֵ������w���ٶȵ�ת��
			}
		}break;
		
		case MANUAL_FOLLOW_GIMBAL://���̸�����̨ FOR ������ģʽ
		{	
			if(system_mode == KEY_GYRO_MODE)
			{
				Chassis_WSAD_Keyboard_Ctrl(3, 3, 20, 3000);	
				chassis.vw =  (Yaw_M_MID - Motor_angle[YAW]) / RC_RESOLUTION * CHASSIS_RC_MAX_SPEED_R;			         //������ģʽ�µ�w���ٶȻ���
			}
			else if(system_mode == RC_GYRO_MODE)
			{
				chassis.vx = -(RC_Ctl.rc.ch3 - RC_CH_VALUE_OFFSET) / RC_RESOLUTION * CHASSIS_RC_MAX_SPEED_X;				//ң����ֵ������x���ٶȵ�ת��
				chassis.vy = -(RC_Ctl.rc.ch2 - RC_CH_VALUE_OFFSET) / RC_RESOLUTION * CHASSIS_RC_MAX_SPEED_Y;				//ң����ֵ������y���ٶȵ�ת��
				chassis.vw =  (Yaw_M_MID - Motor_angle[YAW]) / RC_RESOLUTION * CHASSIS_RC_MAX_SPEED_R;		        	//������ģʽ�µ�w���ٶȻ���
			}
		}break;
		
		case DODGE_MODE://ҡ��ģʽ UNDER ������ģʽ
		{	
      Chassis_twist_handle();
		}break;

		default://																																									//default�����
		{
			chassis.vx = 0;chassis.vy = 0;chassis.vw = 0;
			chassis.current[M1] = chassis.current[M2] = chassis.current[M3] = chassis.current[M4] = 0;
		}break;
	}
	
	if(chassis_mode == CHASSIS_STOP)
	{
	  CAN1_Send(0x200,chassis.current[M1],chassis.current[M2],chassis.current[M3],chassis.current[M4]);    //ֱ�����������
	}
	else
	{
	  mecanum_calc(chassis.vx, chassis.vy, chassis.vw, chassis.wheel_speed_ref);													//�����������ٶȵ�Ŀ��ֵ�ֽ�õ��ĸ����ӵ�Ŀ��ֵ
	
//		/*Roboremo���Σ�����ʱ��ע�͵�*/
//	  Roboremo_PID_tuning(&roboremo_pid.Inner_p, &roboremo_pid.Inner_i, &roboremo_pid.Inner_d, &roboremo_pid.Outter_p, &roboremo_pid.Outter_i, &roboremo_pid.Outter_d);//PID���κ���(�����������������)
//    for(u8 k = 0; k < 4; k++)
//	  PIDCKZ_param_roboremo(&pid_chassis_speed[k], roboremo_pid.Inner_p, roboremo_pid.Inner_i, roboremo_pid.Inner_d);
//    /*Roboremo���Σ�����ʱ��ע�͵�*/
	
	  for(u8 k = 0; k < 4; k++)
	  chassis.current[k]=PIDCKZ_Calc(&pid_chassis_speed[k],chassis.wheel_speed_fdb[k],chassis.wheel_speed_ref[k]);                            //PID����
	
//		/*Roboremo���Σ�����ʱ��ע�͵�*/
//	  Roboremo_Power_tuning(&roboremo_power.Total_Limit, &roboremo_power.Power_Threshold);//���ʵ��κ���
//		Chassis_power_handle(roboremo_power.Total_Limit,roboremo_power.Power_Threshold);
//    /*Roboremo���Σ�����ʱ��ע�͵�*/		
		
		Chassis_power_handle(36000,60);
		
	  CAN1_Send(0x200,chassis.current[M1],chassis.current[M2],chassis.current[M3],chassis.current[M4]);    //can1���͵���ֵ	
	}		

}

int rotation_center_gimbal = 0;//��̨�Ƿ��ڵ�������
void mecanum_calc(float vx, float vy, float vw, int16_t Wheel_speed[])
{
  static float rotate_ratio_fr;
  static float rotate_ratio_fl;
  static float rotate_ratio_bl;
  static float rotate_ratio_br;
  static float wheel_rpm_ratio;
	int16_t wheel_rpm[4];
  float   max = 0;
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
  wheel_rpm[3] = ( vx + vy + vw * rotate_ratio_bl) * wheel_rpm_ratio;
  wheel_rpm[2] = (-vx + vy + vw * rotate_ratio_br) * wheel_rpm_ratio;

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


/*����ģʽ�µ�ǰ������ƽ��(����Ť��ͬʱ�ƶ��Ĵ���)*/
void Chassis_WSAD_Keyboard_Ctrl(float Acc_For_AD, float Acc_For_WS, float Acc_Brake, u16 Chassis_Speed)
{
		if(KEY_W_PRESSED) 
		{
			chassis.dx -= Acc_For_AD;
			chassis.dx = constrain(chassis.dx, -Chassis_Speed, Chassis_Speed);
		}
		else if(KEY_S_PRESSED)
		{
			chassis.dx += Acc_For_AD;
			chassis.dx = constrain(chassis.dx, -Chassis_Speed, Chassis_Speed);
		}
		else
		{
			if(chassis.dx > 0)
			{
				chassis.dx -= Acc_Brake;
				if(chassis.dx < 0) chassis.dx = 0;
			}
			else if(chassis.dx < 0)
			{
				chassis.dx += Acc_Brake;
				if(chassis.dx > 0) chassis.dx = 0;
			}
		  else chassis.dx = 0;
		}
		
		if(KEY_D_PRESSED) 
		{
			chassis.dy -= Acc_For_WS;
			chassis.dy = constrain(chassis.dy, -Chassis_Speed, Chassis_Speed);
		}
		else if(KEY_A_PRESSED)
		{
			chassis.dy += Acc_For_WS;
			chassis.dy = constrain(chassis.dy, -Chassis_Speed, Chassis_Speed);
		}
		else
		{
			if(chassis.dy > 0)
			{
				chassis.dy -= Acc_Brake;
				if(chassis.dy < 0) chassis.dy = 0;
			}
			else if(chassis.dy < 0)
			{
				chassis.dy += Acc_Brake;
				if(chassis.dy > 0) chassis.dy = 0;
			}
		  else chassis.dy = 0;
		}
		
		if(system_mode == KEY_DODGE_MODE)//Ť��ģʽ��Ҫ����任
		{
			chassis.vx =  chassis.dy * sin( gim.yaw_relative_angle / RADIAN_COEF) 
	                 +chassis.dx * cos( gim.yaw_relative_angle / RADIAN_COEF);
	
			chassis.vy =  chassis.dy * cos( gim.yaw_relative_angle / RADIAN_COEF) 
	                 -chassis.dx * sin( gim.yaw_relative_angle / RADIAN_COEF);
		}
		else//����Ť��ģʽ ԭ�����
		{
      chassis.vx = chassis.dx;
			chassis.vy = chassis.dy;
		}
}


/*Ť������*/
uint32_t twist_count;
void Chassis_twist_handle()
{
  static int16_t twist_period = 1000;
  static int16_t twist_angle  = 40;

	twist_count++; 
	
  chassis.position_ref =  twist_angle * sin(2 * PI / twist_period * twist_count);

	chassis.vw = PIDCKZ_Calc(&pid_chassis_angle, gim.yaw_relative_angle, chassis.position_ref);	
	
	if(system_mode == KEY_DODGE_MODE)
	{
		Chassis_WSAD_Keyboard_Ctrl(3, 3, 20, 500);
	}
	else//���˼���ģʽʣ�µľ���ң������
	{
		chassis.vx = -((RC_Ctl.rc.ch2 - RC_CH_VALUE_OFFSET) / RC_RESOLUTION * CHASSIS_RC_MAX_SPEED_Y) * sin( gim.yaw_relative_angle / RADIAN_COEF) 
	               -((RC_Ctl.rc.ch3 - RC_CH_VALUE_OFFSET) / RC_RESOLUTION * CHASSIS_RC_MAX_SPEED_X) * cos( gim.yaw_relative_angle / RADIAN_COEF);
	
	  chassis.vy = -((RC_Ctl.rc.ch2 - RC_CH_VALUE_OFFSET) / RC_RESOLUTION * CHASSIS_RC_MAX_SPEED_Y) * cos( gim.yaw_relative_angle / RADIAN_COEF) 
	               +((RC_Ctl.rc.ch3 - RC_CH_VALUE_OFFSET) / RC_RESOLUTION * CHASSIS_RC_MAX_SPEED_X) * sin( gim.yaw_relative_angle / RADIAN_COEF);
	}
}


/*��������*/
void Chassis_power_handle(float Total_Limit, float Power_Threshold)
{
	chassis.total_current = abs(chassis.current[M1]) + abs(chassis.current[M2]) + abs(chassis.current[M3]) + abs(chassis.current[M4]);//�����ܵ���
	
	if(!PowerProtect.Judge_connect)//�Ӳ���������ѹ��Ϣ ǿ�����Ƶ���
	{
		chassis.total_current_limit = 10000;
	}
	else
	{
		if (PowerProtect.Judge_power_remain < Power_Threshold)
		{
			chassis.total_current_limit = (float)(PowerProtect.Judge_power_remain / 60.0f)*(float)(PowerProtect.Judge_power_remain / 60.0f) * Total_Limit;
		}
		else chassis.total_current_limit = Total_Limit;
	}
	
	if(chassis.total_current > chassis.total_current_limit)
	{
		chassis.current[M1] = chassis.current[M1] / chassis.total_current * chassis.total_current_limit;
		chassis.current[M2] = chassis.current[M2] / chassis.total_current * chassis.total_current_limit;
		chassis.current[M3] = chassis.current[M3] / chassis.total_current * chassis.total_current_limit;
		chassis.current[M4] = chassis.current[M4] / chassis.total_current * chassis.total_current_limit;
//		chassis.total_current = chassis.total_current_limit;
	}
}


