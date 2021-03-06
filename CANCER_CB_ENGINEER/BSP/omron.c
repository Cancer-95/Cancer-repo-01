#include "omron.h"

#define DETECT_PERIOD       3     //放在哪个时间戳
#define CONFIRM_TIME        100  //ms
#define SCAN_PERIOD         1     //放在哪个时间戳
#define SCAN_CONFIRM_TIME   50  //ms

optic_switch_t optic_switch;

void optoelectronic_switch_init(void)
{
  GPIO_InitTypeDef gpio;	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | 
	                       RCC_AHB1Periph_GPIOB, 
	                       ENABLE);
	
	gpio.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	gpio.GPIO_Speed = GPIO_Speed_50MHz;	
	gpio.GPIO_Mode = GPIO_Mode_IN;//输入	

	gpio.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOC,&gpio);	
	
	gpio.GPIO_Pin = GPIO_Pin_1 ;
	GPIO_Init(GPIOB,&gpio);		
	
  memset(&optic_switch, 0, sizeof(optic_switch_t));
	optic_switch.value_record = ENABLE;
	optic_switch.confirm_time = CONFIRM_TIME / DETECT_PERIOD;
	optic_switch.scan_confirm_time = SCAN_CONFIRM_TIME / SCAN_PERIOD;

}

void optic_scan_task(void)
{
//	if(optic_switch.scan_enable)
	{	/*爪子上的那一个 两个方向都得消抖*/
		optic_switch.read[M] = GPIO_ReadInputDataBit(OPTIC_SWITCH_M_GPIO,OPTIC_SWITCH_M_GPIO_PIN);
	
		if(optic_switch.read[M])
		{
//			((millis() - optic_switch.react_time_A[M]) > optic_switch.scan_confirm_time ) ? (optic_switch.value[M] = 1) : (optic_switch.value[M] = 0);
			if((millis() - optic_switch.react_time_A[M]) > optic_switch.scan_confirm_time)
			{
				optic_switch.value[M] = 1;
//				optic_switch.value_last[M] = 0;
			}
			else optic_switch.value[M] = 0;
			
			optic_switch.react_time_B[M] = millis();
		}
		else
		{
//			((millis() - optic_switch.react_time_B[M]) > optic_switch.scan_confirm_time ) ? (optic_switch.value[M] = 0) : (optic_switch.value[M] = 1);
			if((millis() - optic_switch.react_time_B[M]) > optic_switch.scan_confirm_time)
			{
				optic_switch.value[M] = 0;
//				optic_switch.value_last[M] = 1;
			}
			else optic_switch.value[M] = 1;			
			
			optic_switch.react_time_A[M] = millis();
		}
	}
//	else optic_switch.value[M] = 1;

}

void optoelectronic_switch_task(void)
{
	switch(optic_switch.ctrl_mode)
	{
		case DETECT_ENABLE:
		{
			/*底盘上的两个*/
			optic_switch.read[S] = GPIO_ReadInputDataBit(OPTIC_SWITCH_S_GPIO,OPTIC_SWITCH_S_GPIO_PIN);
//			optic_switch.read[R] = GPIO_ReadInputDataBit(OPTIC_SWITCH_R_GPIO,OPTIC_SWITCH_R_GPIO_PIN);
			
			if(optic_switch.read[S])
			{
				((millis() - optic_switch.react_time_A[S]) > optic_switch.confirm_time ) ? (optic_switch.value[S] = 1) : (optic_switch.value[S] = 0);
			}
			else
			{
				optic_switch.react_time_A[S] = millis();
				optic_switch.value[S] = 0;
			}
			
//			if(optic_switch.read[R])
//			{
//				((millis() - optic_switch.react_time_A[R]) > optic_switch.confirm_time ) ? (optic_switch.value[R] = 1) : (optic_switch.value[R] = 0);
//			}
//			else
//			{
//				optic_switch.react_time_A[R] = millis();
//				optic_switch.value[R] = 0;
//			}
			

		}break;
		
		case FORCE_ONE:
		{
			optic_switch.value[S] = 1;
//			optic_switch.value[R] = 1;	
		}break;
		
		case FORCE_ZERO:
		{
			optic_switch.value[S] = 0;
//			optic_switch.value[R] = 0;	
		}break;
		
		default:break;	
	}
	
}

