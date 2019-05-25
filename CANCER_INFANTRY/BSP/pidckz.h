#ifndef __PIDCKZ_H
#define __PIDCKZ_H	 
#include "system.h"

typedef struct {
	float Set;//�趨ֵ
	float Get;//ʵ��ֵ
	
	float err;//ƫ��
	float err_last;//�ϴ�ƫ��
	
	float p,i,d;//���������֡�΢��
	
	float Out;//PID���
	float Integral;//����ֵ
	float MaxOut;//PID������
	float MaxIntegral;//������ֵ�޷�
	
	float PTERM;
	float ITERM;
	float DTERM;

	}PIDCKZ_t;



void abs_limit(float *a, float ABS_MAX);
void PIDCKZ_param_clear(PIDCKZ_t *pid);
void PIDCKZ_param_set(PIDCKZ_t *pid, float maxout, float maxintergral, float kp, float ki, float kd);
void PIDCKZ_param_roboremo(PIDCKZ_t *pid, float kp, float ki, float kd);//����Roboremo��PID
float PIDCKZ_Calc(PIDCKZ_t *pid,float Get,float Set);

/*����PID*/
extern PIDCKZ_t pid_chassis_speed[4];
/*��̨PID*/
extern PIDCKZ_t pid_yaw_M_angle;
extern PIDCKZ_t pid_pit_M_angle;
extern PIDCKZ_t pid_yaw_M_speed;
extern PIDCKZ_t pid_pit_M_speed;
extern PIDCKZ_t pid_yaw_G_angle;
extern PIDCKZ_t pid_pit_G_angle;
extern PIDCKZ_t pid_yaw_G_speed;
extern PIDCKZ_t pid_pit_G_speed;
/*����PID*/
extern PIDCKZ_t pid_trigger_speed;
extern PIDCKZ_t pid_trigger_position;
/*���̽Ƕ�*/
extern PIDCKZ_t pid_chassis_angle;




#endif

