#ifndef __pidckz_H
#define __pidckz_H	 
#include "system.h"

typedef struct {
	float set;//�趨ֵ
	float get;//ʵ��ֵ
	
	float err;//ƫ��
	float err_last;//�ϴ�ƫ��
	
	float p,i,d;//���������֡�΢��
	
	float out;//PID���
	float integral;//����ֵ
	float maxout;//PID������
	float maxintegral;//������ֵ�޷�
	
	float pout;
	float iout;
	float dout;

	}pidckz_t;

void abs_limit(float *a, float ABS_MAX);
	
void pidckz_reset(pidckz_t *pid);
	
void pidckz_param_set(pidckz_t *pid, float maxout, float maxintergral, float kp, float ki, float kd);
void pidckz_param_roboremo(pidckz_t *pid, float kp, float ki, float kd);//����Roboremo��PID

float pidckz_calc(pidckz_t *pid, float get, float set);

void pidckz_integral_handle(pidckz_t *pid, float tolerance_err);
	
/*�������¶�*/
extern pidckz_t pid_imu_tmp;	
/*����PID*/
extern pidckz_t pid_wheel_speed[4];
/*���̽Ƕ�*/
extern pidckz_t pid_chassis_angle;
/*�����ٶ�*/
extern pidckz_t pid_chassis_speed;   
/*�Ӿ��µ���vy*/
extern pidckz_t pid_vision_chassis_vy;   
	
///*��̨PID*/
//extern pidckz_t pid_yaw_G_angle;
//extern pidckz_t pid_yaw_G_speed;

/*����PID*/
extern pidckz_t pid_fric_speed;
extern pidckz_t pid_fric_position;

/*�ϳ�PID*/
extern pidckz_t pid_trailer_speed;
extern pidckz_t pid_trailer_position;

/*̧��PID*/
extern pidckz_t pid_lifter_speed[2];
extern pidckz_t pid_lifter_position[2];
 
/*ȡ�������������PID*/
extern pidckz_t pid_bomb_claw_speed[4];
extern pidckz_t pid_bomb_claw_position[4];


#endif

