#ifndef __SYSTEM_H
#define __SYSTEM_H

#include <stdio.h>
#include "stdbool.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx.h"
#include "stdint.h"

#include "stdlib.h"
#include "stdarg.h"
#include "string.h"
#include "math.h"
#include "ramp.h"

#include "my_math.h"
#include "nvic.h"
#include "can1.h"
#include "can2.h"
#include "roboremo.h"
#include "ano_tc.h"
#include "relay.h"
#include "servo.h"
#include "pidckz.h"
#include "chassis.h"
#include "lifter.h"
#include "bomb_claw.h"
#include "assist_wheel.h"
#include "trailer.h"
#include "dutched.h"
#include "omron.h"
#include "climb_tube.h"
#include "get_bomb.h"
#include "feed_bomb.h"
#include "rescue.h"

#include "modeswitch.h"
#include "sys_check.h"

#include "keyboard.h"
#include "remote_ctrl.h"

#include "mpu6050_iic.h"
#include "mpu6050_driver.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

#include "MPU_Temperature.h"

#include "led.h"
#include "rc.h"

#include "judgement.h"//�����Ѿ�������CRC�����ĺ���

#include "vision.h"

extern volatile uint32_t sysTickUptime;



/*----- �������� -----*/
//#define USING_UART4_JUDGE


uint32_t micros(void);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);

uint32_t millis(void);

void system_init(void);
void loop(void);

extern uint32_t Overflow_check[3];
extern bool MOTORS_ALL_READY;


#endif


