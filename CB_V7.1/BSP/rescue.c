#include "rescue.h"

rescue_t rescue;

void rescue_param_init(void)
{
	TRAILER_HOOK_PULL;                      //�������׻���	
	
	trailer.ctrl_mode = TRAILER_PULL;       //�ϳ��ջ�			
	
	rescue.flag_hook_confirm = 0;           //����ȷ������
	rescue.hook_push_time_record = ENABLE;  //����ʱ���¼��־λ��һ
  rescue.hook_enable = DISABLE;           //���Ӳ��ɿ�
}

void rescue_ctrl(void)
{
	keyboard_rescue_handler();//keyboard	
		
	if(rescue.rescue_enable == ENABLE)	
	{
		switch(rescue.ctrl_mode)
		{	
			case RETRACT_HOOK_MODE://�ջع���
			{
				rescue.hook_enable = DISABLE;           //���Ӳ��ɿ�
				/*��¼���ӷ��µ�ʱ��*/
				if(rescue.hook_push_time_record)
				{
					rescue.hook_push_time = millis();
					rescue.hook_push_time_record = DISABLE;
				}
								
				if((millis() - rescue.hook_push_time < 1000) && (trailer.ctrl_mode == TRAILER_PUSH))  //2000ms����ֻ������������
				{
					TRAILER_HOOK_PUSH;                         //�ϳ��������׵��� �ͷ���Ա
					chassis.force_draft_out_enable = ENABLE;
				}
				else
				{
					trailer.ctrl_mode = TRAILER_PULL;                                //�ϳ��ջ�		
          TRAILER_HOOK_PULL;                                               //�������׻���					
					chassis.force_draft_out_enable = DISABLE;
					if(abs(trailer.total_angle - trailer.target_zero) < 100)         //�ϳ��յĲ����
					{
						flag_rescue_sight = 0;
						GIM_SERVO_ZERO;
						chassis.ass_mode_enable = DISABLE;                             //�رճ�βģʽ				
//						TRAILER_HOOK_PULL;                                            //�������׻���
						flag_trailer_hook = 0;                                         //����һ�ι������������������ȥ��
						rescue.flag_hook_confirm = 0;                                  //����ȷ������
						rescue.hook_push_time_record = 1;
						rescue.rescue_enable = DISABLE;                                //�����caseֻ��һ��			
					}							
				}
			}break;	
			
			case RELEASE_HOOK_MODE://�ͷŹ���
			{				
			  trailer.ctrl_mode = TRAILER_PUSH;  //�ϳ��Ƴ�
//				GIM_SERVO_BACK;
				chassis.ass_mode_enable = ENABLE;  //������βģʽ
				
				if(abs(trailer.total_angle - trailer.target_entire) < 100) rescue.hook_enable = ENABLE;//���ӿɿ� 	
			}break;	
			
			case HOOK_CONFIRM_MODE://ȷ�Ϲ�ס
			{
				rescue.hook_enable = DISABLE;      //���Ӳ��ɿ�
				TRAILER_HOOK_PULL;                 //�������׻���(��ס��Ա)
				trailer.ctrl_mode = TRAILER_RELAX; //�ϳ�к��
			  flag_rescue_sight = 0;
				GIM_SERVO_ZERO;
				chassis.ass_mode_enable = DISABLE; //�رճ�βģʽ
			}break;				
			
			default: break;
		}
	}
}

void rescue_task(void)
{
//	if(trailer.reset)
//	{
		rescue_ctrl();
//	}
}


