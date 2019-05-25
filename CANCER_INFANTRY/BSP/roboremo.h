#ifndef __ROBOREMO_H
#define __ROBOREMO_H	 
#include "system.h"

#define U3_TXMAX_len 128
#define U3_RXMAX_len 128

typedef struct {
	float Inner_p, Inner_i, Inner_d;//����Roboremo���ڻ����������֡�΢��
	float Outter_p, Outter_i, Outter_d;//����Roboremo���⻷���������֡�΢��
	
	}Roboremo_PID_t;

typedef struct {
	float Total_Limit, Power_Threshold;//����Roboremo��
	
	}Roboremo_Power_t;	

void Roboremo_Init(void);
void exeCmd(void);
void Roboremo_printf(const char* format, ...);

void Roboremo_PID_tuning(float* Inner_p,float* Inner_i,float* Inner_d, float* Outter_p,float* Outter_i,float* Outter_d); //pid���κ���
void Roboremo_Power_tuning(float* Total_Limit,float* Power_Remain); //���ʵ��κ���

/*Roboremo�����ýṹ��ȫ�ֿ���*/
extern Roboremo_PID_t roboremo_pid;
extern Roboremo_Power_t roboremo_power;
	
#endif


