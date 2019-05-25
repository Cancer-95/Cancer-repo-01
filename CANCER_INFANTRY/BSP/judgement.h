#ifndef __JUDGEMENT_H
#define __JUDGEMENT_H

#include "system.h"

#define CMD_ID_SIZE 	 		      2
#define FRAME_TAIL_SIZE 	      2

enum Judge_CmdID
{ 
	Robot_State_Data 		      	 = 0x0001,        //����������״̬�� 10Hz Ƶ�����ڷ���
	Hurt_Data					           = 0x0002,        //�˺����ݣ��յ��˺�ʱ����
	Real_Time_Shoot_Data 		     = 0x0003,        //ʵʱ������ݣ����䵯��ʱ����
	Real_Time_Power_Heat_Data	   = 0x0004,        //ʵʱ���ʺ��������ݣ�50Hz Ƶ�����ڷ���
	Real_Time_Rfid_Detect_Data   = 0x0005,    	  //ʵʱ���ؽ������ݣ���⵽ RFID ��ʱ 10Hz ���ڷ���
	Game_Result_Data 		      	 = 0x0006,        //����������ݣ���������ʱ����
	Get_Buff_Data 				       = 0x0007,        //��ȡ�� buff��������غ���һ��
	Game_Robot_Pos_Data          = 0x0008,        //������λ�ó�����Ϣ
	Team_Custom_Data 			       = 0x0100,   	    //�������Զ������ݣ�������ʾ�ڲ�������
};

//֡ͷ
typedef __packed struct
{
	uint8_t 	SQF; 			 //����֡��ʼ�ֽڣ��̶�ֵΪ0xA5
	uint16_t 	DataLength;		 //����֡��Data����
	uint8_t 	Seq;			 //�����
	uint8_t 	CRC8;			 //֡ͷCRCУ��
}extFrameHeader;

//����������״̬ (0x0001)
typedef __packed struct
{
	uint16_t  stageRemianTime;   //��ǰ�׶�ʣ��ʱ�䣬��λ s
	uint8_t   gameProgress;      //��ǰ�����׶�:0��δ��ʼ����  1��׼���׶�  2���Լ�׶�  3��5s ����ʱ  4����ս��  5������������
	uint8_t   robotLevel;        //�����˵�ǰ�ȼ�
	uint16_t  remainHP;          //�����˵�ǰѪ��
	uint16_t  maxHP;             //��������Ѫ��
}extGameRobotState_t;	

//�˺����� (0x0002)
typedef __packed struct
{
	uint8_t armorType : 4;
	uint8_t hurtType :  4;
}extRobotHurt_t;

//ʵʱ�����Ϣ (0x0003)
typedef __packed struct
{
	uint8_t bulletType;          //��������  1��17mm ����   2��42mm ����
	uint8_t bulletFreq;          //������Ƶ
	float bulletSpeed;           //��������
}extShootData_t;

//ʵʱ������������ (0x0004)
typedef __packed struct
{
	float chassisVolt;           //���������ѹ
	float chassisCurrent;        //�����������
	float chassisPower;          //�����������
	float chassisPowerBuffer;    //���̹��ʻ���
	uint16_t shooterHeat0;       //17mm ǹ������
	uint16_t shooterHeat1;       //42mm ǹ������
}extPowerHeatData_t;

//���ؽ������ݣ�0x0005��
typedef __packed struct
{
	uint8_t cardType;            //������0�������ӳɿ� 1�������ӳɿ� 2���췽��Ѫ�� 3��������Ѫ�� 4���췽���������ؿ� 5���������������ؿ�
	uint8_t cardIdx;             //�������ţ����������ֲ�ͬ����
}extRfidDetect_t;

//����ʤ�����ݣ�0x0006��
typedef __packed struct
{
	uint8_t winner;              //������� 0��ƽ�� 1���췽ʤ 2������ʤ
}extGameResult_t;

//Buff ��ȡ���ݣ�0x0007��
typedef __packed struct
{
	uint8_t buffType;            //Buff ���� 0�������ӳ� 1�������ӳ� 2����ô���������
	uint8_t buffAddition;        //�ӳɰٷֱȣ����� 10 ����ӳ� 10%��
} extGetBuff_t;

//������λ�ó�����Ϣ
typedef __packed struct
{
  float x;
	float y;
	float z;
	float yaw;
} extGameRobotPos_t;

//�������Զ������ݣ�0x0100��
typedef __packed struct
{
	float data1;
	float data2;
	float data3;
	uint8_t mask;
}extShowData_t;

typedef __packed struct
{
	uint16_t		     CmdID;
	extShowData_t 	 ShowData;
	uint16_t		     FrameTail;
}tTxGameInfo;

extern extGameRobotState_t   RobotStateData;
extern extRobotHurt_t 		   HurtData;
extern extShootData_t        ShootData;
extern extPowerHeatData_t    PowerHeatData;
extern extRfidDetect_t 		   RfidData;
extern extGameResult_t 	  	 GameResultData;
extern extGetBuff_t 		     BuffData;
extern extShowData_t 		     TeamCustomData;


void Judgement_Init(void);
void Receive_Data_From_Judgement(unsigned char *JudgementData);
void Send_Data_To_Judgement(void);




/*����CRC�ĺ�������*/
void CRC_init(void);
unsigned char Get_CRC8_Check_Sum(unsigned char *pchMessage,unsigned int dwLength,unsigned char ucCRC8);
unsigned int Verify_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
void Append_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage,uint32_t dwLength,uint16_t wCRC);
uint32_t Verify_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);
void Append_CRC16_Check_Sum(uint8_t * pchMessage,uint32_t dwLength);


/*------------- �Զ��� -------------*/

//extern float current_heat42;
//extern int Bomb_Shoot_Num;
//extern bool Shoot_Data_Update;

//��������
typedef struct
{
	float  Judge_chassisVolt;     //���������ѹ
  float  Judge_chassisCurrent;  //�����������
	bool   Judge_connect;         //���߱�־λ
	float  Judge_power_remain;    //����ʣ������
	float  Judge_power;         //����ʵʱ����
	int    Protect_num;           //
}PowerProtect_t;


//ǹ������
typedef struct
{
	float	   Current_heat17;   //ǹ������
	uint32_t Shoot_Time1;   //ʱ��1
	uint32_t Shoot_Time2;   //ʱ��2
	uint16_t Shooted_Num;   //�����ӵ�������
	uint32_t Shoot_Delay;   //�����ӳ�
}Shooter_t;

extern Shooter_t Shooter;
extern PowerProtect_t PowerProtect;

#endif




