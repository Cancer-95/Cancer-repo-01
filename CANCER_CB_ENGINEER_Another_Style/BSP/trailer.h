#ifndef __TRAILER_H__
#define __TRAILER_H__

#include "system.h"

typedef enum
{	
	TRAILER_PULL    = 0,      //�ϳ��ջ�
	TRAILER_PUSH    = 1,      //�ϳ��Ƴ�
	TRAILER_RELAX   = 2,      //relax
	
} trailer_mode_e;

typedef __packed struct
{

	bool              reset;
	int16_t           reset_speed;       //��λ�ٶ�
	int16_t           target_speed;      //temp

	bool              across_stall_protect;
	bool              stall_protect_record; 
  uint32_t          stall_protect_time;   //
	
	bool              stall_time_record; //��תʱ����
	uint32_t          stall_time;        //��תʱ��
	uint16_t          stall_times;       //��ת����
//	bool              serious_stall;     //�ж��Ƿ����ض�ת
	
	trailer_mode_e  ctrl_mode;
	
	uint16_t offset_ecd;  //�����ʼ�Ƕ�ƫ��
	
	uint16_t cur_ecd;     //�����ǰʱ�̶���
  uint16_t las_ecd;     //�����һʱ�̶���
  bool     ecd_record;  //���μ�¼����־λ
	
	int32_t  round_cnt;
	int32_t  total_ecd;   //�ܶ���
	int32_t  total_angle; //�ܽǶȻ���ֵ
	
  int32_t  target_angle;
	int32_t  target_entire;
  int32_t  target_zero;
	
	int16_t  speed_fdb;
  int16_t  speed_ref;
	int16_t  angle_fdb;
  int16_t  angle_ref;
  int16_t  current;
	
} trailer_t;

void trailer_param_init(void);
void trailer_data_handler(void);
void trailer_task(void);


extern trailer_t trailer;
#endif

