#ifndef __SHOOT_CTRL_H__
#define __SHOOT_CTRL_H__

#include "system.h"

#define Bomb_Angle 1620  //360*36/8 (360/�ٱ�/���̸���)




typedef struct
{
	uint16_t offset_ecd; //�����ʼ�Ƕ�ƫ��
	
	uint16_t cur_ecd;    //�����ǰʱ�̶���
  uint16_t las_ecd;    //�����һʱ�̶���
	
  int16_t  speed_rpm;  //
	
	int32_t  round_cnt;
	int32_t  total_ecd;   //�ܶ���
	int32_t  total_angle; //�ܽǶȻ���ֵ
	
} Tri_moto_t;





void lifter_init(void);


void lif_data_handler(void);
void Tri_Cas_Ctrl(u32 Target_Angle);
void Tri_Spe_Ctrl(int16_t speed);


void Fric_Ctrl(uint16_t pwm_L, uint16_t pwm_R);
void Servo_Ctrl( int16_t pwm );

void Shoot_Remote_Ctrl(void);


#endif
