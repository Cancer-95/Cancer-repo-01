#ifndef __CLIMB_TUBE_H
#define __CLIMB_TUBE_H

#include "system.h"

#define LAUNCH_TRIGGER    ((KEY_F_PRESSED ? 1 : 0) && KEY_CTRL_PRESSED) //CTRL+F �Զ����򴥷�

typedef enum
{	
	PLAIN_MODE             = 0,//ƽԭģʽ(�������ö�ٵ�һλ ʹ�ϵ�ʱ�ĳ�ʼ״̬��ƽԭģʽ)
	CLIMEB_DOWN_OPTIC_MODE = 1,//�¸�ģʽ (��緽��)
	CLIMEB_UP_OPTIC_MODE   = 2,//����ģʽ (��緽��)
	CLIMEB_DOWN_GYRO_MODE  = 3,//�¸�ģʽ (�����Ƿ���)
	CLIMEB_UP_GYRO_MODE    = 4,//����ģʽ (�����Ƿ���)

}climb_tube_mode_e;

typedef enum
{	
	ON_GROUND          = 0,//��ƽ��
	ON_ISLAND          = 1,//�ڵ���
	IN_DANGER          = 2,//Σ��

}climb_tube_location_e;

typedef struct
{
	climb_tube_mode_e  ctrl_mode;
	
	climb_tube_location_e location;              
	
	bool climb_enable;
	bool climb_up_optic_enable;
	bool climb_down_optic_enable;
	bool climb_up_gyro_enable;
	bool climb_down_gyro_enable;
	
  bool fiag_launch;             //�����־λ �������Ϊ��ʼת��
	
	bool yaw_record_enable;       //Ҫ��Ҫ�궨�Ƕ�
	bool twist_calc_enable;       //Ҫ��Ҫ����ֵ����
	
  float yaw_record;             //��¼�ı궨�Ƕ�
	float twisted_angle;          //ת���ĽǶ�
	
} climb_tube_t;

extern climb_tube_t climb_tube;

void climb_tube_param_init(void);
void climb_tube_task(void);

#endif

