#include "gimbal_ctrl.h"

gimbal_t gim;

float  Pitch_Target[2] = {Pitch_M_MID, Pitch_G_MID}, Yaw_Target[2] = {Yaw_M_MID, Yaw_G_MID};

void Gimbal_Init()//Gimbal��PID������ʼ��
{
	PIDCKZ_param_clear(&pid_yaw_M_angle);
	PIDCKZ_param_clear(&pid_yaw_M_speed);
	PIDCKZ_param_clear(&pid_pit_M_angle);
	PIDCKZ_param_clear(&pid_pit_M_speed);
	
	memset(&gim, 0, sizeof(gimbal_t));
	
	PIDCKZ_param_set(&pid_yaw_M_speed, 5000, 3000, 5.0f, 0.0f, 0.0f);        //��еģʽYAW�ڻ������趨
	PIDCKZ_param_set(&pid_pit_M_speed, 5000, 3000, 3.0f, 0.0f, 0.0f);        //��еģʽPIT�ڻ������趨
	PIDCKZ_param_set(&pid_yaw_M_angle, 5000, 1000, 5.0f, 0.0f, 0.0f);	       //��еģʽYAW�⻷�����趨
	PIDCKZ_param_set(&pid_pit_M_angle, 5000, 1000, 6.0f, 0.0f, 0.0f);        //��еģʽPIT�⻷�����趨
	
	PIDCKZ_param_set(&pid_yaw_G_speed, 5000, 3000, 8.0f, 0.0f, 0.0f);        //��еģʽYAW�ڻ������趨
	PIDCKZ_param_set(&pid_pit_G_speed, 5000, 3000, 5.0f, 0.0f, 0.0f);        //��еģʽPIT�ڻ������趨
	PIDCKZ_param_set(&pid_yaw_G_angle, 5000, 1000, 150.0f, 0.0f, 0.0f);	       //��еģʽYAW�⻷�����趨
	PIDCKZ_param_set(&pid_pit_G_angle, 5000, 1000, 150.0f, 0.0f, 0.0f);        //��еģʽPIT�⻷�����趨
	
	}
void Gimbal_Output_Stop()//�����ֹͣ״̬
{
  pid_yaw_M_speed.Out = pid_pit_M_speed.Out = 0;
	pid_yaw_G_speed.Out = pid_pit_G_speed.Out = 0;		
	CAN1_Send(0x1ff,0,0,0,0);               
}
u8 Turn_Around_flag;	
void Gimbal_Turn_Around()//���̰�C��ͷ
{
/*ע�͵��ĵ�ͷ������һ���Լӵ�180�� ������������λ �ʼ����ۼ��ӳ�*/
//	if(KEY_C_PRESSED && Turn_Around_flag)
//	{
//		Yaw_Target[GYO] += 180;
//		Turn_Around_flag = 0;
//	}
//	else if(KEY_C_NOT_PRESSED) Turn_Around_flag = 1;
	if(KEY_C_PRESSED)
	{
    Turn_Around_flag = 1;
	}
	else if(KEY_C_NOT_PRESSED && Turn_Around_flag !=0) 
	{
		Yaw_Target[GYO] += 0.72f;
		Turn_Around_flag++;
		if(Turn_Around_flag >= 250)
		Turn_Around_flag = 0;
	}
}

void Get_MEC_Target()//��ȡP��Y���Ŀ��ֵ
{
	Yaw_Target[MEC] = Yaw_M_MID;//��еģʽ�¸���ֵ���ž�����
	
	if(system_mode == RC_MACHINE_MODE)
	{
		Pitch_Target[MEC] += -(RC_Ctl.rc.ch1-RC_CH_VALUE_OFFSET)*0.01;
	}
	else if(system_mode == KEY_MACHINE_MODE)
	{
		Pitch_Target[MEC] += RC_Ctl.mouse.y * 0.25;	
	}
	
	Pitch_Target[MEC]  = constrain(Pitch_Target[MEC],Pitch_M_MIN,Pitch_M_MAX);

}
void Get_GYO_Target()//��ȡP��Y���Ŀ��ֵ
{	 
	if(system_mode == RC_GYRO_MODE)
	{
		Yaw_Target[GYO]   += -(RC_Ctl.rc.ch0-RC_CH_VALUE_OFFSET)*0.0008;
		Pitch_Target[GYO] += -(RC_Ctl.rc.ch1-RC_CH_VALUE_OFFSET)*0.0005;
	}
	else if(system_mode == KEY_GYRO_MODE || system_mode == KEY_DODGE_MODE)
	{
		Gimbal_Turn_Around();//����ģʽ�µ�ͷ�ж�
	  Yaw_Target[GYO]   += -RC_Ctl.mouse.x * 0.01;
		Pitch_Target[GYO] +=  RC_Ctl.mouse.y * 0.01;
	}

	Pitch_Target[GYO]  = constrain(Pitch_Target[GYO],Pitch_G_MIN,Pitch_G_MAX);	
		
/*YAW��Ŀ��ֵȷ��Խ���ٽ�ֵ*/	
	if(Yaw_Target[GYO] >= 180)Yaw_Target[GYO] = Yaw_Target[GYO]  - 360;
	else if(Yaw_Target[GYO] <= -180)Yaw_Target[GYO] = 360 + Yaw_Target[GYO];
}

void Gimbal_M_Control_Outter()//��еģʽ�⻷
{
	//yaw�����⻷
  gim.yaw_angle_ref=Yaw_Target[MEC];                                            //�⻷(�ǶȻ�)Ŀ��ֵ
	gim.yaw_angle_fdb=Motor_angle[YAW];                                           //�⻷(�ǶȻ�)����ֵ
	PIDCKZ_Calc(&pid_yaw_M_angle, gim.yaw_angle_fdb, gim.yaw_angle_ref);    //PID����õ��⻷���(�Խ��ٶ���ʽ���)	
	//pit�����⻷
	gim.pit_angle_ref=Pitch_Target[MEC];                                          //�⻷(�ǶȻ�)Ŀ��ֵ
	gim.pit_angle_fdb=Motor_angle[PIT];                                           //�⻷(�ǶȻ�)����ֵ
	PIDCKZ_Calc(&pid_pit_M_angle, gim.pit_angle_fdb, gim.pit_angle_ref);    //PID����õ��⻷���(�Խ��ٶ���ʽ���)	
}

void Gimbal_M_Control_Inner()//��еģʽ�ڻ�
{
	//yaw�����ڻ�
	gim.yaw_speed_ref = pid_yaw_M_angle.Out;                                //�⻷�����ΪĿ��ֵ
	gim.yaw_speed_fdb = imu.gz;                                             //��ȡ�����ǵĽ��ٶ�ֵ
	PIDCKZ_Calc(&pid_yaw_M_speed, gim.yaw_speed_ref, gim.yaw_speed_fdb);    //PID����õ��������(ע��˭��˭)
	//pit�����ڻ�
	gim.pit_speed_ref = pid_pit_M_angle.Out;                                //�⻷�����ΪĿ��ֵ
	gim.pit_speed_fdb = imu.gx;                                             //��ȡ�����ǵĽ��ٶ�ֵ
	PIDCKZ_Calc(&pid_pit_M_speed, gim.pit_speed_ref, gim.pit_speed_fdb);    //PID����õ��������(ע��˭��˭)

//	Moving_Average_Filter(pid_yaw_M_speed.Out, 4, &pid_yaw_M_speed.Out);     //����ƽ���˲� ʧ����
//	Moving_Average_Filter(pid_pit_M_speed.Out, 4, &pid_pit_M_speed.Out);     //����ƽ���˲� ʧ����
	CAN1_Send(0x1ff,pid_yaw_M_speed.Out,pid_pit_M_speed.Out,0,0);           //can1���͵���ֵ
}


void Gimbal_G_Control_Outter()//������ģʽ�⻷
{
	//yaw����
  gim.yaw_angle_ref=Yaw_Target[GYO];                                      //�⻷(�ǶȻ�)Ŀ��ֵ
	gim.yaw_angle_fdb=imu.yaw;                                              //��ȡ�����ǽǶ���Ϊ��ǰ�Ƕ�
	Critical_Value_Treatment(gim.yaw_angle_fdb, &gim.yaw_angle_ref,0,360);    //�ٽ�ֵ����
	PIDCKZ_Calc(&pid_yaw_G_angle, gim.yaw_angle_fdb, gim.yaw_angle_ref);    //PID����õ��⻷���(�Խ��ٶ���ʽ���)	
	//pit����
	gim.pit_angle_ref=Pitch_Target[GYO];                                    //�⻷(�ǶȻ�)Ŀ��ֵ
	gim.pit_angle_fdb=imu.pit;                                              //��ȡ�����ǽǶ���Ϊ��ǰ�Ƕ�
	Critical_Value_Treatment(gim.pit_angle_fdb, &gim.pit_angle_ref,0,360);    //�ٽ�ֵ����
	PIDCKZ_Calc(&pid_pit_G_angle, gim.pit_angle_fdb, gim.pit_angle_ref);    //PID����õ��⻷���(�Խ��ٶ���ʽ���)	
}

void Gimbal_G_Control_Inner()//������ģʽ�ڻ�
{
	//yaw����
	gim.yaw_speed_ref = pid_yaw_G_angle.Out;                                //�⻷�����ΪĿ��ֵ
	gim.yaw_speed_fdb = imu.gz;                                             //��ȡ�����ǵĽ��ٶ�ֵ
	PIDCKZ_Calc(&pid_yaw_G_speed, gim.yaw_speed_ref, gim.yaw_speed_fdb);    //PID����õ��������(ע��˭��˭)
	//pit����
	gim.pit_speed_ref = pid_pit_G_angle.Out;                                //�⻷�����ΪĿ��ֵ
	gim.pit_speed_fdb = imu.gx;                                             //��ȡ�����ǵĽ��ٶ�ֵ
	PIDCKZ_Calc(&pid_pit_G_speed, gim.pit_speed_ref, gim.pit_speed_fdb);    //PID����õ��������(ע��˭��˭)

//	Moving_Average_Filter(pid_yaw_G_speed.Out, 4, &pid_yaw_G_speed.Out);     //����ƽ���˲� ʧ����
//	Moving_Average_Filter(pid_pit_G_speed.Out, 4, &pid_pit_G_speed.Out);     //����ƽ���˲� ʧ����
	CAN1_Send(0x1ff,pid_yaw_G_speed.Out,pid_pit_G_speed.Out,0,0);           //can1���͵���ֵ
}
void Gimbal_M_Control()//��еģʽ���⻷�ϲ�
{
  Yaw_Target[GYO] = imu.yaw;      //��¼������ģʽ�����ǽǶ�
	Pitch_Target[GYO] = imu.pit;    //��¼������ģʽ�����ǽǶ�
	Get_MEC_Target();                     //��ȡ��̨Ŀ��ֵ
	Gimbal_M_Control_Outter();            //��еģʽ�⻷
	Gimbal_M_Control_Inner();             //��еģʽ�ڻ�
}
void Gimbal_G_Control()//������ģʽ���⻷�ϲ�
{	
	Yaw_Target[MEC] = Motor_angle[YAW];   //��¼��еģʽ��е��
	Pitch_Target[MEC] = Motor_angle[PIT]; //��¼��еģʽ��е��
	Get_GYO_Target();                     //��ȡ��̨Ŀ��ֵ	
	Gimbal_G_Control_Outter();            //������ģʽ�⻷
	Gimbal_G_Control_Inner();             //������ģʽ�ڻ�
}
void  Gimbal_Control()
{
  gim.yaw_relative_angle = ( Motor_angle[YAW] - Yaw_M_MID ) / ENCODER_ANGLE_RATIO;//��¼ʵʱ����ԽǶ�
	gim.pit_relative_angle = ( Motor_angle[PIT] - Pitch_M_MID ) / ENCODER_ANGLE_RATIO;//��¼ʵʱ����ԽǶ�
	
	if((Motor_angle[YAW] <= Yaw_M_MIN && (RC_Ctl.mouse.x > 0 || RC_Ctl.rc.ch0 > RC_CH_VALUE_OFFSET))	//������̨����̷���Ƕ�
		 ||(Motor_angle[YAW] >= Yaw_M_MAX && (RC_Ctl.mouse.x < 0 || RC_Ctl.rc.ch0 < RC_CH_VALUE_OFFSET)))
	{
		RC_Ctl.mouse.x = 0;
		RC_Ctl.rc.ch0 = RC_CH_VALUE_OFFSET;
	}
	
	switch (gimbal_mode)
  {
		case GIMBAL_STO:												//��̨�����																								
		{
			Gimbal_Output_Stop();
		}break;
		case GIMBAL_MEC:												//��̨��еģʽ																											
		{
			Gimbal_M_Control();
		}break;
		case GIMBAL_GYO:												//��̨������ģʽ																
		{
			Gimbal_G_Control();
		}break;
		default:																																															
		{
			Gimbal_Output_Stop();
		}break;
	}
}




