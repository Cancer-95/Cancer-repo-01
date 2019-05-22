#include "climb_tube.h"

/*------------------------------------------
����launchλ�Ĺ��ܽ��ͣ�
����һ:��������
���ܶ�:��צ�ӵ����

��ϸ���͹��ܶ���
�����ǰ���F��������� ����ûת��ȥ ���ʱ�����ǰ�ctrl+Z����ƽԭģʽ�����ǻ�ǿ�ƴ�צ�ӵ� ��ֱ�ӵ�����
�������launchλ ��λǡ�ɼ�¼���������Ƿ�����F�� Ҫ�ǰ����� ��ƽԭģʽ��ʱ��צ�ӻ���ǿ�ƺ��ϵ�
����û����F��ֻ����ctrl+Q�����ǵǵ����� �Ҷ���launchλ����� �����ٻ�ƽԭģʽ��ʱ��ͻ�ǿ���ſ�צ��
���ʱ��ָ�ƽԭģʽ������жϳ��ӵ���ʲôλ�� ������ٰ�ctrl+Q��ʱ�� ���չ����

ע��������ctrl+Q����ʱ�� צ�Ӷ��ǲ��ɿص� ��������Ϊ����ȡ��צ�ӹ���һ������
------------------------------------------*/

/* get information task period time (ms) */
#define LANDING_ANGLE_PERIOD     1 //�����ĸ�ʱ��� calc�������ĸ�ʱ���

/* key acceleration time */
#define LANDING_ANGLE_ACC_TIME   1444  //ms

climb_tube_t climb_tube;
void climb_tube_param_init(void)
{				
	climb_tube.location = ON_GROUND;         //Ĭ����ƽ��
//  kb.tube_claw_enable = ENABLE;            //צ��ʹ��	
	kb.tube_claw_enable = DISABLE;           //צ��ʧ��
	TUBE_CLAW_LOOSE;                         //צ�Ӹ�λ�Ǵ�
//	TUBE_BRAKE_PULL;                         //ɲ������
	
	ducted.ctrl_mode = DUCTED_ALL_OFF;       //�غ���
	optic_switch.ctrl_mode = DETECT_ENABLE;  //��翪�ؿ������ 
//  assist_wheel.ctrl_mode = WHEEL_HOLD;     //Ħ������ס			
	lifter.ctrl_mode = LIFTER_ZERO;	   //��ܻر궨��λ��
						
	climb_tube.fiag_launch = 0;              //launchλ ��0			
	climb_tube.twisted_angle = 0;            //�Ѿ�ת���ĽǶ���0
	climb_tube.yaw_record = 0;               //��¼�ĽǶ���0
	climb_tube.yaw_record_enable = ENABLE;   //��¼�Ƕ�ʹ��
	climb_tube.twist_calc_enable = DISABLE;  //�Ƕȼ���ʧ��	
	
	climb_tube.landing_angle_record = ENABLE; //��¼��½�Ƕ�
	climb_tube.landing_angle_delay_time_record = ENABLE;//��½����ʱ
	ramp_init(&landing_angle_ramp, LANDING_ANGLE_ACC_TIME / LANDING_ANGLE_PERIOD);

	climb_tube.climb_up_optic_enable = ENABLE;
	climb_tube.climb_down_optic_enable = ENABLE;
	climb_tube.climb_up_gyro_enable = ENABLE;
	climb_tube.climb_down_gyro_enable = ENABLE;
		
//	climb_tube.climb_down_delay_time_record = ENABLE;
	
/*ע�͵�����������ζ�ſ��������S1����������*/	
//			  climb_tube.climb_enable = ENABLE;        //�������Ǹ�ƽԭģʽ��һ��(��Ҫ��Ϊ��Ҫ�ǳ�ʼ������S1�����и�������ģʽ)		
}

void climb_tube_ctrl(void)
{
	static bool LAUNCH_RECORD = DISABLE;            //��F�������ı�־λ
	
	keyboard_climb_tube_handler();//keyboard	
		
	if(climb_tube.climb_enable == ENABLE)	
	{
		switch(climb_tube.ctrl_mode)
		{
			case PLAIN_MODE://ƽԭģʽ
			{
				memset(&global_task, ENABLE, sizeof(global_task_t)); //����������Ҳ��ִ��

				LAUNCH_RECORD = DISABLE;                        //�Զ�����λ ��0								
 				kb.tube_claw_enable = DISABLE;                  //צ��ʧ��		
        
				if(chassis.ass_mode_enable)
				{
					GIM_SERVO_BACK;	
				}
				else GIM_SERVO_ZERO;

//				if(climb_tube.location == ON_GROUND)GIM_SERVO_ZERO;	
//				else GIM_SERVO_BACK;
		
        chassis.ctrl_mode = CHASSIS_FOLLOW;             //����������ģʽ
				
				/*��һ����ս�� ��������ʹ�� ������F������Ī�÷�Ӧ*/
				if(climb_tube.fiag_launch) 
				{
				  TUBE_CLAW_CATCH;                       //צ��ǿ�ƿ���			
				  chassis.ctrl_mode = CHASSIS_SEPARATE;  //���̲�Ҫ�Ҷ� ��ʱ�����ö���ײһ�°�ʲô��
					climb_tube.climb_enable = ENABLE;      //ֻ�����caseһֱ��
					
					if(optic_switch.value[S] == 0)// && optic_switch.value[R] == 0)//����⵽������ �ж�Ϊƽ����
					{
						climb_tube.location = ON_GROUND;
					}
					else if(optic_switch.value[S] == 1)// && optic_switch.value[R] == 1)
					{
						climb_tube.location = ON_ISLAND;//����⵽���ŵ� �ж�Ϊ����
					}
					else climb_tube.location = IN_DANGER;//��������ж�ΪΣ�� ʲôҲ������ ��ctrl+Q �����ܽ�������part
				}
				else
				{
					TUBE_CLAW_LOOSE;                       //צ��ǿ���ſ� 			
					climb_tube.climb_enable = DISABLE;     //ֻ�����case��һ��				
				}				
        /*-----------------------------------------------*/
				
//				TUBE_BRAKE_PULL;                         //ɲ������
		
				optic_switch.ctrl_mode = DETECT_ENABLE;  //��翪�ؿ������ ��仰��Ϊ�������caseд��
				lifter.ctrl_mode = LIFTER_ZERO;	         //��ܻر궨��λ��
//			  assist_wheel.ctrl_mode = WHEEL_HOLD;     //Ħ������ס			
			  ducted.ctrl_mode = DUCTED_ALL_OFF;       //�غ���
					
				climb_tube.twisted_angle = 0;            //�Ѿ�ת���ĽǶ���0
				climb_tube.yaw_record = 0;               //��¼�ĽǶ���0
			  climb_tube.yaw_record_enable = ENABLE;   //��¼�Ƕ�ʹ��
				climb_tube.twist_calc_enable = DISABLE;  //�Ƕȼ���ʧ��	
				
				climb_tube.landing_angle_record = ENABLE; //��¼��½�Ƕ�
				climb_tube.landing_angle_delay_time_record = ENABLE;//��½����ʱ
				ramp_init(&landing_angle_ramp, LANDING_ANGLE_ACC_TIME / LANDING_ANGLE_PERIOD);
				
				climb_tube.climb_up_optic_enable = ENABLE;
				climb_tube.climb_down_optic_enable = ENABLE;
				climb_tube.climb_up_gyro_enable = ENABLE;
				climb_tube.climb_down_gyro_enable = ENABLE;
	
	  		climb_tube.vision_enable = DISABLE;//�ص��Ӿ�
				vision.enable = DISABLE;		  
				flag_tube_claw_vision = 0;
				
//				climb_tube.climb_down_delay_time_record = ENABLE;
				
			}break;
			  
			case CLIMEB_UP_OPTIC_MODE://����ģʽ (��緽��)
			{ 
				chassis.ass_mode_enable = DISABLE;//Ĭ�ϲ�����βģʽ Ҫ���Ļ��ں���ENABLE
				
				if((LAUNCH_TRIGGER && flag_tube_claw_catch) || LAUNCH_RECORD)
				{
//					TRAILER_HOOK_CAPTURE;//�ϳ���������
					GIM_SERVO_BACK;
					LAUNCH_RECORD = ENABLE;					
					
					if(climb_tube.twist_calc_enable)
					{
						critical_value_treatment(imu.yaw, &climb_tube.yaw_record, 0, 360);   //�ٽ�ֵ����	
						climb_tube.twisted_angle = climb_tube.yaw_record - imu.yaw;          //ת���ĽǶ�						
					}
					
					if(climb_tube.fiag_launch == 0 && optic_switch.value[S] == 1)// && optic_switch.value[R] == 1)//һ��F�����
					{
						chassis.ctrl_mode = MOVE_CLOCKWISE;                      //���̻�еģʽ
//						ducted.ctrl_mode = DUCTED_L_RUN;                           //�󺭵���
						ducted.ctrl_mode = DUCTED_ALL_RUN;                           //ȫ������
						kb.tube_claw_enable = DISABLE;                                
            TUBE_CLAW_CATCH;                                           //צ��ǿ��ץס
						 
						lifter.ctrl_mode = LIFTER_ZERO;		                         //��ܻر궨��λ��
						optic_switch.ctrl_mode = DETECT_ENABLE;		                 //���������
					}	
					else if(climb_tube.fiag_launch == 0 && optic_switch.value[S] == 0)// && optic_switch.value[R] == 0)//
					{
//						ducted.ctrl_mode = DUCTED_L_RUN;                           //�󺭵���
//						assist_wheel.ctrl_mode = WHEEL_FORWARD;                    //Ħ����ת
						climb_tube.fiag_launch = 1;					                       //����λ��1
					}
					else if(climb_tube.fiag_launch == 1 && 
					       ((abs(lifter.total_angle[L] - lifter.target_zero[L]) < 200) || 
					       (abs(lifter.total_angle[R] - lifter.target_zero[R]) < 200)) && 
					        optic_switch.value[S] == 0)// && optic_switch.value[R] == 0)//
					{
//						assist_wheel.ctrl_mode = WHEEL_FORWARD_SLOW;               //Ħ������ת
//						ducted.ctrl_mode = DUCTED_R_RUN;                           //�Һ�����
						
			  		climb_tube.twisted_angle = 0;                               //�Ѿ�ת���ĽǶ���0
				  	climb_tube.yaw_record = 0;                                  //��¼�ĽǶ���0
				    climb_tube.yaw_record_enable = ENABLE;                      //��¼�Ƕ�ʹ��
				    climb_tube.twist_calc_enable = DISABLE;                     //�Ƕȼ���ʧ��
					}				
					else if(climb_tube.yaw_record_enable == ENABLE && climb_tube.fiag_launch == 1 && 
					        ((abs(lifter.total_angle[L] - lifter.target_zero[L]) < 200) || 
					        (abs(lifter.total_angle[R] - lifter.target_zero[R]) < 200)) && 
					        optic_switch.value[S] == 1)// && optic_switch.value[R] == 1)//��綼��⵽̨��&̧������λ��
					{
//						TUBE_BRAKE_PUSH;		                                         //ɲ������
//						ducted.ctrl_mode = DUCTED_R_RUN;                           //�Һ�����
						climb_tube.yaw_record = imu.yaw;                             //��ȡ�����ǽǶ���Ϊ��ǰ�Ƕ�
						climb_tube.yaw_record_enable = DISABLE;                      //ֻ��¼һ��
						climb_tube.twist_calc_enable = ENABLE;                       //��������
					}					
					else if((abs(climb_tube.twisted_angle - 38) < 1) && climb_tube.fiag_launch == 1 && 
						      optic_switch.value[S] == 1)// && optic_switch.value[R] == 1)//������ת����λ��
					{
						climb_tube.twisted_angle = 38;                               //ǿ��������������Ƕ�
						climb_tube.twist_calc_enable = DISABLE;                      //��Ҫ������
						
						ducted.ctrl_mode = DUCTED_ALL_OFF;                           //�غ���
//						assist_wheel.ctrl_mode = WHEEL_HOLD;                         //Ħ����holdס
						lifter.ctrl_mode = TUBE_LIFTER_BIT;                         //���̧��һ���
						optic_switch.ctrl_mode = FORCE_ONE;                          //��翪��ǿ����1	������
						if((abs(lifter.target_tube_bit[L] - lifter.total_angle[L]) < 120) || 
							 (abs(lifter.target_tube_bit[R] - lifter.total_angle[R]) < 120))//̧����Ŀ��Ƕ�
						{
							TUBE_CLAW_LOOSE;		                                       //צ���ɿ�
//              TUBE_BRAKE_PULL;									                         //ɲ���ջ�
							chassis.ctrl_mode = MOVE_FORWARD;                          //������ǰ��
              climb_tube.location = ON_ISLAND;/*��ǰһ�����Ϊ���� ��֪����ʲô����*/	
							
							if(FORWARD || BACK || GET_BOMB_CTRL)
							{
								climb_tube.fiag_launch = 0;              //launchλ ��0			
								climb_tube.ctrl_mode = PLAIN_MODE;	//ǰ�������л�ƽԭģʽ
                chassis.ass_mode_enable = ENABLE;			
								climb_tube.location = ON_ISLAND;    //�Ѿ��ڵ��Ͽ�					
		 					}
						}
					}			
				}
				else 	
				{
					kb.tube_claw_enable = ENABLE;                                //צ�ӿɿ�
					
					lifter.ctrl_mode = TUBE_LIFTER_ENTIRE;		                   //���̧�� ̧��Ŀ��֮�󱣳�
					
					if(flag_tube_claw_catch)
					{
//				   	TRAILER_HOOK_RELEASE;                                         //�������׵��¸���һ��
						chassis.ctrl_mode = CHASSIS_SEPARATE;  //���̻�еģʽ(������)
						
						climb_tube.vision_enable = DISABLE;//�ص��Ӿ�
						vision.enable = DISABLE;		  
						flag_tube_claw_vision = 0;//�´�Ĭ�ϴ��Ӿ�
					}
					else 
					{
//					  TRAILER_HOOK_CAPTURE;   
						if(lifter.total_angle[R] > lifter.target_bomb_entire[R] || 
							 lifter.total_angle[L] < lifter.target_bomb_entire[L])
						{
							climb_tube.vision_enable = ENABLE;//���Ӿ��ɿ�
						}	
						
						if(vision.enable == DISABLE)
						{
							chassis.ctrl_mode = CHASSIS_SEPARATE;//CHASSIS_FOLLOW; //����������
						}
						else if(vision.captured)//ץ��ͼ����
						{
					  	chassis.ctrl_mode = VISION_CLIMB_TUBE;
						}
						else chassis.ctrl_mode = CHASSIS_SEPARATE;//CHASSIS_FOLLOW; //����������

					}
					
					optic_switch.ctrl_mode = FORCE_ONE;                          //��翪��ǿ����1  ���̧������������ȫ����1 �Ա�֤�����
				  
//					if(lifter.total_angle[R] > lifter.target_bomb_bit[R] || 
//						 lifter.total_angle[L] < lifter.target_bomb_bit[L])
//					{
				  	GIM_SERVO_SIDE;
//					}
					
					climb_tube.fiag_launch = 0;                                  //launchλ ��0			
				}
			}break;
			
			case CLIMEB_DOWN_OPTIC_MODE://�¸�ģʽ (��緽��)
			{ 
				chassis.ass_mode_enable = DISABLE;//Ĭ�ϲ�����βģʽ Ҫ���Ļ��ں���ENABLE
				
				if((LAUNCH_TRIGGER && flag_tube_claw_catch) || LAUNCH_RECORD)
				{
//					TRAILER_HOOK_CAPTURE;//�ϳ���������					
					LAUNCH_RECORD = ENABLE;
					GIM_SERVO_ZERO;
					
//					if(climb_tube.climb_down_delay_time_record)//
//					{
//						climb_tube.climb_down_delay_time = millis();
//						climb_tube.climb_down_delay_time_record = DISABLE;
//					}
//					else if(millis() - climb_tube.climb_down_delay_time > 500)
//					{
						if(climb_tube.twist_calc_enable)
						{						
							critical_value_treatment(imu.yaw, &climb_tube.yaw_record, 0, 360);   //�ٽ�ֵ����	
							climb_tube.twisted_angle = imu.yaw - climb_tube.yaw_record;          //ת���ĽǶ�						
						}
					
						if(climb_tube.fiag_launch == 0 && optic_switch.value[S] == 1)// && optic_switch.value[R] == 1)//��ʵ����һ����F�������
						{
							chassis.ctrl_mode = MOVE_ANTICLOCKWISE;                         //���̻�еģʽ
							
							kb.tube_claw_enable = DISABLE;                                
							TUBE_CLAW_CATCH;                                              //צ��ǿ��ץס
							
							lifter.ctrl_mode = LIFTER_ZERO;		                            //��ܻر궨��λ��
							optic_switch.ctrl_mode = DETECT_ENABLE;                       //��翪�ؿ��ż��	
							ducted.ctrl_mode = DUCTED_ALL_OFF;                            //ȫ������					
	//						ducted.ctrl_mode = DUCTED_R_RUN;                              //�Һ�����
	//						assist_wheel.ctrl_mode = WHEEL_REVERSE;                     //Ħ����ת
							climb_tube.fiag_launch = 1;					                          //����λ��1
						}
	//					else if(optic_switch.value[L] == 1 && optic_switch.value[R] == 0 && climb_tube.fiag_launch == 1 && ((abs(lifter.total_angle[L] - lifter.target_zero[L]) < 200) || (abs(lifter.total_angle[R] - lifter.target_zero[R]) < 200)))//
						else if(climb_tube.fiag_launch == 1 && optic_switch.value[S] == 1)// && optic_switch.value[R] == 0)
						{
	// 						assist_wheel.ctrl_mode = WHEEL_REVERSE_SLOW;                   //Ħ������ת
	//						ducted.ctrl_mode = DUCTED_L_RUN;                               //�󺭵���
							
							climb_tube.twisted_angle = 0;                                 //�Ѿ�ת���ĽǶ���0
							climb_tube.yaw_record = 0;                                    //��¼�ĽǶ���0
							climb_tube.yaw_record_enable = ENABLE;                        //��¼�Ƕ�ʹ��
							climb_tube.twist_calc_enable = DISABLE;                       //�Ƕȼ���ʧ��
						}				

	//					else if(optic_switch.value[L] == 0 && optic_switch.value[R] == 0 && climb_tube.yaw_record_enable == ENABLE && climb_tube.fiag_launch == 1 && ((abs(lifter.total_angle[L] - lifter.target_zero[L]) < 200) || (abs(lifter.total_angle[R] - lifter.target_zero[R]) < 200)))//��綼��⵽����&̧������λ��
						else if(climb_tube.yaw_record_enable == ENABLE && climb_tube.fiag_launch == 1 && 
										optic_switch.value[S] == 0)// && optic_switch.value[R] == 0)				
						{
	//						TUBE_BRAKE_PUSH;		                                           //ɲ������
	//						ducted.ctrl_mode = DUCTED_L_RUN;                               //�󺭵���
							
							climb_tube.yaw_record = imu.yaw;                               //��ȡ�����ǽǶ���Ϊ��ǰ�Ƕ�
							climb_tube.yaw_record_enable = DISABLE;                        //ֻ��¼һ�� 
							climb_tube.twist_calc_enable = ENABLE;                         //��������
						}					
						else if((abs(climb_tube.twisted_angle - 65) < 1) && climb_tube.fiag_launch == 1 && 
										optic_switch.value[S] == 0)// && optic_switch.value[R] == 0)//������ת����λ��
						{
							climb_tube.twisted_angle = 65;                                  //ǿ��������������Ƕ�
							climb_tube.twist_calc_enable = DISABLE;                         //��Ҫ������
							
							ducted.ctrl_mode = DUCTED_ALL_OFF;                              //�غ���
	//						assist_wheel.ctrl_mode = WHEEL_HOLD;                           //Ħ����holdס
							lifter.ctrl_mode = TUBE_LIFTER_ENTIRE;                         //���̧��
	//						lifter.ctrl_mode = TUBE_LIFTER_BIT;                          //���̧��
							optic_switch.ctrl_mode = FORCE_ZERO;                            //��翪��ǿ����0 �������Ҳ�������elseif��ĳ���
							chassis.ctrl_mode = CHASSIS_SEPARATE;                      //���̻�еģʽ
							if((abs(lifter.target_tube_entire[L] - lifter.total_angle[L]) < 120) || 
								 (abs(lifter.target_tube_entire[R] - lifter.total_angle[R]) < 120))//̧����Ŀ��Ƕ�
							{
								TUBE_CLAW_LOOSE;                                              //צ���ɿ�
	//							TUBE_BRAKE_PULL;									                            //ɲ���ջ�
								chassis.ctrl_mode = MOVE_FORWARD;                             //������ǰ��
	//							GIM_SERVO_ZERO;
								if(FORWARD || BACK || LEFT || RIGHT)  
								{
									climb_tube.fiag_launch = 0;              //launchλ ��0			
									climb_tube.ctrl_mode = PLAIN_MODE;	//ǰ�������л�ƽԭģʽ
									chassis.ass_mode_enable = DISABLE;  //��βģʽ�ر�		    
									climb_tube.location = ON_GROUND;    //�Ѿ���ƽ���Ͽ�		  								
								}
							}
						}							
//					}	
				}
				else
				{
					kb.tube_claw_enable = ENABLE;                                //צ�ӿɿ�
										
					if(flag_tube_claw_catch)
					{
//				   	TRAILER_HOOK_RELEASE;                                         //�������׵��¸���һ��
						chassis.ctrl_mode = CHASSIS_SEPARATE;                      //���̻�еģʽ
					}
					else 
					{
//						TRAILER_HOOK_CAPTURE;
						chassis.ctrl_mode = CHASSIS_FOLLOW;                        //����������ģʽ
					}
					
					optic_switch.ctrl_mode = FORCE_ONE;                          //��翪��ǿ����1  ���̧������������ȫ����1 �Ա�֤�����										
					lifter.ctrl_mode = TUBE_LIFTER_BIT;		                       //���̧��һ��� ̧��Ŀ��֮�󱣳�
					GIM_SERVO_SIDE;
					climb_tube.fiag_launch = 0;              //launchλ ��0			
				}				
			}break;
			
			case CLIMEB_UP_GYRO_MODE://����ģʽ (�����Ƿ���)
			{ 
				chassis.ass_mode_enable = DISABLE;//Ĭ�ϲ�����βģʽ Ҫ���Ļ��ں���ENABLE
				
				if((LAUNCH_TRIGGER && flag_tube_claw_catch) || LAUNCH_RECORD)
				{
					GIM_SERVO_BACK;
					LAUNCH_RECORD = ENABLE;					
					
					if((((abs(lifter.total_angle[L] - lifter.target_zero[L]) < 200) || 
					     (abs(lifter.total_angle[R] - lifter.target_zero[R]) < 200)) && 
					     (abs(climb_tube.twisted_angle - 149) < 1)) || climb_tube.fiag_launch)//
					{					
		        climb_tube.fiag_launch = 1;
						climb_tube.twisted_angle = 149;                             //ǿ��������������Ƕ�											
						ducted.ctrl_mode = DUCTED_ALL_OFF;                          //�غ���
						lifter.ctrl_mode = TUBE_LIFTER_BIT;                         //���̧��һ���
						
						if((abs(lifter.target_tube_bit[L] - lifter.total_angle[L]) < 120) || (abs(lifter.target_tube_bit[R] - lifter.total_angle[R]) < 120))//̧����Ŀ��Ƕ�
						{
							TUBE_CLAW_LOOSE;		                                       //צ���ɿ�
							chassis.ctrl_mode = MOVE_FORWARD;                          //������ǰ��
							climb_tube.location = ON_ISLAND;/*��ǰһ�����Ϊ���� ��֪����ʲô����*/	

							
							if(FORWARD || BACK || GET_BOMB_CTRL)
							{	
								climb_tube.fiag_launch = 0;
								climb_tube.ctrl_mode = PLAIN_MODE;	//ǰ�������л�ƽԭģʽ
                chassis.ass_mode_enable = ENABLE;			
								climb_tube.location = ON_ISLAND;    //�Ѿ��ڵ��Ͽ�					
		 					}
						}
					}	
					else 
					{
						critical_value_treatment(imu.yaw, &climb_tube.yaw_record, 0, 360);   //�ٽ�ֵ����	
						climb_tube.twisted_angle = climb_tube.yaw_record - imu.yaw;          //ת���ĽǶ�						
					
						chassis.ctrl_mode = MOVE_CLOCKWISE;                          //���̻�еģʽ
						ducted.ctrl_mode = DUCTED_ALL_RUN;                           //ȫ������
						kb.tube_claw_enable = DISABLE;                                
            TUBE_CLAW_CATCH;                                           //צ��ǿ��ץס					 
						lifter.ctrl_mode = LIFTER_ZERO;		                         //��ܻر궨��λ��
					}	
				}
				else 	
				{
					kb.tube_claw_enable = ENABLE;                                //צ�ӿɿ�
				
					lifter.ctrl_mode = TUBE_LIFTER_ENTIRE;		                   //���̧�� ̧��Ŀ��֮�󱣳�
									
					if(flag_tube_claw_catch)
					{
						chassis.ctrl_mode = CHASSIS_SEPARATE;                      //���̻�еģʽ
					
						climb_tube.vision_enable = DISABLE;//�ص��Ӿ�
						vision.enable = DISABLE;		  
						flag_tube_claw_vision = 0;//�´�Ĭ�ϴ��Ӿ�
					}
					else 
					{
						if(lifter.total_angle[R] > lifter.target_bomb_entire[R] || 
							 lifter.total_angle[L] < lifter.target_bomb_entire[L])
						{
							climb_tube.vision_enable = ENABLE;//���Ӿ��ɿ� Ĭ�ϴ�
						}
						
						if(vision.enable == DISABLE)
						{
							chassis.ctrl_mode = CHASSIS_SEPARATE;//CHASSIS_FOLLOW; //����������
						}
						else if(vision.captured)
						{
					  	chassis.ctrl_mode = VISION_CLIMB_TUBE;
						}
						else chassis.ctrl_mode =CHASSIS_SEPARATE;// CHASSIS_FOLLOW; //����������
					}		
					
				  
//					if(lifter.total_angle[R] > lifter.target_bomb_bit[R] || 
//						 lifter.total_angle[L] < lifter.target_bomb_bit[L])
//					{
				  	GIM_SERVO_SIDE;
//					}		
          climb_tube.yaw_record = imu.yaw;                             //��ȡ�����ǽǶ���Ϊ��ǰ�Ƕ�				
					climb_tube.fiag_launch = 0;                                  //launchλ ��0			
				}
			}break;
			
					
			case CLIMEB_DOWN_GYRO_MODE://�¸�ģʽ (�����Ƿ���)
			{ 
				chassis.ass_mode_enable = DISABLE;//Ĭ�ϲ�����βģʽ Ҫ���Ļ��ں���ENABLE
				
				if((LAUNCH_TRIGGER && flag_tube_claw_catch) || LAUNCH_RECORD)
				{	
					LAUNCH_RECORD = ENABLE;
	
					if(abs(climb_tube.twisted_angle - 135) < 1)//������ת����λ��
					{
						climb_tube.twisted_angle = 135;                                 //ǿ��������������Ƕ�
						climb_tube.twist_calc_enable = DISABLE;                         //��Ҫ������
						
						ducted.ctrl_mode = DUCTED_ALL_OFF;                              //�غ���
						lifter.ctrl_mode = TUBE_LIFTER_ENTIRE;                         //���̧��
//						lifter.ctrl_mode = TUBE_LIFTER_BIT;                             //���̧��
						chassis.ctrl_mode = CHASSIS_SEPARATE;                          //���̻�еģʽ
						
						if((abs(lifter.target_tube_entire[L] - lifter.total_angle[L]) < 120) || 
							 (abs(lifter.target_tube_entire[R] - lifter.total_angle[R]) < 120))//̧����Ŀ��Ƕ�
						{
              TUBE_CLAW_LOOSE;                                              //צ���ɿ�
							chassis.ctrl_mode = MOVE_FORWARD;                             //������ǰ��
							GIM_SERVO_ZERO;
							if(FORWARD || BACK || LEFT || RIGHT)  
							{
								climb_tube.fiag_launch = 0;              //launchλ ��0			
								climb_tube.ctrl_mode = PLAIN_MODE;	//ǰ�������л�ƽԭģʽ
								chassis.ass_mode_enable = DISABLE;  //��βģʽ�ر�		    
	              climb_tube.location = ON_GROUND;    //�Ѿ���ƽ���Ͽ�		  								
							}
						}
					}
					else 
					{
						critical_value_treatment(imu.yaw, &climb_tube.yaw_record, 0, 360);   //�ٽ�ֵ����	
						climb_tube.twisted_angle = imu.yaw - climb_tube.yaw_record;          //ת���ĽǶ�						

						chassis.ctrl_mode = MOVE_ANTICLOCKWISE;                              //���̻�еģʽ					
						kb.tube_claw_enable = DISABLE;                                
            TUBE_CLAW_CATCH;                                                     //צ��ǿ��ץס
						
						lifter.ctrl_mode = LIFTER_ZERO;		                                   //��ܻر궨��λ��
						ducted.ctrl_mode = DUCTED_ALL_OFF;                                   //ȫ������			
					}					
				}
				else
				{
					kb.tube_claw_enable = ENABLE;                                //צ�ӿɿ�
										
					if(flag_tube_claw_catch)
					{
						chassis.ctrl_mode = CHASSIS_SEPARATE;                      //���̻�еģʽ
					}
					else 
					{
						chassis.ctrl_mode = CHASSIS_FOLLOW;                        //����������ģʽ
					}					
					lifter.ctrl_mode = TUBE_LIFTER_BIT;		                       //���̧��һ��� ̧��Ŀ��֮�󱣳�
					GIM_SERVO_SIDE;
					
					climb_tube.yaw_record = imu.yaw;                             //��ȡ�����ǽǶ���Ϊ��ǰ�Ƕ�
					climb_tube.fiag_launch = 0;                                  //launchλ ��0			
				}				
			}break;


			default: break;
		}
	}
}

void climb_tube_task(void)
{
//	if(lifter.reset)
//	{
		climb_tube_ctrl();
//	}

}

