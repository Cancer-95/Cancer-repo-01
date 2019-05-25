#include "relay.h"

bool flag_mirror_CCTV;
bool flag_bomb_claw_CCTV_switch;

void relay_init(void)
{	
  TUBE_CLAW_LOOSE;  //����צ���ɿ�
	BOMB_CLAW_CATCH;  //ȡ��צ�Ӻ���
	BOMB_CLAW_PULL;   //ȡ��צ���ջ�
	BOMB_EJECT_PULL;  //���������ջ�	
	TRAILER_HOOK_PULL;//�������׻���
  CCTV_CLIMB_TUBE;  //������ҰĬ������
}

void CCTV_switch_task(void)
{
	if(get_bomb.get_bomb_enable == ENABLE)
	{
		if(flag_bomb_claw_CCTV_switch)
		{
			CCTV_FEED_RESCUE;
		}
		else CCTV_GET_BOMB;
	}
	else if(feed_bomb.feed_bomb_enable == ENABLE || 
		      rescue.rescue_enable == ENABLE)
	{
		if(flag_magazine_exit)
		{
			CCTV_GET_BOMB;
		}
	  else CCTV_FEED_RESCUE;
	}
	else if(!check.check_CCTV_enable)//������ռ��
	{
		CCTV_CLIMB_TUBE;
	}
		
}
