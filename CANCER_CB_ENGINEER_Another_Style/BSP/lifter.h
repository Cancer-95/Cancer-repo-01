#ifndef __LIFTER_H__
#define __LIFTER_H__

#include "system.h"


typedef enum
{
	LIFTER_ZERO 	            = 0,//��������½�
	
	TUBE_LIFTER_ENTIRE        = 1,//���̧��
	TUBE_LIFTER_BIT           = 2,//���̧��һ���	
	
	BOMB_LIFTER_ENTIRE        = 3,//���̧��
	BOMB_LIFTER_BIT           = 4,//���̧��һ���
	
	FEED_LIFTER_ENTIRE        = 5,//���̧��	

} lifter_mode_e;


typedef struct
{
	
	bool              reset;
	int16_t           reset_speed[2];       //��λ�ٶ�
	int16_t           target_speed[2];      //temp

	bool              across_stall_protect[2];
	bool              stall_protect_record[2]; 
  uint32_t          stall_protect_time[2];   //
	
	bool              stall_time_record; //��תʱ����
	uint32_t          stall_time;        //��תʱ��
	uint16_t          stall_times[2];       //��ת����
	
	lifter_mode_e  ctrl_mode;
	
	uint16_t offset_ecd[2]; //�����ʼ�Ƕ�ƫ��
	
	uint16_t cur_ecd[2];    //�����ǰʱ�̶���
  uint16_t las_ecd[2];    //�����һʱ�̶���
	bool     ecd_Record[2]; //���μ�¼����־λ
	
	int32_t  round_cnt[2];
	int32_t  total_ecd[2];   //�ܶ���
	int32_t  total_angle[2]; //�ܽǶȻ���ֵ
	
  int32_t  target_angle[2];
	
	int32_t  target_tube_entire[2];
	int32_t  target_tube_bit[2];
	
	int32_t  target_bomb_entire[2];
	int32_t  target_bomb_bit_normal[2];
	int32_t  target_bomb_bit_scan[2];
	int32_t  target_bomb_bit_vision[2];
	int32_t  target_bomb_bit[2];
	
	int32_t  target_zero_climb_down[2];
	int32_t  target_zero_climb_up[2];
  int32_t  target_zero_normal[2];
	int32_t  target_zero[2];
	
  int32_t  target_feed_entire[2];
		
	int16_t  speed_fdb[2];
  int16_t  speed_ref[2];
	int16_t  angle_fdb[2];
  int16_t  angle_ref[2];
  int16_t  current[2];
	
	bool     flag_bomb_claw_reset;
  bool	   lift_for_bomb_claw_once;
	
} lifter_t;

void lifter_param_init(void);
void lifter_data_handler(u8 k);
void lifter_task(void);

extern lifter_t lifter;

extern bool from_lifter_zero;
extern bool from_lifter_bomb_entire;//�������ĳ�ȫ�ֱ��� ���Զ���λ��

#endif

