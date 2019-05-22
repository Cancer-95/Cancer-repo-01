#ifndef __MODESWITH_H
#define __MODESWITH_H
#include "system.h"




typedef enum
{
	SAFETY_MODE       = 0,//��ȫģʽ
	RC_MACHINE_MODE   = 1,//ң�ػ�еģʽ
	RC_GYRO_MODE      = 2,//ң��������ģʽ
	KB_MODE           = 3,//����ģʽ(����ֻ��������ģʽ)

} SYSTEM_MODE;//ϵͳģʽ






extern SYSTEM_MODE system_mode;


void mode_switch_task(void);


#endif 

