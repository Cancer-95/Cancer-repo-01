#include "shoot_ctrl.h"

Tri_moto_t Tri_Moto;
//Fri_whee_t Fri_Whee;

/*�����������־λ*/
bool flag_tri_stall;
/*
**˵��:����������ֻд��*ң��������* Ч������ ���������Ż�����Bug�� ��ش���ע�ͺ��*****
**��������ļ���ʵ�ֵ�Ч����
**���䣺
**1.���̶�ת�ܷ�תһ��ʱ��
**2.��תΪ����(���ٶȻ�)һ��ʱ���к��(���Ϊ0)���ʱ��ת���ֶ�תһ�¸о���û������
**3.��㷢�� Ŀ��ֵ�ܼ�ʱ��Ӧ������ �������ж϶�ת��־Ϊ������һ��ʱ���ڵ�̫�������Ŀ��ֵ���� ����������һ�ж�ת��־λ�ͼ�����ǰλ����Ŀ��λ��(��ʱ������۵�̫������˺�Щֵ)��n*������С(nȡ���ֵ)
**4.���������תͣ��λ������ ������������ֶ�����������������λ�� ��ʱ�����ٵ㷢�� ���̵�Ч������λ�û�����ʽת�����뵱ǰλ�������һ�����һ��(���������һ��Ĵ�С������һ������)
**����:
**1.�ٶȻ�ת ͣ��λ���Ǹ���
**2.������ת���򿪻�ת��һ��ʱ������ٶȻ�����ת Ҳ���Ƿ��䲻���� ��Ҳ���Լ�����
*/

void Shoot_Init()
{
	PIDCKZ_param_clear(&pid_trigger_position);
	PIDCKZ_param_clear(&pid_trigger_speed);
	
	PIDCKZ_param_set(&pid_trigger_position, 8500, 1000, 6.5f, 0.0f, 0.0f);            //���̵��λ�û�
	PIDCKZ_param_set(&pid_trigger_speed, 8500, 1000, 15.0f, 0.0f, 0.0f);	            //���̵���ٶȻ�
}

void Tri_Spe_Ctrl(int16_t speed)//For running shoot
{
	Tri_Moto.speed_rpm=Trigger_Motor[TRI_SPEED];                                      //��ǰ�ٶȶ�ȡ
	
	if(!flag_tri_stall) 
	{
		PIDCKZ_Calc(&pid_trigger_speed, Tri_Moto.speed_rpm, speed);    //PID����(�ڿ�����־λ����ʱ��ִ�� ����������ֵ)*****
	}
	
	Block_Bullet_Handle();                                                            //������ʱ��ǿ�в�����仰ǿ�з�תһ��ʱ��
	CAN2_Send(0X1FF,pid_trigger_speed.Out);                                           //�ٶȻ�PID���
}

void Tri_Cas_Ctrl(u32 Target_Angle)//For single shoot
{
  Tri_Data_Handle();                                                                //�ܽǶ�ֵ�������ȡ
	PIDCKZ_Calc(&pid_trigger_position, Tri_Moto.total_angle, Target_Angle);           //�⻷(λ�û�)PID����
	Tri_Spe_Ctrl(pid_trigger_position.Out);                                           //�⻷��������뾭�ٶȻ�PID���
}

/*
ң����ģʽ�£�S2�ϲ����� �����޶��� �ϲ�ͣ��һ��ʱ������
����ģʽ���������̰����� ��������400ms��������
*/
u32 tri_target_angle;//�����target
u16 speed_stop_offset;//�ٶȻ�ͣ��ʱ���ڵ�ƫ��
u16 speed_shoot_count;//�����ٶȻ��ļ���
	static bool flag_speed_shoot;//�����ٶȻ���flag
void Trigger_Remote_Control()
{
	static bool flag_single_shot;//��֤�����flag
	static bool flag_feed_offset;//�Ƿ����λ�ò�����flag

	
	Tri_Data_Handle();//�������ԭ������Tri_Cas_Ctrl()������ǰ����������г���ͷ
	if(RC_Ctl.rc.s1 == RC_SW_DOWN)//����ģʽ
	{
		if(MOUSE_L_PRESSED)//����������
		{
			speed_shoot_count ++;//�����ٶȻ�����
			if(speed_shoot_count > 420)//420ms
			{
				flag_speed_shoot = 1;//�ٶȻ�trigger��1�����ٶȻ�
				speed_shoot_count = 0;//��������
			}	
			
			flag_feed_offset = 0;   //����Ͳ���Ҫ������
			if(flag_single_shot)		//�������Ŀ��ֵ���ۼ�ִֻ��һ��
			{	
				Shooter.Shoot_Time1 = millis();//��¼���µ�ʱ����������ϵͳ��¼��Time2����ó�������ʱ
				tri_target_angle += Bomb_Angle;	//����̶�Targetֵ
				flag_single_shot = 0;//�������Ŀ��ֵ���ۼ�ִֻ��һ��
			}		
		}
		else if(MOUSE_L_NOT_PRESSED)//�ɵ�������
		{
			flag_speed_shoot = 0;//�ٶȻ�trigger����
			speed_shoot_count = 0;//��������
			flag_single_shot = 1;
			if(flag_feed_offset)     //λ�ò���
			{
				tri_target_angle = Tri_Moto.total_angle;//��仰����Ҫ �ŵĵط�Ҳ����Ҫ ��������ģʽ�¼�¼����λ�� ������������if����ģʽ��
				speed_stop_offset = Tri_Moto.total_angle % Bomb_Angle;//��������
				tri_target_angle += (Bomb_Angle - speed_stop_offset);//targetֵ����
				flag_feed_offset = 0;//���ifִֻ��һ��
			}
		}
		if(flag_speed_shoot)//����ģʽ�´���������
		{
	  	flag_feed_offset = 1;         //����ʱ���˻�����Ҫ����
	  	Tri_Spe_Ctrl(3000);           //�����ٶȻ�����
		}
		else Tri_Cas_Ctrl(tri_target_angle);	//��������ִ�д���
	}
	
	else//�Ǽ���ģʽ
	{
		switch(RC_Ctl.rc.s2)
		{
	/*S2�����ϲ�*/
			case RC_SW_UP:
			{
					speed_shoot_count ++;//�����ٶȻ�����
					if(speed_shoot_count > 420)//420ms
					{
						flag_speed_shoot = 1;//�ٶȻ�trigger��1�����ٶȻ�
						speed_shoot_count = 0;//��������
					}				
					flag_feed_offset = 0;  //����Ͳ���Ҫ������

					if(flag_single_shot)		//�������Ŀ��ֵ���ۼ�ִֻ��һ��
					{			
		        if(flag_tri_stall) 
						{
							tri_target_angle -= (tri_target_angle - Tri_Moto.total_angle) / Bomb_Angle * Bomb_Angle;						
						}
						/*������仰��Ϊ�˼�����������ʹĿ��ֵ�����ϵ�ֵ ���������������ۼ��Ǿ仰�ĺ��� Ч�������� ���к��֮���ٲ����˻�ͽ��ܵ������һ�� ���������һ�����һ��******/
						
						Shooter.Shoot_Time1 = millis();//��¼���µ�ʱ����������ϵͳ��¼��Time2����ó�������ʱ
						tri_target_angle += Bomb_Angle;	//����̶�Targetֵ
						flag_single_shot = 0;//�������Ŀ��ֵ���ۼ�ִֻ��һ��	
					}				
					flag_tri_stall = 0;//��ʵ˵�����־λ�����㲻֪��Ϊʲô������*****
			}break;
	/*S2���˻���*/				 
			case RC_SW_MID:
			{
				flag_speed_shoot = 0;//�ٶȻ�trigger����
			  speed_shoot_count = 0;//��������
				flag_single_shot = 1;
				if(flag_feed_offset)     //λ�ò���
				{
					tri_target_angle = Tri_Moto.total_angle;//��仰����Ҫ �ŵĵط�Ҳ����Ҫ ��������ģʽ�¼�¼����λ�� ������������if����ģʽ��
					speed_stop_offset = Tri_Moto.total_angle % Bomb_Angle;//��������
					tri_target_angle += (Bomb_Angle - speed_stop_offset);//targetֵ����
					flag_feed_offset = 0;
				}
			}break;
		
			default:					
			break;//Default do nothing
		}
	/*�����־λ��Ч*/
		if(flag_speed_shoot)
		{
	  	flag_feed_offset = 1;         //����ʱ���˻�����Ҫ����
	  	flag_tri_stall= 0;            //�ú�����ٶȻ��е�PID�����ܹ�ִ��*****
			Tri_Spe_Ctrl(3000);           //�����ٶȻ�����
		}
		else Tri_Cas_Ctrl(tri_target_angle);	//��������ִ�д���

	}
		
}

void Block_Bullet_Handle(void)//��������
{
  static bool stall_flag = 0;
	static uint32_t stall_count = 0;
	static uint32_t stall_inv_count = 0;
			
  if (pid_trigger_speed.Out >= 7500 && abs(Trigger_Motor[TRI_SPEED]) < 20 )
  {
    if (stall_flag == 0) stall_count ++;
  }
  else stall_count = 0;
  
  if (stall_count >= 250)          //0.25s
  {
    stall_flag = 1;
    stall_count = 0;	
		flag_tri_stall = 1;//������־λ�ڶ�תһ��ʱ���ڿ��� ���������ڲ����ϲ�֮ʱ*****�����յ������ӣ�
  }
  
  if (stall_flag == 1)
  {
    stall_inv_count++;
		flag_tri_stall = 1;//������־λ�ڶ�תһ��ʱ���ڿ��� ���������ڲ����ϲ�֮ʱ*****�����յ������ӣ�
    if (stall_inv_count >= 250)   //0.25s
    {
      stall_flag = 0;
      stall_inv_count = 0;
			pid_trigger_speed.Out = 0;
    } 
		else if(flag_speed_shoot) 
		{
			PIDCKZ_Calc(&pid_trigger_speed, Tri_Moto.speed_rpm, -1500);     //����ʱ�ķ�תΪ�ٶȻ�(��Ȼ����������һ���ķ�ת��ʽ)
//			pid_trigger_speed.Out = -1500;
		}
		else pid_trigger_speed.Out = -500;                                //����ʱ�ķ�תΪ����(��Ȼ����������һ���ķ�ת��ʽ)
  }

}


int16_t Open_Angle =  60;//������ǽǶ�
int16_t Close_Angle = 27;//����رսǶ�
int16_t Close_Count = 0;//�رռ�ʱ
int16_t BBox_Open_Flag = 0;
int16_t Reload_Flag = 0;
void Arsenal_Control()//���ֿ��Ͽ���
{
	if((RC_Ctl.rc.s1 == RC_SW_UP && RC_Ctl.rc.s2 == RC_SW_UP && Reload_Flag == 0) || KEY_R_PRESSED)//S1S2���˶������ϲŴ���//����ģʽ���ֺ�ſ�ʼ��ʱ2s��ر�
	{			
		Reload_Flag = 1;//ȷ��Flag�Լ�����ֻ��ֵһ��
		BBox_Open_Flag = 1;
    Close_Count = 0;		
	}
	else if(BBox_Open_Flag)
	{
		/*��������̨ǿ��ˮƽ+�����ָ�*/
		Pitch_Target[MEC] = Pitch_M_MID;
		Pitch_Target[GYO] = Pitch_G_MID;
		Close_Count ++;
			  
		if((abs(Motor_angle[PIT] - Pitch_M_MID) <= 100)||(abs(imu.pit) <= 10)) Servo_Ctrl(Open_Angle);//Լ�������в����������Ǹ���֪��Ϊʲô������ģʽ��̧ͷ��ˮƽ���ܿ�����
			
		if(Close_Count > 2000) BBox_Open_Flag = 0;
	}
	else Servo_Ctrl(Close_Angle);
	
	if(RC_Ctl.rc.s2 != RC_SW_UP) Reload_Flag = 0;
	

//int16_t Servo_Ramp = 2;//���б���¶�
//int16_t Servo_Aim;//���Ŀ��
//int16_t Servo_Real;//���ʵʱ

//if(Servo_Real < Servo_Aim)//б�º��� Cancer��ʾ����֪�����б�º�����������ʲô��
//{
//	Servo_Real += Servo_Ramp;
//	Servo_Real = constrain(Servo_Real, Servo_Real, Servo_Aim);
//}
//else if(Servo_Real > Servo_Aim)
//{
//	Servo_Real -= Servo_Ramp;
//	Servo_Real = constrain(Servo_Real, Servo_Real, Servo_Aim);
//}	
//	Servo_Ctrl(Servo_Real);
}




bool Trigger_ecd_Record = 1;
void Tri_Data_Handle()//�ú������������̵�ʱ��� �Է��ǲ���Ȧ�� ����2ms���������
{
	if(Trigger_ecd_Record)
	{
		Trigger_ecd_Record = 0;
		Tri_Moto.offset_ecd = Trigger_Motor[TRI_ANGLE];  //ֻ��¼һ�γ�ʼƫ��
	}
	
	Tri_Moto.las_ecd = Tri_Moto.cur_ecd;
	Tri_Moto.cur_ecd = Trigger_Motor[TRI_ANGLE];
	
	if(Tri_Moto.cur_ecd - Tri_Moto.las_ecd > 4096)
	{
		Tri_Moto.round_cnt--;
	}
	else if(Tri_Moto.cur_ecd - Tri_Moto.las_ecd < -4096)
	{
		Tri_Moto.round_cnt++;
	}
	
	Tri_Moto.total_ecd = Tri_Moto.round_cnt * 8192 + Tri_Moto.cur_ecd - Tri_Moto.offset_ecd;
  Tri_Moto.total_angle = Tri_Moto.total_ecd / ENCODER_ANGLE_RATIO;//�������� �������Ķ�����ת�ӵ�ת���ܽǶ�
}
 

void Fric_Ctrl(uint16_t pwm_L, uint16_t pwm_R)//Ħ���ֵĿ���
{
	FRIC_L_PWM = 1000 + pwm_L;
	FRIC_R_PWM = 1000 + pwm_R;
}

void Servo_Ctrl( int16_t pwm )//���ֶ���Ŀ���
{
	  pwm = abs(pwm);

	  SERVO_PWM = pwm;
}

int16_t fric_pwm_set=620;
int16_t fric_pwm = 0;
void Shoot_Remote_Ctrl()
{
	Arsenal_Control();
	switch(fric_mode)
	{
		case FRIC_WHEEL_RUN://Ħ���ֿ�
		{	
			if(fric_pwm < fric_pwm_set)//б��PWM����Ħ����
			{
				fric_pwm++;
			}		
			Fric_Ctrl((int16_t)fric_pwm,(int16_t)fric_pwm);
			
			Trigger_Remote_Control();	//���̵Ŀ��� ֻ����Ħ���ֿ���ʱ����У�
		}break;

		case FRIC_WHEEL_OFF://Ħ���ֹ�
		{	
			fric_pwm = 0;//����
			Fric_Ctrl((int16_t)fric_pwm,(int16_t)fric_pwm);
			pid_trigger_position.Out = 0;
			pid_trigger_speed.Out = 0;

		}break;
		
		default://ȫ�����																																															
		{
			fric_pwm = 0;
			Fric_Ctrl((int16_t)fric_pwm,(int16_t)fric_pwm);
			pid_trigger_position.Out = 0;
			pid_trigger_speed.Out = 0;
		}break;
	}
}


////u32 Target_Temp;
//void Tri_Ctrl_Position(u32 Target_Angle)//λ�û�(�⻷)�����Լ����
//{
//	Tri_Data_Handle();
//	
//	/*����һ�����Ե���λ�û����Ʋ��̵��д��г������ ��ʵ֤��û���õķ�����*/
////	if((Target_Temp < Target_Angle)&&(Target_Temp - Tri_Moto.total_angle) <= (13870 / ENCODER_ANGLE_RATIO)) 
////	{
////		Target_Temp += (1387 / ENCODER_ANGLE_RATIO);
////		if(Target_Temp > Target_Angle) Target_Temp = Target_Angle;
////	}	
////	else if((Target_Temp > Target_Angle)&&(Target_Temp - Tri_Moto.total_angle) >= (-13870 / ENCODER_ANGLE_RATIO))
////	{
////		Target_Temp -= (1387 / ENCODER_ANGLE_RATIO);
////		if(Target_Temp < Target_Angle) Target_Temp = Target_Angle;
////	}	
//	
//	PIDCKZ_Calc(&pid_trigger_position, Tri_Moto.total_angle, Target_Angle);
//	Tri_Ctrl_Speed(pid_trigger_position.Out);
//}
//void Tri_Ctrl_Speed(int16_t Target_Speed)//�ٶȻ�(�ڻ�)
//{
//	Tri_Moto.speed_rpm=Trigger_Motor[TRI_SPEED];
//	PIDCKZ_Calc(&pid_trigger_speed, Tri_Moto.speed_rpm, Target_Speed);    //PID����
//	CAN2_Send(0X1FF,pid_trigger_speed.Out);
//}
