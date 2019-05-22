#include "get_bomb.h"

get_bomb_t get_bomb;

void get_bomb_param_init(void)
{
	lifter.ctrl_mode = LIFTER_ZERO;     //�������������	

	kb.bomb_claw_enable = DISABLE;           //ȡ��צ��
//	SUPPORTING_WHEEL_PULL;                   //����֧�����ջ�
	BOMB_CLAW_CATCH;                         //ȡ��צ�Ӻ���
	BOMB_EJECT_PULL;                         //�����ջ�
	
	BOMB_CLAW_MOVE_LEFT;                     //
//	BOMB_CLAW_MOVE_MID;                      //ȡ��צ��ƽ������
	BOMB_CLAW_ROLL_ZERO;                     //ȡ��צ�ӷ�ת����	
//	BOMB_CLAW_ROLL_ENTIRE; 
	BOMB_CLAW_PULL;                          //ȡ��צ���ջ�

	get_bomb.once_triple = 0;							
	flag_bomb_claw_triple = 0;               //�ص�����ģʽ ���´ΰ�ctrl+R Ĭ���ǵ���ģʽ
	
	get_bomb.once_penta = 0;							
	flag_bomb_claw_penta = 0;                //�ص�����ģʽ ���´ΰ�ctrl+R Ĭ���ǵ���ģʽ
	
	vision.enable = DISABLE;
//	vision.aim = HAVENT;
//	get_bomb.vision_assist = 0;	
	flag_bomb_claw_vision = 0;               //�ص��Ӿ�ģʽ ���´ΰ�ctrl+R Ĭ���ǵ���ģʽ
	
	get_bomb.scan = 0;	
  get_bomb.flag_scan_failed = NO;	
	get_bomb.scan_finished = NO;	
	optic_switch.value_record	= ENABLE;	
  get_bomb.scan_delay_record = ENABLE;	
	flag_bomb_claw_scan = 0;                 //�ص�ɨ��ģʽ ���´ΰ�ctrl+R Ĭ���ǵ���ģʽ
	
	get_bomb.turn_around_record = ENABLE;    //����ȡ����ͷʹ��
	get_bomb.turn_around_enable = ENABLE;
	
	get_bomb.flag_push_aim_single = HAVENT;    //��û����Ƴ���׼ģʽ	
	get_bomb.flag_push_aim_multiple = HAVENT;  //��û����Ƴ���׼ģʽ
	get_bomb.flag_push_aim_scan = HAVENT;      //��û����Ƴ���׼ģʽ
	
	get_bomb.get_pour_step = 0;              //ȡ������ģʽstep����	
}

bool flag_get_bomb_init = 1;

bool flag_push_aim_single_init = 1;
bool flag_push_aim_multiple_init = 1;
bool flag_push_aim_scan_init = 1;
bool flag_scan_safe_delay = 0;//��ʱһС��ʱ��

bool flag_eject_push_Time_record = 1;
bool flag_bomb_claw_record = 0;  

void get_bomb_ctrl(void)
{
	/*����ʼ����Y_CLAW_HALF������� �ʷ��������ʼ��*/	
	if(flag_get_bomb_init)
	{
		get_bomb_param_init();
		BOMB_CLAW_MOVE_MID;                      //ȡ��צ��ƽ������
		BOMB_CLAW_ROLL_ENTIRE; 
		
	  flag_push_aim_single_init = 1;
	  flag_push_aim_multiple_init = 1;
		flag_push_aim_scan_init = 1;
		flag_scan_safe_delay = 0;
		
		flag_get_bomb_init = 0;
	}

	keyboard_get_bomb_handler();//keyboard	
	
  if(get_bomb.get_bomb_enable == ENABLE)	
	{
//		get_bomb.vision_assist = ENABLE;//ȫ�������Ӿ���λ
		
		switch(get_bomb.ctrl_mode)
		{	
			case STAND_BY_MODE://���д���ģʽ
			{		
				get_bomb.turn_around_record = ENABLE;   //����ȡ����ͷʹ��
				get_bomb.turn_around_enable = ENABLE;
				
		    chassis.ass_mode_enable = DISABLE; 	    //�ص���βģʽ  
				chassis.force_push_in_enable = DISABLE; //�ص��ƽ�ģʽ
				chassis.ctrl_mode = CHASSIS_FOLLOW;	    //����������ģʽ
				
				flag_bomb_claw_CCTV = 0;
				flag_bomb_claw_CCTV_switch = 0;
				
				flag_eject_push_Time_record = 1;
				get_bomb.have_finished_once_get_bomb = NO;
				
				flag_push_aim_single_init = 1;
				flag_push_aim_multiple_init = 1;
				flag_push_aim_scan_init = 1;
				flag_scan_safe_delay = 0;
				
				get_bomb.once_triple = 0;							
	      flag_bomb_claw_triple = 0;               //�ص�����ģʽ ���´ΰ�ctrl+R Ĭ���ǵ���ģʽ
				
				get_bomb.once_penta = 0;							
	      flag_bomb_claw_penta = 0;                //�ص�����ģʽ ���´ΰ�ctrl+R Ĭ���ǵ���ģʽ
				
//				get_bomb.vision_assist = DISABLE;
				vision.enable = DISABLE;
//				vision.aim = HAVENT;
//				get_bomb.vision_assist = 0;	
				flag_bomb_claw_vision = 0;               //�ص��Ӿ�ģʽ ���´ΰ�ctrl+R Ĭ���ǵ���ģʽ
				
			  get_bomb.scan = 0;	
				get_bomb.flag_scan_failed = NO;
        get_bomb.scan_finished = NO;	
        optic_switch.value_record	= ENABLE;	
				get_bomb.scan_delay_record = ENABLE;			
		    flag_bomb_claw_scan = 0;             //�ص�ɨ��ģʽ ���´ΰ�ctrl+R Ĭ���ǵ���ģʽ
				
				/*��������ֵY��*/	
				if(bomb_claw.target_half[Y] > bomb_claw.Y_target[M])
				{
					bomb_claw.from_claw_entire[Y_axis] = YES;
					bomb_claw.from_claw_zero[Y_axis] = NO;
				}	
				else if(bomb_claw.target_half[Y] < bomb_claw.Y_target[M])
				{
					bomb_claw.from_claw_entire[Y_axis] = NO;
					bomb_claw.from_claw_zero[Y_axis] = YES;
				}		
				bomb_claw.target_half[Y] = bomb_claw.Y_target[M];
				/*�������Ŀ��ֵ*/ 
				if(lifter.target_bomb_bit[L] != lifter.target_bomb_bit_normal[L] || 
					 lifter.target_bomb_bit[R] != lifter.target_bomb_bit_normal[R])		
				{
					lifter.target_bomb_bit[L] = lifter.target_bomb_bit_normal[L];
					lifter.target_bomb_bit[R] = lifter.target_bomb_bit_normal[R];
					if((lifter.total_angle[L] > lifter.target_bomb_bit[L]) && 
						 (lifter.total_angle[R] < lifter.target_bomb_bit[R]))
					{
						from_lifter_zero = YES;
						from_lifter_bomb_entire = NO; 			
					}
					else if((lifter.total_angle[L] < lifter.target_bomb_bit[L]) && 
									(lifter.total_angle[R] > lifter.target_bomb_bit[R]))
					{
						from_lifter_zero = NO;
						from_lifter_bomb_entire = YES; 			
					}
				}		
				/*����ƽ̨����Ŀ��ֵ*/
				if(bomb_claw.target_entire[Y] !=  bomb_claw.Y_target[R] ||
 				   bomb_claw.target_zero[Y] != bomb_claw.Y_target[L])
				{
					bomb_claw.target_entire[Y] =  bomb_claw.Y_target[R];
					bomb_claw.target_zero[Y] = bomb_claw.Y_target[L];
				}
				
				
				kb.bomb_claw_enable = DISABLE;    	 //ȡ��צײ���ɿ�	 �ص��˰�ctrl+E��û��ʲô�õ�
				
//	      SUPPORTING_WHEEL_PULL;               //����֧�����ջ�				
				BOMB_CLAW_CATCH;                   	 //ȡ��צ�Ӻ���
				BOMB_EJECT_PULL;                     //�����ջ�

        BOMB_CLAW_MOVE_MID;               	 //ȡ��צ��ƽ������
        BOMB_CLAW_ROLL_ENTIRE;               //ȡ��צ�ӷ�ת����			
				BOMB_CLAW_PULL;                   	 //ȡ��צ���ջ�		
				
				GIM_SERVO_ZERO;                      //��̨ǰ��ת����ǰ��

				get_bomb.flag_push_aim_single   = HAVENT;     //��û����Ƴ���׼ģʽ	
				get_bomb.flag_push_aim_multiple = HAVENT;   //��û����Ƴ���׼ģʽ	
				get_bomb.flag_push_aim_scan     = HAVENT;       //��û����Ƴ���׼ģʽ
				
				get_bomb.box_num = 0;	
				
				get_bomb.get_pour_step = 0;          //ȡ������ģʽstep����
					
//        if(servo_get_bomb_delay_record)//��ʱһС��ʱ���ö��ת������
//				{					
//					servo_get_bomb_delay_time = millis();
//					servo_get_bomb_delay_record = 0;
//				}
//				if(millis() - servo_get_bomb_delay_time > 800)
//				{
				if((abs(bomb_claw.total_angle[L] - bomb_claw.target_entire[L]) < 100) || 
					 (abs(bomb_claw.total_angle[R] - bomb_claw.target_entire[R]) < 100))	//צ�ӷ������� ������������Ҫ��צ�ӷ��ÿ��				
				{
					get_bomb.get_bomb_enable = DISABLE; //�����case��һ�ξ��˳���
					lifter.ctrl_mode = LIFTER_ZERO;     //�������������
				}										
//				}		
			}break;
					
			case PUSH_AIM_SINGLE_MODE://����ģʽ�µ��Ƴ���׼ģʽ
			{	
				if((climb_tube.location == ON_GROUND) && 
					 (climb_tube.ctrl_mode == PLAIN_MODE) &&
				    chassis.ctrl_mode == CHASSIS_FOLLOW &&
				    get_bomb.turn_around_enable == ENABLE)//�����ȵ���ͷ
				{
					if(get_bomb.turn_around_record)
					{
						get_bomb.turn_around_time = millis();
						flag_turn_around = 1;
						chassis.position_temp = imu.yaw;
						ramp_init(&turn_around_ramp, TUR_ARO_TIME/INFO_GET_PERIOD);		
						get_bomb.turn_around_record = DISABLE;
					}
					else if((!flag_turn_around) && ((millis() - get_bomb.turn_around_time) > 1200))//��ͷ���ǰ����������ģʽ ��ʱһС��ʱ�価������ת��λ
					{
						chassis.ass_mode_enable = ENABLE;               //������βģʽ
						chassis.force_push_in_enable = ENABLE;          //�����ƽ�ģʽ
						if(vision.enable == DISABLE)
						{
							chassis.ctrl_mode = CHASSIS_SEPARATE;         //���̻�еģʽ
						}							
					}
				}
	      else //���ϸø������
				{
					chassis.ass_mode_enable = ENABLE;               //������βģʽ
					chassis.force_push_in_enable = ENABLE;          //�����ƽ�ģʽ
					if(vision.enable == DISABLE)
					{
						chassis.ctrl_mode = CHASSIS_SEPARATE;         //���̻�еģʽ
					}							
				}					
	
		  	{
				  /*��������ֵY��*/	
					if(bomb_claw.target_half[Y] > bomb_claw.Y_target[M])
					{
						bomb_claw.from_claw_entire[Y_axis] = YES;
						bomb_claw.from_claw_zero[Y_axis] = NO;
					}	
					else if(bomb_claw.target_half[Y] < bomb_claw.Y_target[M])
					{
						bomb_claw.from_claw_entire[Y_axis] = NO;
						bomb_claw.from_claw_zero[Y_axis] = YES;
					}		
					bomb_claw.target_half[Y] = bomb_claw.Y_target[M];
					if(!vision.enable)
					{
						/*�������Ŀ��ֵ*/ 
						if(lifter.target_bomb_bit[L] != lifter.target_bomb_bit_normal[L] || 
							 lifter.target_bomb_bit[R] != lifter.target_bomb_bit_normal[R])		
						{
							lifter.target_bomb_bit[L] = lifter.target_bomb_bit_normal[L];
							lifter.target_bomb_bit[R] = lifter.target_bomb_bit_normal[R];
							if((lifter.total_angle[L] > lifter.target_bomb_bit[L]) && 
								 (lifter.total_angle[R] < lifter.target_bomb_bit[R]))
							{
								from_lifter_zero = YES;
								from_lifter_bomb_entire = NO; 			
							}
							else if((lifter.total_angle[L] < lifter.target_bomb_bit[L]) && 
											(lifter.total_angle[R] > lifter.target_bomb_bit[R]))
							{
								from_lifter_zero = NO;
								from_lifter_bomb_entire = YES; 			
							}	
						}					
					}
					/*����ƽ̨����Ŀ��ֵ*/
					if(bomb_claw.target_entire[Y] !=  bomb_claw.Y_target[R] || 
						 bomb_claw.target_zero[Y] != bomb_claw.Y_target[L])
					{
						bomb_claw.target_entire[Y] =  bomb_claw.Y_target[R];
						bomb_claw.target_zero[Y] = bomb_claw.Y_target[L];
					}			
					
	//				servo_get_bomb_delay_record = 1;
	//				servo_get_bomb_scan_delay_record = 1;
					
					get_bomb.once_triple = 0;							
					flag_bomb_claw_triple = 0;               //�ص�����ģʽ ���´ΰ�ctrl+R Ĭ���ǵ���ģʽ
					
					get_bomb.once_penta = 0;							
					flag_bomb_claw_penta = 0;                //�ص�����ģʽ ���´ΰ�ctrl+R Ĭ���ǵ���ģʽ
					
	//				vision.enable = DISABLE;
	//				vision.aim = HAVENT;
	//				get_bomb.vision_assist = 0;	
	//				flag_bomb_claw_vision = 0;               //�ص��Ӿ�ģʽ ���´ΰ�ctrl+R Ĭ���ǵ���ģʽ
					
					get_bomb.scan = 0;	
					get_bomb.flag_scan_failed = NO;
					optic_switch.value_record	= ENABLE;		
					get_bomb.scan_delay_record = ENABLE;						
					get_bomb.scan_finished = NO;				
					flag_bomb_claw_scan = 0;             //�ص�ɨ��ģʽ ���´ΰ�ctrl+R Ĭ���ǵ���ģʽ
					
					get_bomb.flag_push_aim_single = HAVENT;     //��û��ɵ���ģʽ���Ƴ���׼ģʽ
					get_bomb.flag_push_aim_multiple = HAVENT;   //��û�������ģʽ���Ƴ���׼ģʽ
					get_bomb.flag_push_aim_scan = HAVENT;       //��û����Ƴ���׼ģʽ
					
					BOMB_CLAW_MOVE_MID;                 //ǿ���Ƶ��м�
					get_bomb.get_pour_step = 0;         //�����ȡ����������		
					
					if(flag_push_aim_single_init)
					{
						BOMB_CLAW_ROLL_HALF;//ǿ�Ʒ�תצ����HALF		

//						if((abs(lifter.target_bomb_bit[L] - lifter.total_angle[L]) < 100) || 
//							 (abs(lifter.target_bomb_bit[R] - lifter.total_angle[R]) < 100))
//						{
							GIM_SERVO_SIDE;
//						}						
						
						if((abs(bomb_claw.total_angle[L] - bomb_claw.target_half[L]) < 100) || 
							 (abs(bomb_claw.total_angle[R] - bomb_claw.target_half[R]) < 100))			
						{
	//						if(climb_tube.location == ON_ISLAND)
	//						{
	//					     BOMB_CLAW_PUSH;
	//						}
	//						else BOMB_CLAW_PULL;					
							
							BOMB_CLAW_PULL;

							BOMB_EJECT_PULL;//�����ջ�
							BOMB_CLAW_LOOSE;//ǿ���ſ�צ�� 
							
							if(flag_eject_push_Time_record && get_bomb.have_finished_once_get_bomb == NO)
							{
								get_bomb.eject_push_Time = millis();//��¼�����ʱ��ms	
								flag_eject_push_Time_record = 0;
							}
							else if((millis() - get_bomb.eject_push_Time > 300) && 
								      (millis() - get_bomb.eject_push_Time < 600)&&
						          	get_bomb.have_finished_once_get_bomb == NO)//ms
							{
								BOMB_EJECT_PUSH;
							}
							else if(millis() - get_bomb.eject_push_Time > 900)//ms
							{
								BOMB_EJECT_PULL;
								flag_push_aim_single_init = 0;
								flag_push_aim_multiple_init = 1;
								flag_push_aim_scan_init = 1;
								flag_scan_safe_delay = 0;
								get_bomb_clear_record = 0;//�˳����������Ŀ���							
							}					
						}
					}
					
					if((abs(lifter.target_bomb_bit[L] - lifter.total_angle[L]) < 100) ||
						 (abs(lifter.target_bomb_bit[R] - lifter.total_angle[R]) < 100))//̧����Ŀ��Ƕ�
					{ /*���̧����λ�ò�ִ�������׼*/	
							kb.bomb_claw_enable = ENABLE;//ȡ��צ�ӿɿ�								
					
//						if((flag_bomb_claw_catch) && ((abs(bomb_claw.target_zero[L] - bomb_claw.total_angle[L]) < 100) || (abs(bomb_claw.target_zero[R] - bomb_claw.total_angle[R]) < 100)))							
						if((flag_bomb_claw_catch) && (bomb_claw.zero_correct == FINISHED))
						{				
							if(get_bomb.catch_confirm_record)//��ʱһС��ʱ����צ�Ӽ�ס
							{					
								get_bomb.catch_confirm_delay_time = millis();
								get_bomb.catch_confirm_record = DISABLE;
							}
							else if(millis() - get_bomb.catch_confirm_delay_time > 125)
							{
								get_bomb.flag_push_aim_single = FINISHED;//���������Ƴ���׼ģʽ
								get_bomb.catch_confirm_record = ENABLE;	
							}
						}	
						else get_bomb.catch_confirm_record = ENABLE;			
					}
					else lifter.ctrl_mode = BOMB_LIFTER_BIT;//���̧��һ��
				}					
																		
			}break;
						
			case PUSH_AIM_MULTIPLE_MODE://����ģʽ�µ��Ƴ���׼ģʽ
			{					
				
				chassis.ass_mode_enable = ENABLE;                //������βģʽ
				chassis.force_push_in_enable = ENABLE;           //�����ƽ�ģʽ
				if(vision.enable == DISABLE)
				{
					chassis.ctrl_mode = CHASSIS_SEPARATE;          //���̻�еģʽ
				}
				
			
//				vision.enable = DISABLE;
//				vision.aim = HAVENT;				
//				get_bomb.vision_assist = 0;	
//	      flag_bomb_claw_vision = 0;                  //�ص��Ӿ�ģʽ ���´ΰ�ctrl+R Ĭ���ǵ���ģʽ
				
				get_bomb.scan = 0;	
				get_bomb.flag_scan_failed = NO;
				optic_switch.value_record	= ENABLE;			
				get_bomb.scan_delay_record = ENABLE;	
        get_bomb.scan_finished = NO;								
		    flag_bomb_claw_scan = 0;                    //�ص�ɨ��ģʽ ���´ΰ�ctrl+R Ĭ���ǵ���ģʽ
					
//				if(get_bomb.once_triple)
//				{
//					get_bomb.once_penta = 0;							
//					flag_bomb_claw_penta = 0;
//				}
//				else if(get_bomb.once_penta)
//				{
//					get_bomb.once_triple = 0;	
//					flag_bomb_claw_triple = 0;
//				}
				
				get_bomb.flag_push_aim_single = HAVENT;     //��û��ɵ���ģʽ���Ƴ���׼ģʽ	
				get_bomb.flag_push_aim_multiple = HAVENT;   //��û�������ģʽ���Ƴ���׼ģʽ
				get_bomb.flag_push_aim_scan = HAVENT;       //��û�������ģʽ���Ƴ���׼ģʽ

				if(get_bomb.box_num == 3 || get_bomb.box_num == 4)//����֮�е����ڶ��俪ʼ
				{ /*ǿ�б��Ϊ�Ƴ�*/
					BOMB_CLAW_PUSH;
					flag_bomb_claw_record = flag_bomb_claw_push;
					/*�޸�ƽ̨����Ŀ��ֵ*/
					bomb_claw.target_entire[Y] = (bomb_claw.Y_target[R] + bomb_claw.Y_target[M])/2;
	        bomb_claw.target_zero[Y] = (bomb_claw.Y_target[L] + bomb_claw.Y_target[M])/2;				
				}
				
				
				switch(get_bomb.box_num)
				{
					case 0:
					{					
						BOMB_CLAW_MOVE_MID;
						get_bomb.get_pour_step = 0;//�����ȡ����������	
						
						if(flag_push_aim_multiple_init)
						{
							flag_push_aim_multiple_init = 0;
							flag_push_aim_single_init = 1;
							flag_push_aim_scan_init = 1;
							flag_scan_safe_delay = 0;
							
							BOMB_CLAW_LOOSE;//ǿ���ſ�צ��
							BOMB_CLAW_ROLL_HALF;//ǿ�Ʒ�תצ����HALF	
							
//							/*�ڵ��ϵ�ʱ��Ĭ��ȡ��צ���Ƴ� ���µ�ʱ��Ĭ�ϲ��Ƴ�*/					
//							if(climb_tube.location == ON_ISLAND) 
//							{
//								BOMB_CLAW_PUSH;
//							}							
//							else BOMB_CLAW_PULL;
							BOMB_CLAW_PULL;
						}
									
						if((abs(lifter.target_bomb_bit[L] - lifter.total_angle[L]) < 100) || 
							 (abs(lifter.target_bomb_bit[R] - lifter.total_angle[R]) < 100))//̧����Ŀ��Ƕ�
						{ /*���̧����λ�ò�ִ�������׼*/							  
							kb.bomb_claw_enable = ENABLE;//ȡ��צ�ӿɿ�								
						  
							if(get_bomb.once_penta) flag_bomb_claw_push = 0;//ȡ�����ʱ���һ���ǲ��Ƴ�ȥ��
							
//							if((flag_bomb_claw_catch) && 
//								((abs(bomb_claw.target_zero[L] - bomb_claw.total_angle[L]) < 100) || 
//							   (abs(bomb_claw.target_zero[R] - bomb_claw.total_angle[R]) < 100)))	
              if((flag_bomb_claw_catch) && (bomb_claw.zero_correct == FINISHED))					
							{
							  flag_bomb_claw_record = flag_bomb_claw_push;//��¼�ǰ������Ƴ�����
								
								if(get_bomb.catch_confirm_record)//��ʱһС��ʱ����צ�Ӽ�ס
								{					
									get_bomb.catch_confirm_delay_time = millis();
									get_bomb.catch_confirm_record = DISABLE;
								}
								else if(millis() - get_bomb.catch_confirm_delay_time > 125)
								{
									get_bomb.flag_push_aim_multiple = FINISHED; //���������Ƴ���׼ģʽ
								  get_bomb.catch_confirm_record = ENABLE;	
								}
						  }							
              else get_bomb.catch_confirm_record = ENABLE;							
						}
						else lifter.ctrl_mode = BOMB_LIFTER_BIT;//���̧��һ��										
					}break;
					
					case 1:
					case 3:
					{
						chassis.ctrl_mode = CHASSIS_STOP;      //�����ٶȻ���ס
						
						kb.bomb_claw_enable = ENABLE;//ȡ��צ�Ӳ��ɿ�

//						BOMB_CLAW_MOVE_LEFT;							
//						if(abs(bomb_claw.target_zero[Y] - bomb_claw.total_angle[Y]) < 100)//ȡ��צ��Ų����λ��
					  BOMB_CLAW_MOVE_RIGHT;							
						if(abs(bomb_claw.target_entire[Y] - bomb_claw.total_angle[Y]) < 100)//ȡ��צ��Ų����λ��
						{
							BOMB_CLAW_ROLL_ZERO;
//							if((abs(bomb_claw.target_zero[L] - bomb_claw.total_angle[L]) < 100) || 
//								 (abs(bomb_claw.target_zero[R] - bomb_claw.total_angle[R]) < 100))//ȡ��צ�ӷ�ת����λ��
							if(bomb_claw.zero_correct == FINISHED)
							{
								BOMB_CLAW_CATCH;                             //ȡ��צ��ץȡ
								if(get_bomb.catch_confirm_record)//��ʱһС��ʱ����צ�Ӽ�ס
								{					
									get_bomb.catch_confirm_delay_time = millis();
									get_bomb.catch_confirm_record = DISABLE;
								}
								else if(millis() - get_bomb.catch_confirm_delay_time > 125)
								{
									get_bomb.flag_push_aim_multiple = FINISHED;  //���������Ƴ���׼ģʽ
								  get_bomb.catch_confirm_record = ENABLE;
								}
							}
						}
					}break;
						
					case 2:
					case 4:
					{
						chassis.ctrl_mode = CHASSIS_STOP;      //�����ٶȻ���ס
						
						kb.bomb_claw_enable = ENABLE;//ȡ��צ�Ӳ��ɿ�										

						BOMB_CLAW_MOVE_LEFT;							
						if(abs(bomb_claw.target_zero[Y] - bomb_claw.total_angle[Y]) < 100)//ȡ��צ��Ų����λ��
//						BOMB_CLAW_MOVE_RIGHT;							
//						if(abs(bomb_claw.target_entire[Y] - bomb_claw.total_angle[Y]) < 100)//ȡ��צ��Ų����λ��
						{
							BOMB_CLAW_ROLL_ZERO;
//							if((abs(bomb_claw.target_zero[L] - bomb_claw.total_angle[L]) < 100) || 
//								 (abs(bomb_claw.target_zero[R] - bomb_claw.total_angle[R]) < 100))//ȡ��צ�ӷ�ת����λ��
							if(bomb_claw.zero_correct == FINISHED)
							{
								BOMB_CLAW_CATCH;                             //ȡ��צ��ץ
								if(get_bomb.catch_confirm_record)//��ʱһС��ʱ����צ�Ӽ�ס
								{					
									get_bomb.catch_confirm_delay_time = millis();
									get_bomb.catch_confirm_record = DISABLE;
								}
								else if(millis() - get_bomb.catch_confirm_delay_time > 125)
								{
									get_bomb.flag_push_aim_multiple = FINISHED;  //���������Ƴ���׼ģʽ
							  	get_bomb.catch_confirm_record = ENABLE;
								}
							}
						}
					}break;
					
					
					default: break;					
				}											
																		
			}break;
							
			case PUSH_AIM_SCAN_MODE://ɨ��ģʽ�µ��Ƴ���׼ģʽ
			{	
				
			  chassis.ass_mode_enable = ENABLE;               //������βģʽ
				chassis.force_push_in_enable = ENABLE;          //�����ƽ�ģʽ
				if(vision.enable == DISABLE)
				{
					chassis.ctrl_mode = CHASSIS_STOP;             //�����ٶȻ���ס
				}
				
				
				get_bomb.once_triple = 0;							
	      flag_bomb_claw_triple = 0;           //�ص�����ģʽ ���´ΰ�ctrl+R Ĭ���ǵ���ģʽ
					
				get_bomb.once_penta = 0;							
				flag_bomb_claw_penta = 0;            //�ص�����ģʽ ���´ΰ�ctrl+R Ĭ���ǵ���ģʽ
				
//				vision.enable = DISABLE;
//				vision.aim = HAVENT;
//				get_bomb.vision_assist = 0;	
//	      flag_bomb_claw_vision = 0;               //�ص��Ӿ�ģʽ ���´ΰ�ctrl+R Ĭ���ǵ���ģʽ
				
				get_bomb.flag_push_aim_single = HAVENT;     //��û��ɵ���ģʽ���Ƴ���׼ģʽ	
				get_bomb.flag_push_aim_multiple = HAVENT;   //��û�������ģʽ���Ƴ���׼ģʽ
				get_bomb.flag_push_aim_scan = HAVENT;       //��û�������ģʽ���Ƴ���׼ģʽ
				
				get_bomb.get_pour_step = 0;         //�����ȡ����������	
				
				if(flag_bomb_claw_push)//�����shift+R֮ǰצ�������ȥ�� �������һ����ʱһ��
				{
					BOMB_CLAW_PULL;//צ���ջ���
					get_bomb.scan_safe_Time = millis();//��¼�����ʱ��ms
					flag_scan_safe_delay = 1;//��ʱʹ�ܿ����� ����ʱ����צ�������
				}
				if(flag_scan_safe_delay)
				{
					if(millis() - get_bomb.scan_safe_Time > 1000)//ms
					{
					  flag_scan_safe_delay = 0;
					}
				}
				else
				{
					if(flag_push_aim_scan_init)
					{
//					  if(servo_get_bomb_scan_delay_record)
//						{
//							servo_get_bomb_scan_delay_time = millis();
//							servo_get_bomb_scan_delay_record = 0;
//							GIM_SERVO_BACK;
//						}	
//						else if(millis() - servo_get_bomb_scan_delay_time > 500)
//						{						
							flag_push_aim_multiple_init = 1;
							flag_push_aim_single_init = 1;
							
							BOMB_CLAW_LOOSE;//ǿ���ſ�צ��
							BOMB_CLAW_ROLL_HALF;//ǿ�Ʒ�תצ����HALF	
							
							
							lifter.target_bomb_bit[L] = lifter.target_bomb_bit_scan[L];
							lifter.target_bomb_bit[R] = lifter.target_bomb_bit_scan[R];	
							from_lifter_zero = NO;
							from_lifter_bomb_entire = YES; //�������Ŀ��ֵ

							lifter.ctrl_mode = BOMB_LIFTER_BIT;//���̧��һ��(�޸�)
							
							if(((abs(bomb_claw.total_angle[L] - bomb_claw.target_half[L]) < 100) || 
									(abs(bomb_claw.total_angle[R] - bomb_claw.target_half[R]) < 100)) && 
								 ((abs(lifter.target_bomb_bit[L] - lifter.total_angle[L]) < 50) || 
									(abs(lifter.target_bomb_bit[R] - lifter.total_angle[R]) < 50)))			
							{	
								if(get_bomb.scan_delay_record)//��ʱһС��ʱ�� ���жϸ�׼
								{
									get_bomb.scan_delay_time = millis();
									get_bomb.scan_delay_record = 0;
								}
								else if(millis() - get_bomb.scan_delay_time > 333)
								{
									if(optic_switch.value_record)//�ж�������ȥ��
									{
										optic_switch.value_start[M] = optic_switch.value[M];
										optic_switch.value_record = DISABLE;
									}												
									if(climb_tube.location == ON_ISLAND)
									{
										if(optic_switch.value_start[M])
										{
											/*��������*/			
											if((optic_switch.value[M] == 1) && (get_bomb.scan_finished == NO)) //��⵽����
											{
												BOMB_CLAW_MOVE_RIGHT;                //ǿ���Ƶ��ұ�	
												if(abs(bomb_claw.target_entire[Y] - bomb_claw.total_angle[Y]) < 100)
												{
													get_bomb.flag_scan_failed = YES;  //���ɨ��ʧ�
													get_bomb.scan_finished = YES;
												}													
											}
											else//��һ�η�ת�޸�target_mid
											{
												bomb_claw.target_half[Y] = bomb_claw.total_angle[Y] - 21000;	
												if((bomb_claw.target_half[Y] > bomb_claw.target_entire[Y]) ||
													 (bomb_claw.target_half[Y] < bomb_claw.target_zero[Y]) ||
												    get_bomb.flag_scan_failed)
												{
													bomb_claw.target_half[Y] = bomb_claw.Y_target[M];
													get_bomb.flag_scan_failed = YES;  //���ɨ��ʧ��
												}	
												
												get_bomb.scan_finished = YES;       //���ɨ�����
												BOMB_CLAW_MOVE_MID;                 //ǿ���Ƶ��м�
												flag_push_aim_scan_init = 0;
											}								
										}
										else
										{
											/*��������*/
											if((optic_switch.value[M] == 0) && (get_bomb.scan_finished == NO))
											{
												BOMB_CLAW_MOVE_RIGHT;                //ǿ���Ƶ��ұ�															
												if(abs(bomb_claw.target_entire[Y] - bomb_claw.total_angle[Y]) < 100)
												{
													get_bomb.flag_scan_failed = YES;  //���ɨ��ʧ��
													get_bomb.scan_finished = YES;
												}												
											}
											else //��һ�η�ת�޸�target_mid
											{
												bomb_claw.target_half[Y] = bomb_claw.total_angle[Y] + 16000;
												if((bomb_claw.target_half[Y] > bomb_claw.target_entire[Y]) ||
													 (bomb_claw.target_half[Y] < bomb_claw.target_zero[Y]) ||
												    get_bomb.flag_scan_failed)
												{
													bomb_claw.target_half[Y] = bomb_claw.Y_target[M];
													get_bomb.flag_scan_failed = YES;  //���ɨ��ʧ��
												}	
												/*�������������Ϊ���ٶȿɵ�*/
												bomb_claw.from_claw_entire[Y_axis] = NO;
												bomb_claw.from_claw_zero[Y_axis] = YES;									

												get_bomb.scan_finished = YES;       //���ɨ�����
												BOMB_CLAW_MOVE_MID;                 //ǿ���Ƶ��м�
												flag_push_aim_scan_init = 0;
											}						
										}
									}
									else if(climb_tube.location == ON_GROUND)
									{
										if(optic_switch.value_start[M] == 0)
										{
											/*ֻ���������� ������֮����ܴ�������ɨ��*/			
											if((optic_switch.value[M] == 0) && (get_bomb.scan_finished == NO)) //��⵽����
											{
												BOMB_CLAW_MOVE_RIGHT;                //ǿ���Ƶ��ұ�				
												if(abs(bomb_claw.target_entire[Y] - bomb_claw.total_angle[Y]) < 100)
												{
													get_bomb.flag_scan_failed = YES;  //���ɨ��ʧ��
													get_bomb.scan_finished = YES;
												}												
											}
											else //��һ�η�ת�޸�target_mid
											{
												bomb_claw.target_half[Y] = bomb_claw.total_angle[Y] - 8500;	
												if((bomb_claw.target_half[Y] > bomb_claw.target_entire[Y]) ||
													 (bomb_claw.target_half[Y] < bomb_claw.target_zero[Y]) ||
												    get_bomb.flag_scan_failed)
												{
													bomb_claw.target_half[Y] = bomb_claw.Y_target[M];
													get_bomb.flag_scan_failed = YES;  //���ɨ��ʧ��
												}	
												
												get_bomb.scan_finished = YES;       //���ɨ�����
												BOMB_CLAW_MOVE_MID;                 //ǿ���Ƶ��м�
												flag_push_aim_scan_init = 0;
											}								
										}
										else get_bomb_clear_record = 1;//�����˳���������
									}							
								}
							}							
//						}
					}	
				}
				
        if(get_bomb.scan_finished)				
				{		
		      /*�������Ŀ��ֵ*/ 
          if(lifter.target_bomb_bit[L] != lifter.target_bomb_bit_normal[L] || 
						 lifter.target_bomb_bit[R] != lifter.target_bomb_bit_normal[R])		
	       	{					
						lifter.target_bomb_bit[L] = lifter.target_bomb_bit_normal[L];
						lifter.target_bomb_bit[R] = lifter.target_bomb_bit_normal[R];
						if((lifter.total_angle[L] > lifter.target_bomb_bit[L]) && 
							 (lifter.total_angle[R] < lifter.target_bomb_bit[R]))
						{
							from_lifter_zero = YES;
							from_lifter_bomb_entire = NO; 			
						}
						else if((lifter.total_angle[L] < lifter.target_bomb_bit[L]) && 
										(lifter.total_angle[R] > lifter.target_bomb_bit[R]))
						{
							from_lifter_zero = NO;
							from_lifter_bomb_entire = YES; 			
						}
					}
					
					if((abs(lifter.target_bomb_bit[L] - lifter.total_angle[L]) < 100) || 
						 (abs(lifter.target_bomb_bit[R] - lifter.total_angle[R]) < 100))
					{
					  kb.bomb_claw_enable = ENABLE;//ȡ��צ�ӿɿ�	
            GIM_SERVO_SIDE;						

						if(climb_tube.location == ON_ISLAND && !get_bomb.flag_scan_failed)
						{
							 BOMB_CLAW_PUSH;
						}
//						else BOMB_CLAW_PULL;

//						if((flag_bomb_claw_catch) && 
//						((abs(bomb_claw.target_zero[L] - bomb_claw.total_angle[L]) < 100) || 
//						 (abs(bomb_claw.target_zero[R] - bomb_claw.total_angle[R]) < 100)))							
						if((flag_bomb_claw_catch) &&(bomb_claw.zero_correct == FINISHED))
						{	
							if(get_bomb.catch_confirm_record)//��ʱһС��ʱ����צ�Ӽ�ס
							{					
								get_bomb.catch_confirm_delay_time = millis();
								get_bomb.catch_confirm_record = DISABLE;
							}
							else if(millis() - get_bomb.catch_confirm_delay_time > 125)
							{
								get_bomb.flag_push_aim_scan = FINISHED;//���������Ƴ���׼ģʽ
								get_bomb.catch_confirm_record = ENABLE;
							}
						}					
					}	
					
				}		
				
			}break;				
					
			case GET_POUR_MODE://ȡ������ģʽ
			{		
			  chassis.ass_mode_enable = ENABLE;      //������βģʽ
				chassis.force_push_in_enable = ENABLE; //�����ƽ�ģʽ
				chassis.ctrl_mode = CHASSIS_STOP;      //�����ٶȻ���ס
				vision.enable = DISABLE;
				flag_bomb_claw_vision = 0;		
				
				if(get_bomb.get_pour_step == 0)//step 0
				{	 
					kb.bomb_claw_enable = DISABLE;        //ȡ��צ�Ӳ��ɿ�	
					lifter.ctrl_mode = BOMB_LIFTER_ENTIRE;//���̧���γ�����	
					if((abs(lifter.target_bomb_entire[L] - lifter.total_angle[L]) < 100) || 
						 (abs(lifter.target_bomb_entire[R] - lifter.total_angle[R]) < 100))//̧����Ŀ��Ƕ�
					{ /*���̧����λ�ò�ִ�е���*/
						BOMB_CLAW_PULL;                    //ȡ��צ���ջ�
						/*��������ֵ*/	
						if(bomb_claw.target_half[Y] > bomb_claw.Y_target[M])
						{
							bomb_claw.from_claw_entire[Y_axis] = YES;
							bomb_claw.from_claw_zero[Y_axis] = NO;
						}	
						else if(bomb_claw.target_half[Y] < bomb_claw.Y_target[M])
						{
							bomb_claw.from_claw_entire[Y_axis] = NO;
							bomb_claw.from_claw_zero[Y_axis] = YES;
						}													
						bomb_claw.target_half[Y] = bomb_claw.Y_target[M];  
						
						BOMB_CLAW_MOVE_MID;                //ȡ��צ���Ƶ��м�
						BOMB_CLAW_ROLL_BIT;                //ȡ��צ�ӷ�ת��BIT
						get_bomb.get_pour_step ++;
					}		
				}							
						
			  else if(get_bomb.get_pour_step == 1)//step 1
				{
//					if((abs(bomb_claw.target_half[L] - bomb_claw.total_angle[L]) < 100) || 
//						 (abs(bomb_claw.target_half[R] - bomb_claw.total_angle[R]) < 100))							
//					{
					lifter.ctrl_mode = BOMB_LIFTER_BIT;//��ܻص�ȡ���Ǹ�λ��							
//					}
							
					if((abs(bomb_claw.target_bit[L] - bomb_claw.total_angle[L]) < 100) || 
						 (abs(bomb_claw.target_bit[R] - bomb_claw.total_angle[R]) < 100))							
					{										
//						BOMB_CLAW_LOOSE;                    //ȡ��צ���ɿ�
						get_bomb.claw_loose_Time = millis();//��¼צ���ɿ���ʱ��ms
						get_bomb.get_pour_step ++;
					}								
				}
						
				else if(get_bomb.get_pour_step == 2)//step 2
				{
					if((millis() - get_bomb.claw_loose_Time > 200) && 
						 (millis() - get_bomb.claw_loose_Time < 350))//ms
					{
						BOMB_CLAW_LOOSE;                    //ȡ��צ���ɿ�
					}
					else if((millis() - get_bomb.claw_loose_Time > 350) && 
						      (millis() - get_bomb.claw_loose_Time < 500))//ms
					{
						BOMB_CLAW_ROLL_HALF;                //
					}					
					else if(millis() - get_bomb.claw_loose_Time > 500)//ms
					{
						BOMB_EJECT_PUSH;                    //�����Ƴ�
						get_bomb.eject_push_Time = millis();//��¼�����ʱ��ms
						get_bomb.get_pour_step ++;
					}
				}
						
				else if(get_bomb.get_pour_step == 3)//step 3
				{
					if(millis() - get_bomb.eject_push_Time > 300)//ms
					{
						get_bomb.get_pour_step = 0;//ȡ����������		
						get_bomb.have_finished_once_get_bomb = YES;
						
						get_bomb.flag_push_aim_single = HAVENT;    //��û����Ƴ���׼ģʽ											
						get_bomb.flag_push_aim_multiple = HAVENT;  //��û����Ƴ���׼ģʽ	
						get_bomb.flag_push_aim_scan = HAVENT;      //��û����Ƴ���׼ģʽ		
						
						BOMB_EJECT_PULL;        //�����ջ�								
						flag_bomb_claw_roll = 0;//ʹ�˳����ٽ���PUH_AIM��ENABLEʱצ�����Զ�HALF
								
						/*�ڵ��ϵ�ʱ��Ĭ��ȡ��צ���Ƴ� ���µ�ʱ��Ĭ�ϲ��Ƴ�*/
//						if(((climb_tube.location == ON_ISLAND) || (flag_bomb_claw_record  == 1)) && (get_bomb.box_num != 2))//���ϻ��Ǽ�¼���Ƴ�
						if(flag_bomb_claw_record  == 1 && get_bomb.box_num != 2)//���ϻ��Ǽ�¼���Ƴ�
						{
							flag_bomb_claw_push = 1;		
						}				
						else//���� 
						{
							flag_bomb_claw_push = 0;//����ģʽ��Ĭ�ϲ��Ƴ�	
              flag_bomb_claw_record  = 0;							
						}
						
						
						if(get_bomb.once_triple)/*���䶼ȡ��֮���ٹص�����ģʽ*/
						{							
							if(get_bomb.box_num < 2) 
							{
								get_bomb.box_num ++;
							}
							else 
							{
								get_bomb.box_num = 0;	
								
                get_bomb.once_triple = 0;	
                flag_bomb_claw_triple = 0; //�ص�����ģʽ								
							}						
						}		
						else if(get_bomb.once_penta)/*���䶼ȡ��֮���ٹص�����ģʽ*/
						{							
							if(get_bomb.box_num < 4) 
							{
								get_bomb.box_num ++;
							}
							else 
							{
								get_bomb.box_num = 0;	
								
                get_bomb.once_penta = 0;	
                flag_bomb_claw_penta = 0; //�ص�����ģʽ		

								flag_bomb_claw_push = 0;//����ģʽ��Ĭ�ϲ��Ƴ�	
								flag_bomb_claw_record  = 0;									
							}						
						}	
						
            /*����ɨ���һЩ����*/						
//						if(get_bomb.scan)
//						{		
							get_bomb.scan = 0;	
						  get_bomb.flag_scan_failed = NO;
						  optic_switch.value_record	= ENABLE;	
              get_bomb.scan_delay_record = ENABLE;							
	            get_bomb.scan_finished = NO;	
              flag_bomb_claw_scan = 0; //�ص�ɨ��ģʽ		
//						}
						
					}					
				}
					
			}break;
				
//			case VISION_ASSIST_MODE://�Ӿ���λģʽ
//			{								
//				vision.enable = ENABLE;
//				if(vision.aim == FINISHED)
//				{
//					vision.enable = DISABLE;
//					vision.aim = HAVENT;
//					get_bomb.vision_assist = 0;	
//					flag_bomb_claw_vision = 0;               //�ص��Ӿ�ģʽ ���´ΰ�ctrl+R Ĭ���ǵ���ģʽ		
//				}	

//			}break;			
					
			case SUSPEND_MODE://����̬ģʽ
			{	
        //do nothing 				
			}break;
			default: break;		
		}		
	}
	
}

void get_bomb_task(void)
{
//	if(bomb_claw.ALL_reset)
//	{
		get_bomb_ctrl();
//	}
}


