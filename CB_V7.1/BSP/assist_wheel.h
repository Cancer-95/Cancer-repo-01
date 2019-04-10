#ifndef __ASSIST_WHEL_H__
#define __ASSIST_WHEL_H__

#include "system.h"


typedef enum
{
	WHEEL_RELAX           = 0,//����к��	
	WHEEL_FORWARD 		    = 1,//������ת
	WHEEL_REVERSE        	= 2,//���ӷ�ת
	WHEEL_FORWARD_SLOW 		= 3,//������ת
	WHEEL_REVERSE_SLOW    = 4,//���ӷ�ת
	WHEEL_HOLD            = 5,//����ɲ��
	
} assist_wheel_mode_e;

typedef __packed struct
{
	assist_wheel_mode_e  ctrl_mode;
	
	uint16_t offset_ecd; //�����ʼ�Ƕ�ƫ��
	
	uint16_t cur_ecd;    //�����ǰʱ�̶���
  uint16_t las_ecd;    //�����һʱ�̶���
	
	int32_t  round_cnt;
	int32_t  total_ecd;   //�ܶ���
	int32_t  total_angle; //�ܽǶȻ���ֵ
	
  int32_t  target_angle;
	
	int16_t  wheel_speed_fdb;
  int16_t  wheel_speed_ref;
	int16_t  wheel_angle_fdb;
  int16_t  wheel_angle_ref;
  int16_t  current;
	
} assist_wheel_t;

void assist_wheel_param_init(void);
void assist_wheel_data_handler(void);
void assist_wheel_task(void);


extern assist_wheel_t assist_wheel;
#endif

