#include "remote_ctrl.h"

rc_ctrl_t rc;

void remote_ctrl_chassis_handler(void)
{
	if(rc.rc_enable)
	{	
		rc.vx = -(RC_Ctl.rc.ch3 - RC_CH_VALUE_OFFSET) / RC_RESOLUTION * CHASSIS_RC_MAX_SPEED_X;				//ң����ֵ������x���ٶȵ�ת��
		rc.vy = -(RC_Ctl.rc.ch2 - RC_CH_VALUE_OFFSET) / RC_RESOLUTION * CHASSIS_RC_MAX_SPEED_Y;				//ң����ֵ������y���ٶȵ�ת��
		rc.vw =  (RC_Ctl.rc.ch0 - RC_CH_VALUE_OFFSET) / RC_RESOLUTION * CHASSIS_RC_MAX_SPEED_R;				//ң����ֵ������w���ٶȵ�ת��
		if(chassis.ass_mode_enable)
		{
			rc.vx = -rc.vx;
			rc.vy = -rc.vy;			
			rc.vw = -rc.vw;	
		}
	}
	else
	{
		rc.vx = 0;
		rc.vy = 0;
		rc.vw = 0;
	}

}

