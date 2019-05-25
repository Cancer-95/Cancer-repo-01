#include "system.h"

static volatile uint32_t usTicks = 0;

uint32_t currentTime = 0;

uint32_t previousTime = 0;
uint16_t cycleTime = 0; 

//�޷�
float constrain(float amt, float low, float high)
{
	if (amt < low)
		return low;
	else if (amt > high)
		return high;
	else
		return amt;
}
int32_t constrain_int32(int32_t amt, int32_t low, int32_t high)
{
	if (amt < low)
		return low;
	else if (amt > high)
		return high;
	else
		return amt;
}

int16_t constrain_int16(int16_t amt, int16_t low, int16_t high)
{
	if (amt < low)
		return low;
	else if (amt > high)
		return high;
	else
		return amt;
}

int constrain_int(int amt,int low,int high)
{
	if (amt < low)
		return low;
	else if (amt > high)
		return high;
	else
		return amt;
}

//��������ʼ��
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

//ϵͳ��ʼ��
void System_Init(void)
{	
	cycleCounterInit();
	SysTick_Config(SystemCoreClock / 1000);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);						//����ϵͳ�ж����ȼ�����2 ��4��Pre 4��Sub��

	Judgement_Init();    			//����ϵͳ uart4��ʼ��  p.s. system.h�п�ע�ͺ궨�壬�л���uart5
	CRC_init();               //CRCУ���ʼ��	
	
	RC_Init();
	Led_Init();
	
	PWM_Init();
	CAN1_Init();
	CAN2_Init();
	ANO_TC_Init();
	Roboremo_Init();

	delay_ms(200);
/**���������ʱ����Ҫ ��û����������Ǿͻ�������� ��Ϊ���ﲢû�е���MPU_Init() ����������ֱ��mpu_dmp_init()����dmp��ʼ������ 
	*��Ϊ����Init�ж���IIC�ĳ�ʼ�� MPU_Init()���кܶ๤���ظ� ����mpu_dmp_init()���˺ܹؼ���100ms��ʱ
	*��Ϊһ�㴫����оƬ������STM32�ڲ�����ADC ADC���ϵ絽�ȶ���Ҫʱ��
	*/
  Shoot_Init();
	Chassis_Init();	
	Gimbal_Init();

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
//	mpu_offset_cal();     //���ڼ��������ǹ���OFFECT ��MPU_Get_Gyroscope()�����м�������ƫ���ע�͵�����
	delay_ms(1500);//�����ʱ����Ϲ��
}

//��ѭ��
void Loop(void)
{	
	static uint32_t currentTime = 0;    
	static uint32_t loopTime_1ms = 0;   //ģʽ�����䡢����
  static uint32_t loopTime_2ms = 0;   //��������̨
	static uint32_t loopTime_5ms = 0;   //���ԷŸ������߰���Ĳ���
  static uint32_t loopTime_20ms = 0;  //���ر���
	static uint32_t loopTime_50ms = 0;  //����Roboremo
	static uint32_t loopTime_100ms = 0; //�������ݸ�����ϵͳ

	static u8 printf_delay = 0; //roboremo��printf����
	
	currentTime = micros();	//��ȡ��ǰϵͳʱ��
	
		
	if((int32_t)(currentTime - loopTime_1ms) >= 0)  
	{	
		loopTime_1ms = currentTime + 1000;	//1ms		
		
		Ctrl_Mode_Select(); //ģʽѡ��		
		Shoot_Remote_Ctrl();//�������
		Chassis_Control();  //���̿���
		

	}
	
	if((int32_t)(currentTime - loopTime_2ms) >= 0)  
	{			
		loopTime_2ms = currentTime + 2000;	//2ms	
					
		MPU_Get_Gyroscope(&imu.gx,&imu.gy,&imu.gz);       	//��ȡ���ٶ�			
		mpu_dmp_get_data(&imu.roll,&imu.pit,&imu.yaw); 	    //��ȡŷ����
    Gimbal_Control();//��̨����
		}
		
	if((int32_t)(currentTime - loopTime_5ms) >= 0)  
	{			
		loopTime_5ms = currentTime + 5000;	//5ms	
		
//		ANO_TC_Report16(PowerProtect.Judge_power*100);//Test-����������ʵʱ����		
//		ANO_TC_Report(0,0,0,imu.gx,imu.gy,imu.gz,imu.roll,imu.pit,imu.yaw);//Test-������������������
	}
		
	if((int32_t)(currentTime - loopTime_20ms) >= 0)  
	{			
		loopTime_20ms = currentTime + 20000;	//20ms	
		
		RC_Offline_Handle();//���ر���
	}
	
	if((int32_t)(currentTime - loopTime_50ms) >= 0)  
	{			
    loopTime_50ms = currentTime + 50000;    //50ms ���ʱ����Ǹ�roboremo_printf׼��
		
		/*
	 	*����ȷ�ϲ����޸ĳɹ�
		*ִ������printf������ܲ���̫��
		*/
		printf_delay ++;
		switch(printf_delay)
		{
			case 1: Roboremo_printf("IN_p %.3f\n",roboremo_pid.Inner_p);   break;        //pid�趨��������roboremo
			case 2: Roboremo_printf("IN_i %.3f\n",roboremo_pid.Inner_i);   break;
			case 3: Roboremo_printf("IN_d %.3f\n",roboremo_pid.Inner_d);   break;
			case 4: Roboremo_printf("OUT_p %.3f\n",roboremo_pid.Outter_p); break;
			case 5: Roboremo_printf("OUT_i %.3f\n",roboremo_pid.Outter_i); break;
			case 6: Roboremo_printf("OUT_d %.3f\n",roboremo_pid.Outter_d); break;
			
			case 7: Roboremo_printf("TT_L %.1f\n",roboremo_power.Total_Limit); break;     //�����趨��������roboremo
			case 8: Roboremo_printf("PW_T %.1f\n",roboremo_power.Power_Threshold); break;
			
			default:printf_delay = 0; break;
		}
	}
	
	if((int32_t)(currentTime - loopTime_100ms) >= 0)  
	{			
		loopTime_100ms = currentTime + 100000;	//100ms	
		
		Send_Data_To_Judgement();//�����Զ������ݲ���ϵͳ
	}
	

}








