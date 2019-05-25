#include "ramp.h"

/*������WSAD��б��*/
ramp_t fb_ramp = RAMP_GEN_DAFAULT;
ramp_t lr_ramp = RAMP_GEN_DAFAULT; 

/*������������б��*/
ramp_t ld_run_ramp = RAMP_GEN_DAFAULT;
ramp_t ld_off_ramp = RAMP_GEN_DAFAULT;

ramp_t rd_run_ramp = RAMP_GEN_DAFAULT;
ramp_t rd_off_ramp = RAMP_GEN_DAFAULT;

/*����λ������б��*/
ramp_t reset_lifter_spd_ramp = RAMP_GEN_DAFAULT;
ramp_t reset_bomb_claw_spd_ramp = RAMP_GEN_DAFAULT;
ramp_t reset_trailer_spd_ramp = RAMP_GEN_DAFAULT;

/*����λ������б��*/
ramp_t landing_angle_ramp = RAMP_GEN_DAFAULT;

/*����ͷ��б��*/
ramp_t turn_around_ramp = RAMP_GEN_DAFAULT;

/*����ת��б��*/
ramp_t turn_left_ramp = RAMP_GEN_DAFAULT;

/*����ת��б��*/
ramp_t turn_right_ramp = RAMP_GEN_DAFAULT;

/*����תɲ����б��*/
ramp_t spin_brake_ramp = RAMP_GEN_DAFAULT;

void ramp_init(ramp_t *ramp, int32_t scale)//б�¼�������
{
  ramp->count = 0;
  ramp->scale = scale;
}

float ramp_calc(ramp_t *ramp)//ʵʱб�±�������ֵ
{
  if (ramp->scale <= 0)
    return 0;
  
  if (ramp->count++ >= ramp->scale)
    ramp->count = ramp->scale;
  
  ramp->out = ramp->count / ((float)ramp->scale);
  return ramp->out;
}

/*Cacner�Լ����ϵ����ڵ��̵�б�� �����˷���ɲ��*/
float ramp_kb_calc(ramp_t *ramp, uint8_t dir, int32_t scale)
{
	ramp->scale = scale;

	switch(dir)
	{
	  case CNT_UP://������
	  {
			if(ramp->count < 0) ramp->count += 3;
			else if(ramp->count++ >= ramp->scale) ramp->count = ramp->scale;	
	  }break;
		
	  case CNT_DOWN://������
	  {
			if(ramp->count > 0) ramp->count -= 3;
      else if(ramp->count-- <= -ramp->scale) ramp->count = -ramp->scale;
	  }break;
		
		case TO_ZERO://��������ɲ������
	  {		
			if(ramp->count < 0 && (ramp->count += 3) >= 0) ramp->count = 0;			
      else if(ramp->count > 0 && (ramp->count -= 3) <= 0) ramp->count = 0;
	  }break;

		default:break;
	}
	
  ramp->out = ramp->count / ((float)ramp->scale);
  return ramp->out;
}
/*Cacner�Լ����ϵ����ں�����б�� û��ʲô�ص�*/
float ramp_dutched_calc(ramp_t *ramp, uint8_t dir, int32_t scale)
{
	ramp->scale = scale;

	switch(dir)
	{
	  case CNT_UP:
	  {
			if(ramp->count++ >= ramp->scale) ramp->count = ramp->scale;	
	  }break;
	
		case TO_ZERO:
	  {		
      if((ramp->count -= 3) <= 0) ramp->count = 0;
	  }break;

		default:break;
	}
	
  ramp->out = ramp->count / ((float)ramp->scale);
  return ramp->out;
}

