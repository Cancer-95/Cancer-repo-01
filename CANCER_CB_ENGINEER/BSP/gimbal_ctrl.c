#include "gimbal_ctrl.h"






void Gimbal_G_Control()//������ģʽ���⻷�ϲ�
{	
	Get_GYO_Target();                     //��ȡ��̨Ŀ��ֵ	
	Gimbal_G_Control_Outter();            //������ģʽ�⻷
	Gimbal_G_Control_Inner();             //������ģʽ�ڻ�
}
void  Gimbal_Control()
{
//  chassis.yaw_relative_angle = ( Motor_angle[YAW] - Yaw_M_MID ) / ENCODER_ANGLE_RATIO;//��¼ʵʱ����ԽǶ�

	
	switch (gimbal_mode)
  {

		case GIMBAL_GYO:												//��̨������ģʽ																
		{
			Gimbal_G_Control();
		}break;
		default:break;
	}
}




