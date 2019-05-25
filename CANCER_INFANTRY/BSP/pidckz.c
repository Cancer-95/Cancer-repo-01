#include "pidckz.h"

/*����PID*/
PIDCKZ_t pid_chassis_speed[4]   = {0};
/*��̨PID*/
PIDCKZ_t pid_yaw_M_angle        = {0};
PIDCKZ_t pid_pit_M_angle        = {0};
PIDCKZ_t pid_yaw_M_speed        = {0};
PIDCKZ_t pid_pit_M_speed        = {0};
PIDCKZ_t pid_yaw_G_angle        = {0};
PIDCKZ_t pid_pit_G_angle        = {0};
PIDCKZ_t pid_yaw_G_speed        = {0};
PIDCKZ_t pid_pit_G_speed        = {0};
/*����PID*/
PIDCKZ_t pid_trigger_speed      = {0};
PIDCKZ_t pid_trigger_position   = {0};
/*���̽Ƕ�*/
PIDCKZ_t pid_chassis_angle      = {0};


void abs_limit(float *a, float ABS_MAX)
{
  if (*a > ABS_MAX)
      *a = ABS_MAX;
  if (*a < -ABS_MAX)
      *a = -ABS_MAX;
}

void PIDCKZ_param_clear(PIDCKZ_t *pid)
{
  memset(pid, 0, sizeof(PIDCKZ_t));
}

void PIDCKZ_param_set(PIDCKZ_t *pid, float maxout, float maxintergral, float kp, float ki, float kd)
{
  pid->p=kp;
	pid->i=ki;
	pid->d=kd;
	pid->MaxOut=maxout;
	pid->MaxIntegral=maxintergral;
}

void PIDCKZ_param_roboremo(PIDCKZ_t *pid, float kp, float ki, float kd)
{
  pid->p=kp;
	pid->i=ki;
	pid->d=kd;
}

float PIDCKZ_Calc(PIDCKZ_t *pid,float Get,float Set)
{
	pid->Set=Set;//�����趨ֵ	
	pid->Get=Get;//��ȡ

	pid->err=pid->Set-pid->Get;//����ƫ��
	 
	pid->Integral+=pid->i*pid->err;//�������ֵ	
	abs_limit(&pid->Integral,pid->MaxIntegral);	 //�����޷�
		
	pid->PTERM=pid->p*pid->err;
	pid->ITERM=pid->Integral;	
	pid->DTERM=pid->d*(pid->err-pid->err_last);
	
	pid->Out=pid->p*pid->err//����
								+pid->Integral//����
								+pid->d*(pid->err-pid->err_last);//΢��
	abs_limit(&pid->Out,pid->MaxOut);//PID����޷�  

	pid->err_last=pid->err;//����ǰƫ�������һ��ƫ��

	return pid->Out;
}
