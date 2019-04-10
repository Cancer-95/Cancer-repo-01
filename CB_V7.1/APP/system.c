#include "system.h"

//��������ʼ��
static volatile uint32_t usTicks = 0;
static void cycleCounterInit(void)
{
	RCC_ClocksTypeDef clocks;
	RCC_GetClocksFreq(&clocks);
	usTicks = clocks.SYSCLK_Frequency / 1000000; 
}

//��΢��Ϊ��λ����ϵͳʱ��
uint32_t micros(void)
{
	register uint32_t ms, cycle_cnt;
	do {
			ms = sysTickUptime;
			cycle_cnt = SysTick->VAL;
	} while (ms != sysTickUptime);
	return (ms * 1000) + (usTicks * 1000 - cycle_cnt) / usTicks;
}

//΢�뼶��ʱ
void delay_us(uint32_t us)
{
	uint32_t now = micros();
	while (micros() - now < us);
}

//���뼶��ʱ
void delay_ms(uint32_t ms)
{
	while (ms--)
			delay_us(1000);
}

//�Ժ���Ϊ��λ����ϵͳʱ��
uint32_t millis(void)
{
	return sysTickUptime;
}

/*--------------------------------------ϵͳ��ʼ��--------------------------------------------*/
void system_init(void)
{	
	cycleCounterInit();
	SysTick_Config(SystemCoreClock / 1000);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);						//����ϵͳ�ж����ȼ�����2 ��4��Pre 4��Sub��
	
	judgement_init();    			//����ϵͳ uart5��ʼ��  p.s. system.h�п�ע�ͺ궨�壬�л���uart5
	CRC_init();               //CRCУ���ʼ��	
	
	vision_init();             //�Ӿ��ӿ�uart4
	
/*ִ��Ԫ����ʼ��1*/  	
	RC_init();
	led_init();
  servo_init();
	relay_init();//�̵���
	ducted_init();//���� 
	
//	ANO_TC_Init();
//	Roboremo_Init();

/**���������ʱ����Ҫ ��û����������Ǿͻ�������� ��Ϊ���ﲢû�е���MPU_Init() ����������ֱ��mpu_dmp_init()����dmp��ʼ������ 
	*��Ϊ����Init�ж���IIC�ĳ�ʼ�� MPU_Init()���кܶ๤���ظ� ����mpu_dmp_init()���˺ܹؼ���100ms��ʱ
	*��Ϊһ�㴫����оƬ������STM32�ڲ�����ADC ADC���ϵ絽�ȶ���Ҫʱ��
	*/
	delay_ms(1000);
/*ϵͳ����ʼ��*/
	sys_check_init();
    
/*ִ��Ԫ����ʼ��2*/ 
	chassis_param_init();//���̵��
  lifter_param_init();//̧�����	
	assist_wheel_param_init();//����Ħ���ֵ��
	trailer_param_init();//�ϳ����
	optoelectronic_switch_init();//��翪��
	bomb_claw_param_init();//ȡ�������������
/*���������ʼ��*/	
  global_task_init();//�������������ʼ��
//  vision_param_init();//�Ӿ���ʼ��
	climb_tube_param_init();//���˳�ʼ��
  get_bomb_param_init();//ȡ����ʼ��	
	rescue_param_init();//��Ԯ��ʼ��
	feed_bomb_param_init();//������ʼ��
	
	CAN1_init();	
	CAN2_init();
	
	MPU_Init();
	MPU_tempPID_init();
	imu_param_init();
	
	while(mpu_dmp_init())
	{
		static u8 fail_time = 0;
		fail_time++;
		delay_ms(100);
		if(fail_time>3)
		{
			mpu_set_dmp_state(1);	
			break;
		}
	}
	mpu_offset_cal();//���ڼ��������ǹ���OFFECT ��MPU_Get_Gyroscope()�����м�������ƫ���ע�͵�����
	delay_ms(1000);//�����ʱ����Ϲ��

	
}

/*--------------------------------------��ѭ��--------------------------------------------*/
uint32_t Overflow_check[3]={0};
bool MOTORS_ALL_READY = 0;
void loop(void)
{	
	static uint32_t currentTime = 0;    
	static uint32_t loopTime_1ms = 0;   //ģʽ�����䡢����
  static uint32_t loopTime_3ms = 0;   //��������̨
	static uint32_t loopTime_5ms = 0;   //���ԷŸ������߰���Ĳ���
  static uint32_t loopTime_20ms = 0;  //���ر���
	static uint32_t loopTime_30ms = 0;  //������������
	static uint32_t loopTime_50ms = 0;  //����Roboremo
	static uint32_t loopTime_100ms = 0; //�������ݸ�����ϵͳ
	static uint32_t loopTime_200ms = 0; //LEDָʾ

//	static u8 printf_delay = 0; //roboremo��printf����
	
	currentTime = micros();	//��ȡ��ǰϵͳʱ��
		
	if((int32_t)(currentTime - loopTime_1ms) >= 0)  
	{	
		loopTime_1ms = currentTime + 1000;	//1ms		
		Overflow_check[0] +=1;
		
		chassis_task();  
    lifter_task();
//		assist_wheel_task();
//		servo_task();

		/*CAN1���͵���*/	
	  CAN1_send(0x200, chassis.current[M1], chassis.current[M2], chassis.current[M3], chassis.current[M4]);
		CAN1_send(0X1FF, lifter.current[L], lifter.current[R], 0, 0);
//		/*CAN2���͵���*/
//		CAN2_send(0X200, bomb_claw.current[Y], trailer.current, 0, 0);  
//		CAN2_send(0X1FF, bomb_claw.current[L], bomb_claw.current[R], 0, 0);
		optic_scan_task();		
    
	}
	
	if((int32_t)(currentTime - loopTime_3ms) >= 0)  
	{			
		loopTime_3ms = currentTime + 3000;	//3ms	
    Overflow_check[1] +=3;
		
		trailer_task();
		bomb_claw_task();	
//		CAN2_send(0X200, 0, trailer.current, 0, bomb_claw.current[L]);      
//	  CAN2_send(0X1FF, bomb_claw.current[R], 0, bomb_claw.current[Y], 0);
		CAN2_send(0X200, bomb_claw.current[L], bomb_claw.current[R], bomb_claw.current[Y], trailer.current);      
		
		
		
		vision_task();
		
		mode_switch_task(); //ģʽѡ��			
		mpu_get_data();		
    
		optoelectronic_switch_task();	
	  CCTV_switch_task();
		
		if(MOTORS_ALL_READY)
		{
			climb_tube_task();
			get_bomb_task();		
			rescue_task();
			feed_bomb_task();				
		}
    

	}
		
	if((int32_t)(currentTime - loopTime_5ms) >= 0)  
	{			
		loopTime_5ms = currentTime + 5000;	//5ms	
	  Overflow_check[2] +=5;
	
		ducted_task();
//		ANO_TC_Report16(PowerProtect.Judge_power*100);//Test-����������ʵʱ����		
//		ANO_TC_Report(0,0,0,imu.gx,imu.gy,imu.gz,imu.roll,imu.pit,imu.yaw);//Test-������������������
	}
		
	if((int32_t)(currentTime - loopTime_20ms) >= 0)  
	{			
		loopTime_20ms = currentTime + 20000;	//20ms	
				
		RC_offline_handler();//���ر���
	}
	
	if((int32_t)(currentTime - loopTime_30ms) >= 0)  
	{			
		loopTime_30ms = currentTime + 30000;	//30ms	

		vision_send_task();//���Ӿ�������
	}
	
	if((int32_t)(currentTime - loopTime_50ms) >= 0)  
	{			
    loopTime_50ms = currentTime + 50000;    //50ms ���ʱ����Ǹ�roboremo_printf׼��
	 
    Tempeture_PID();
//		imu_temp_keep();//Cancer�Լ�д����Ʈ�Ƚ��ѵ�		
		sys_check();//ϵͳ���
		/*
	 	*����ȷ�ϲ����޸ĳɹ�
		*ִ������printf������ܲ���̫��
		*/
//		printf_delay ++;
//		switch(printf_delay)
//		{
//			case 1: Roboremo_printf("IN_p %.3f\n",roboremo_pid.Inner_p);   break;        //pid�趨��������roboremo
//			case 2: Roboremo_printf("IN_i %.3f\n",roboremo_pid.Inner_i);   break;
//			case 3: Roboremo_printf("IN_d %.3f\n",roboremo_pid.Inner_d);   break;
//			case 4: Roboremo_printf("OUT_p %.3f\n",roboremo_pid.Outter_p); break;
//			case 5: Roboremo_printf("OUT_i %.3f\n",roboremo_pid.Outter_i); break;
//			case 6: Roboremo_printf("OUT_d %.3f\n",roboremo_pid.Outter_d); break;
//			
//			case 7: Roboremo_printf("TT_L %.1f\n",roboremo_power.Total_Limit); break;     //�����趨��������roboremo
//			case 8: Roboremo_printf("PW_T %.1f\n",roboremo_power.Power_Threshold); break;
//			
//			default:printf_delay = 0; break;
//		}
	}
	
	if((int32_t)(currentTime - loopTime_100ms) >= 0)  
	{			
		loopTime_100ms = currentTime + 100000;	//100ms		

		judgement_send_task();//������ϵͳ������
	}
	
	if((int32_t)(currentTime - loopTime_200ms) >= 0)  
	{			
		loopTime_200ms = currentTime + 200000;	//200ms	
		
		/*����LEDָʾ*/
    led_task();
	}

}








