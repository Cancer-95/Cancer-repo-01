#include "pidckz.h"

/*�������¶�*/
pidckz_t pid_imu_tmp            = {0};   //�������¶�
/*����PID*/
pidckz_t pid_wheel_speed[4]     = {0};   // �����ٶ�
/*���̽Ƕ�*/
pidckz_t pid_chassis_angle      = {0};   //yaw��pid�⻷
/*�����ٶ�*/
pidckz_t pid_chassis_speed      = {0};   //yaw��pid�ڻ�
/*�Ӿ���λ*/
pidckz_t pid_vision_chassis_vy  = {0};   //�Ӿ��µ���vy

///*��̨PID*/
//pidckz_t pid_yaw_G_angle         = {0};
//pidckz_t pid_yaw_G_speed         = {0};

/*����Ħ����PID*/
pidckz_t pid_fric_speed            = {0};
pidckz_t pid_fric_position         = {0};

/*����Ħ����PID*/
pidckz_t pid_trailer_speed         = {0};
pidckz_t pid_trailer_position      = {0};

/*̧��PID*/
pidckz_t pid_lifter_speed[2]       = {0};
pidckz_t pid_lifter_position[2]    = {0};

/*ȡ�������������PID*/
pidckz_t pid_bomb_claw_speed[4]    = {0};
pidckz_t pid_bomb_claw_position[4] = {0};

void abs_limit(float *a, float ABS_MAX)
{
  if (*a > ABS_MAX)
      *a = ABS_MAX;
  if (*a < -ABS_MAX)
      *a = -ABS_MAX;
}

void pidckz_reset(pidckz_t *pid)
{
  memset(pid, 0, sizeof(pidckz_t));
}

void pidckz_param_set(pidckz_t *pid, float maxout, float maxintergral, float kp, float ki, float kd)
{
  pid->p = kp;
	pid->i = ki;
	pid->d = kd;
	pid->maxout = maxout;
	pid->maxintegral = maxintergral;
}

void pidckz_param_roboremo(pidckz_t *pid, float kp, float ki, float kd)
{
  pid->p = kp;
	pid->i = ki;
	pid->d = kd;
}

float pidckz_calc(pidckz_t *pid, float get, float set)
{
	pid->set = set;//�����趨ֵ	
	pid->get = get;//��ȡ

	pid->err = pid->set-pid->get;//����ƫ��
	 
	pid->integral += pid->i * pid->err;//�������ֵ	
	abs_limit(&pid->integral, pid->maxintegral);	 //�����޷�
		
	pid->pout = pid->p * pid->err;
	pid->iout = pid->integral;	
	pid->dout = pid->d * (pid->err - pid->err_last);
	
	pid->out = pid->pout + pid->iout + pid->dout;
	abs_limit(&pid->out, pid->maxout);//PID����޷�  

	pid->err_last = pid->err;//����ǰƫ�������һ��ƫ��

	return pid->out;
}

void pidckz_integral_handle(pidckz_t *pid, float tolerance_err)
{
	/* ������һ����Χ��ʱ����������� */
	if(fabs(pid->err) >= tolerance_err)
	{
		pid->out = pid->out - pid->iout;
		pid->iout = 0;
	}
}
