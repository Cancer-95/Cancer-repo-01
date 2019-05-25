#include "mpu6050_temp.h"

void mpu6050_temp_init()
{
		GPIO_InitTypeDef gpio;	
		TIM_TimeBaseInitTypeDef   tim;
		TIM_OCInitTypeDef         oc;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
	
		gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
		gpio.GPIO_Speed = GPIO_Speed_100MHz;	
		gpio.GPIO_Mode = GPIO_Mode_AF;										
		gpio.GPIO_Pin = GPIO_Pin_8 ;  		

		GPIO_Init(GPIOA, &gpio);	
	
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource8, GPIO_AF_TIM1); 
		

		tim.TIM_Prescaler = 84-1;
		tim.TIM_CounterMode = TIM_CounterMode_Up;		//���ϼ���
		tim.TIM_Period = 1000;   //2ms	��������
		tim.TIM_ClockDivision = TIM_CKD_DIV1;		//����ʱ�ӷָ��Ϊ1�Ļ����2
		TIM_TimeBaseInit(TIM1,&tim);
		
		oc.TIM_OCMode = TIM_OCMode_PWM2;		//ѡ��ʱ��ģʽ
		oc.TIM_OutputState = TIM_OutputState_Enable;		//ѡ������Ƚ�״̬
		oc.TIM_OutputNState = TIM_OutputState_Disable;	//ѡ�񻥲�����Ƚ�״̬
		oc.TIM_Pulse = 0;		//���ô�װ�벶��Ƚ���������ֵ
		oc.TIM_OCPolarity = TIM_OCPolarity_Low;		//�����������
		oc.TIM_OCNPolarity = TIM_OCPolarity_High;		//���û����������
		oc.TIM_OCIdleState = TIM_OCIdleState_Reset;		//ѡ�����״̬�µķǹ���״̬
		oc.TIM_OCNIdleState = TIM_OCIdleState_Set;		//ѡ�񻥲�����״̬�µķǹ���״̬
		TIM_OC1Init(TIM1,&oc);		//ͨ��1
		TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
		
					 
		TIM_ARRPreloadConfig(TIM1,ENABLE);
		TIM_CtrlPWMOutputs(TIM1,ENABLE);
		
		TIM_Cmd(TIM1,ENABLE);
		
}


/*---------------------------------Cancer�Լ�д�ıȽ��ѵ�-------------------------------------*/
void imu_param_init(void)
{
	pidckz_reset(&pid_imu_tmp);
	pidckz_param_set(&pid_imu_tmp, 1000, 50, 8.0f, 0.08f, 80.0f);      
}

void imu_temp_keep(void)
{
  imu.temp_ref = 3500;
  pidckz_calc(&pid_imu_tmp, imu.temp, imu.temp_ref);
	TIM1->CCR1 = constrain(pid_imu_tmp.out, 0, +1000); 
}

void mpu_get_data(void)
{
	MPU_Get_Gyroscope(&imu.gx,&imu.gy,&imu.gz);       	//��ȡ���ٶ�
	mpu_dmp_get_data(&imu.roll,&imu.pit,&imu.yaw); 	    //��ȡŷ����
	imu.temp = MPU_Get_Temperature();	                  //��ȡ�¶�
	Temperature_Value = MPU_Get_Temperature();	        //��ȡ�¶�
}

void imu_task(void)//��Ҫʱ�������Բ𿪷��ڲ�ͬ��ʱ���
{
  mpu_get_data();	
  imu_temp_keep();
}

/*---------------------------------�����õ�һ��-------------------------------------*/
short Temperature_Value;
float TEMP_Error,TEMP_Last_Error,TEMP_Pterm,TEMP_Iterm,TEMP_Dterm,TEMP_PIDterm;
float TEMP_P=12.0f,TEMP_I=1.5f,TEMP_D=5.0f;
void mpu6050_temp_PID()
{	
	TEMP_Error 	 = 	3463 - Temperature_Value;
	TEMP_Pterm 	 = 	TEMP_Error * TEMP_P;
	
	TEMP_Iterm	 += TEMP_Error * TEMP_I * 0.02f;
	TEMP_Iterm	 = 	constrain_int32(TEMP_Iterm,-100,+100);
	
	TEMP_Dterm	 =	(TEMP_Error - TEMP_Last_Error) * TEMP_D/0.02f;
	TEMP_PIDterm =	TEMP_Pterm+ TEMP_Iterm + TEMP_Dterm;

	TEMP_PIDterm =	constrain_int32(TEMP_PIDterm,0,+1000);  
	
	TEMP_Last_Error=	TEMP_Error;
	TIM1->CCR1  = TEMP_PIDterm ;
}
