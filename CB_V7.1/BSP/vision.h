#ifndef __VISION_H_
#define __VISION_H_

#include "system.h"

#define    FALSE    0
#define    TRUE     1

/*--------------------------����---------------------------*/

typedef __packed struct
{
	/* ͷ */
	uint8_t   SOF;			//֡ͷ��ʼλ,�ݶ�0xA5
	uint8_t   CmdID;		//ָ�� 00��Ҫ���ҷ������� 01���� 02����
	uint8_t   CRC8;			//֡ͷCRCУ��,��֤���͵�ָ������ȷ��
	
}extuart4_sendheader_t;

typedef __packed struct
{
	/* ���� */
	uint8_t data_0;//Ԥ����
	
}extuart4_senddata_t;

//������õ�����һ���ֽ�һ���ֽڵط��ͳ�ȥ
typedef __packed struct
{	
	/* β */
	uint16_t  CRC16;
	
}extuart4_sendtail_t;

/*--------------------------����---------------------------*/

typedef __packed struct
{
	/* ͷ */
	uint8_t   SOF;			//֡ͷ��ʼλ,�ݶ�0xA5
	uint8_t   CmdID;		//ָ�� 
	uint8_t   CRC8;			//֡ͷCRCУ��,��֤���͵�ָ������ȷ��
	
	/* ���� */
	float     y_fdb;    //���ص�����fdb
	uint8_t   captured; //��׽���� ���ʱ�������ִ���Զ�����
	/* β */
	uint16_t  CRC16;       
	
}extuart4_recvdata_t;

/*----------------------------------------------------------*/
typedef __packed struct
{	
//	bool     aim;       //�Ƿ�������Ӿ���׼
	bool     enable;
	bool     delay_record; 
  uint32_t light_delay_time; 
	uint32_t quit_delay_time; 
	uint8_t  TX_Cmd;
	uint8_t  RX_Cmd;
	uint8_t  captured;	
	float    y_fdb;
	float    y_ref;
}vision_t;

extern vision_t  vision;            //�Ӿ��ṹ��
 

void vision_init(void);
//void vision_param_init(void);
void vision_send_task(void);
void vision_task(void);

#endif
