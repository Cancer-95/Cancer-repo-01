#include "gpio.h"

/*GPIO normal configration*/
void GPIO_config(GPIO_TypeDef*     GPIOx,//�˿�
	               u32               Pin,  //����
                 GPIOMode_TypeDef  Mode, //ģʽ
                 GPIOOType_TypeDef OType,//�����ʽ
                 GPIOPuPd_TypeDef  PuPd) //������
{
	GPIO_InitTypeDef  gpio;
	RCC->AHB1ENR |= (1 << ((u32)GPIOx - GPIOA_BASE) / (GPIOB_BASE - GPIOA_BASE));		//ʹ��ʱ��
	gpio.GPIO_Pin   = Pin;
	gpio.GPIO_Mode  = Mode;
  gpio.GPIO_OType = OType;
  gpio.GPIO_Speed = GPIO_Fast_Speed;
  gpio.GPIO_PuPd  = PuPd;
	GPIO_Init(GPIOx, &gpio);
}
/*GPIO AF configration*/
void GPIO_AFconfig(GPIO_TypeDef*     GPIOx,//�˿�
	                 u32               Pin,  //����
                   GPIOOType_TypeDef OType,//�����ʽ
                   GPIOSpeed_TypeDef Speed,//�ٶ�
                   GPIOPuPd_TypeDef  PuPd, //������
                   u8                AFto) //���ó�ʲô
{
	u32 pinpos;
	GPIO_InitTypeDef          gpio;
	
	RCC->AHB1ENR |= (1 << ((u32)GPIOx - GPIOA_BASE) / (GPIOB_BASE - GPIOA_BASE));

	for(pinpos = 0; pinpos < 0x10; pinpos++)
	{
		if((Pin & (1 << pinpos)) == 0)
			continue;
		GPIOx->AFR[pinpos >> 0x03] = (GPIOx->AFR[pinpos >> 0x03] & ~(0x0F << (4 * (pinpos & 0x07)))) | (AFto << (4 * (pinpos & 0x07)));
	}
	gpio.GPIO_Pin   = Pin;
	gpio.GPIO_Mode  = GPIO_Mode_AF;
	gpio.GPIO_Speed = Speed;
	gpio.GPIO_OType = OType;
	gpio.GPIO_PuPd  = PuPd;
	GPIO_Init(GPIOx, &gpio);
}

void led_GPIO_init(void)
{
	GPIO_config(LED_ONBOARD_GPIO_Port,                                       //port
							LED_BLUE_Pin | LED_ORANGE_Pin | LED_GREEN_Pin | LED_RED_Pin, //pin
							GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL);             //else
		
	GPIO_config(LED_CAMERA_GPIO_Port,                                        //port
							LED_CAMERA_Pin,                                              //pin
							GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL);             //else
}

void relay_GPIO_init(void)
{
	/*tube_claw catch & loose*/
	GPIO_config(TUBE_CLAW_CL_GPIO_Port,                          //port
							TUBE_CLAW_CL_Pin,                                //pin
							GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL); //else              
	GPIO_config(TUBE_CLAW_CL_BACKUP_GPIO_Port,                   //port 
							TUBE_CLAW_CL_BACKUP_Pin,                         //pin
							GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL); //else
  /*bomb_claw catch & loose*/
	GPIO_config(BOMB_CLAW_CL_GPIO_Port,                          //port 
							BOMB_CLAW_CL_Pin,                                //pin
							GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL); //else
	GPIO_config(BOMB_CLAW_CL_BACKUP_GPIO_Port,                   //port 
							BOMB_CLAW_CL_BACKUP_Pin,                         //pin
							GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL); //else
  /*bomb_eject push & pull*/
	GPIO_config(BOMB_EJECT_PP_GPIO_Port,                         //port 
							BOMB_EJECT_PP_Pin,                               //pin
							GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL); //else
	GPIO_config(BOMB_EJECT_PP_BACKUP_GPIO_Port,                  //port 
							BOMB_EJECT_PP_BACKUP_Pin,                        //pin
							GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL); //else
  /*bomb_claw push & pull*/
	GPIO_config(BOMB_CLAW_PP_GPIO_Port,                          //port
							BOMB_CLAW_PP_Pin,                                //pin
							GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL); //else
	GPIO_config(BOMB_CLAW_PP_BACKUP_GPIO_Port,                   //port
							BOMB_CLAW_PP_BACKUP_Pin,                         //pin
							GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL); //else
	/*trailer_hook push & pull*/
	GPIO_config(TRAILER_HOOK_PP_GPIO_Port,                       //port
							TRAILER_HOOK_PP_Pin,                             //pin
							GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL); //else
	GPIO_config(TRAILER_HOOK_PP_BACKUP_GPIO_Port,                //port
							TRAILER_HOOK_PP_BACKUP_Pin,                      //pin
							GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL); //else
	/*CCTV_switch */
	GPIO_config(CCTV_SWITCH_GPIO_Port,                           //port 
							CCTV_SWITCH_Pin,                                 //pin
							GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL); //else
	GPIO_config(CCTV_SWITCH_BACKUP_GPIO_Port,                    //port
							CCTV_SWITCH_BACKUP_Pin,                          //pin
							GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL); //else
	/*CCTV_switch the third eye*/
	GPIO_config(THE_THIRD_EYE_GPIO_Port,                         //port
							THE_THIRD_EYE_Pin,                               //pin
							GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL); //else
	GPIO_config(THE_THIRD_EYE_BACKUP_GPIO_Port,                  //port
							THE_THIRD_EYE_BACKUP_Pin,                        //pin
							GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL); //else
	/*magazine open & close*/
	GPIO_config(MAGAZINE_OC_GPIO_Port,                           //port
							MAGAZINE_OC_Pin,                                 //pin
							GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL); //else
	GPIO_config(MAGAZINE_OC_BACKUP_GPIO_Port,                    //port
							MAGAZINE_OC_BACKUP_Pin,                          //pin
							GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL); //else
}
void optic_switch_GPIO_init(void)
{
	GPIO_config(OPTIC_SWITCH_S_GPIO_Port,                       //port
							OPTIC_SWITCH_S_Pin,                             //pin
							GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);     //else
	GPIO_config(OPTIC_SWITCH_M_GPIO_Port,                       //port
							OPTIC_SWITCH_M_Pin,                             //pin
							GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);     //else
}

void CAN_GPIO_init(void)
{
  GPIO_AFconfig(CAN1_GPIO_Port,                                                //port
	              CAN1_TX_Pin | CAN1_RX_Pin,                                     //pin
	              GPIO_OType_PP, GPIO_High_Speed, GPIO_PuPd_UP, GPIO_AF_CAN1);   //else
	GPIO_AFconfig(CAN2_GPIO_Port,                                                //port
	              CAN2_TX_Pin | CAN2_RX_Pin,                                     //pin
	              GPIO_OType_PP, GPIO_High_Speed, GPIO_PuPd_UP, GPIO_AF_CAN2);   //else
}

void servo_GPIO_init(void)
{
	GPIO_AFconfig(GIM_SERVO_GPIO_Port,
	              GIM_SERVO_Pin,
	              GPIO_OType_PP, GPIO_High_Speed, GPIO_PuPd_UP, GPIO_AF_TIM4);
}

void ducted_GPIO_init(void)
{
	GPIO_AFconfig(DUCTED_GPIO_Port,
	              DUCTED_R_Pin | DUCTED_L_Pin,
	              GPIO_OType_PP, GPIO_High_Speed, GPIO_PuPd_UP, GPIO_AF_TIM4);
}

void RC_GPIO_init(void)//usart2
{
	GPIO_AFconfig(RC_GPIO_Port,
	              RC_Pin,
	              GPIO_OType_PP, GPIO_Low_Speed, GPIO_PuPd_UP, GPIO_AF_USART2);
}

void vision_GPIO_init(void)//uart4
{
  GPIO_AFconfig(VISION_GPIO_Port,
	              VISION_Pin,
	              GPIO_OType_PP, GPIO_Fast_Speed, GPIO_PuPd_UP, GPIO_AF_UART4);
}

void judgement_GPIO_init(void)//uart5
{
	GPIO_AFconfig(JUDGEMENT_TX_GPIO_Port,
	              JUDGEMENT_TX_Pin,
	              GPIO_OType_PP, GPIO_Fast_Speed, GPIO_PuPd_UP, GPIO_AF_UART5);
	GPIO_AFconfig(JUDGEMENT_RX_GPIO_Port,
	              JUDGEMENT_RX_Pin,
	              GPIO_OType_PP, GPIO_Fast_Speed, GPIO_PuPd_UP, GPIO_AF_UART5);
}

void GPIO_init(void)
{
	/*IO*/
	led_GPIO_init();
	relay_GPIO_init();
	optic_switch_GPIO_init();
	/*CAN*/
	CAN_GPIO_init();
	/*TIM*/
	servo_GPIO_init();
	ducted_GPIO_init();
	/*UART*/
	RC_GPIO_init();
	vision_GPIO_init();
	judgement_GPIO_init();
}
