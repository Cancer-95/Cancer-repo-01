#ifndef __JUDGEMENT_H
#define __JUDGEMENT_H

#include "system.h"

#define CMD_ID_SIZE 	 		 2
#define FRAME_TAIL_SIZE 	 2

#define BLUE  0
#define RED   1

enum Judge_CmdID
{ 
	Game_State 								= 0x0001,		//����������Ϣ 0x0001  ����Ƶ��:1Hz
	Game_Result 							= 0x0002,		//�����������:0x0002   ������������
	Game_Robot_Survivors			= 0x0003,		//�����˴������:0x0003   ����Ƶ��:1Hz
	Event_Data 								= 0x0101,		//����ʱ�����ݣ�0x0101  ����Ƶ�ʣ��¼��ı����
	Supply_Projectile_Action	= 0x0102,		//����վ������ʶ��0x0102  ����Ƶ�ʣ������ı����
	Supply_Orojectile_Booking = 0x0103,   //����վԤԼ�ӵ���cmd_id(0x0103) ����Ƶ�ʣ�����10Hz RM�Կ�����δ����
	Game_Robot_State					= 0x0201,		//����������״̬��0x0201  ����Ƶ��:10Hz
	Power_Heat_Data						= 0x0202,		//ʵʱ������������:0x0202  ����Ƶ��:50Hz
	Game_Robot_Pos						= 0x0203,		//������λ�ã�0x0203  ����Ƶ��:10Hz
	Buff_Musk									= 0x0204,		//���������棺0x0204   ����Ƶ�ʣ�״̬�ı����
	Aerial_Robot_Energy				=	0x0205,		//���л���������״̬��0x0205 ����Ƶ��:10Hz
	Robot_Hurt								= 0x0206,		//�˺�״̬:0x0206  ����Ƶ�ʣ��˺���������
	Shoot_Data								= 0x0207,		//ʵʱ�����Ϣ:0x0207 ����Ƶ�ʣ��������
};

//�����˽�����Ϣ���͵�ID  ���ж���
enum InteractiveStory
{
	Supply_Loading     =  0x0200,//����
	Supply_Unavailable =  0x0201,//û��
	Supply_Available   =  0x0202,//�е�
};

//֡ͷ
typedef __packed struct
{
	uint8_t 	SQF; 			 //����֡��ʼ�ֽڣ��̶�ֵΪ0xA5
	uint16_t 	DataLength;//����֡��Data����
	uint8_t 	Seq;			 //�����
	uint8_t 	CRC8;			 //֡ͷCRCУ��
}extFrameHeader;

//����������Ϣ 0x0001  ����Ƶ��:1Hz
typedef __packed struct
{
 uint8_t  game_type : 4;
 uint8_t  game_progress : 4;
 uint16_t stage_remain_time;
}ext_game_state_t;

//�����������:0x0002   ������������
typedef __packed struct
{
 uint8_t winner;
}ext_game_result_t;

//�����˴������:0x0003   ����Ƶ��:1Hz
typedef __packed struct
{
 uint16_t robot_legion;
} ext_game_robot_survivors_t;

//����ʱ�����ݣ�0x0101  ����Ƶ�ʣ��¼��ı����
typedef __packed struct
{
	uint32_t event_type;
} ext_event_data_t;

//����վ������ʶ��0x0102  ����Ƶ�ʣ������ı����
typedef __packed struct
{
 uint8_t supply_projectile_id;
 uint8_t supply_robot_id;
 uint8_t supply_projectile_step;
 uint8_t supply_projectile_num;
} ext_supply_projectile_action_t;

//����վԤԼ�ӵ���cmd_id(0x0103) ����Ƶ�ʣ�����10Hz RM�Կ�����δ����
typedef __packed struct
{
 uint8_t supply_projectile_id;
 uint8_t supply_robot_id;
 uint8_t supply_num;
} ext_supply_projectile_booking_t;


//����������״̬��0x0201  ����Ƶ��:10Hz
typedef __packed struct
{
 uint8_t  robot_id;
 uint8_t  robot_level;
 uint16_t remain_HP;
 uint16_t max_HP;
 uint16_t shooter_heat0_cooling_rate;
 uint16_t shooter_heat0_cooling_limit;
 uint16_t shooter_heat1_cooling_rate;
 uint16_t shooter_heat1_cooling_limit;
 uint8_t  mains_power_gimbal_output : 1;
 uint8_t  mains_power_chassis_output : 1;
 uint8_t  mains_power_shooter_output : 1;
} ext_game_robot_state_t;

//ʵʱ������������:0x0202  ����Ƶ��:50Hz
typedef __packed struct
{
 uint16_t chassis_volt;
 uint16_t chassis_current;
 float    chassis_power;
 uint16_t chassis_power_buffer;
 uint16_t shooter_heat0;
 uint16_t shooter_heat1; 
}ext_power_heat_data_t;

//������λ�ã�0x0203  ����Ƶ��:10Hz
typedef __packed struct
{
 float x;
 float y;
 float z;
 float yaw;
}ext_game_robot_pos_t;

//���������棺0x0204   ����Ƶ�ʣ�״̬�ı����
typedef __packed struct
{
 uint8_t power_rune_buff;
}ext_buff_musk_t;

//���л���������״̬��0x0205 ����Ƶ��:10Hz
typedef __packed struct
{
 uint8_t energy_point;
 uint8_t attack_time;
}aerial_robot_energy_t;

//�˺�״̬:0x0206  ����Ƶ�ʣ��˺���������
typedef __packed struct
{
 uint8_t armor_id : 4;
 uint8_t hurt_type : 4;
}ext_robot_hurt_t;

//ʵʱ�����Ϣ:0x0207 ����Ƶ�ʣ��������
typedef __packed struct
{
 uint8_t bullet_type;
 uint8_t bullet_freq;
 float   bullet_speed;
}ext_shoot_data_t;

/*�����˽�������*/
//�������ݽ�����Ϣ:0x0301  ����Ƶ��:����10Hz
typedef __packed struct
{
 uint16_t data_cmd_id;  //����ID
 uint16_t send_ID;
 uint16_t receiver_ID;
}ext_student_interactive_header_data_t;

//����ID           ���ڴ��ڶ������ID,������cmd_id����Ƶ�������10Hz  ע������Ŵ���
/*����ID           ����                        ����        				 */
/*                 (ͷ�ṹ����+�������ݳ���) 	 	 									 */
/*0xD180            6+13 											 �ͻ����Զ�������		 */
/*0X0200~0X02FF		  6+n												 ���������˼�ͨ��		 */


//�ϴ��ͻ����Զ�������   cmd_id:0x0301   ����id:0xD180   ����Ƶ��:����10Hz
typedef __packed struct
{
	float   data1;/*������ʾ������״̬ 
	                ǰ��λ:000��еģʽ 111������ģʽ  
	                ����λ:000����ģʽ 111�ǵ�ģʽ 222ȡ��ģʽ 333����ģʽ 444��Ԯģʽ*/
	float   data2;/*������ʾ����ģʽ�л� 
	                ����111111 ����333333 ����555555 ����000000*/
	float   data3;/*������ʾ���ֵ��� 
	                �������һ��ȡ������+20 �������һ��ȡ������+6*/
	uint8_t masks;/*�����Ӿ�ȫ��111111 ɨ���϶��101010 ����ȫ��000000*/

}client_custom_data_t;

//ѧ���������ͨ��  cmd_id:0x0301  ����id��0x0201~0x02FF   ����Ƶ�ʣ�����10Hz
typedef __packed struct
{
	uint8_t  data_frame[112];//С��113�ֽ�
}robot_interactive_data_t;

//֡ͷ  ������  ���ݶ�ͷ�ṹ  ���ݶ�   ֡β
//�ϴ��ͻ���
typedef __packed struct
{
	extFrameHeader                          txFrameHeader;//֡ͷ
	uint16_t		                            CmdID;//������
	ext_student_interactive_header_data_t   dataFrameHeader;//���ݶ�ͷ�ṹ
	client_custom_data_t  	                clientData;//���ݶ�
	uint16_t		                            FrameTail;//֡β
}exSendClientData_t;

//�����˽�����Ϣ
typedef __packed struct
{
	extFrameHeader                          txFrameHeader;//֡ͷ
	uint16_t		                            CmdID;//������
	ext_student_interactive_header_data_t   dataFrameHeader;//���ݶ�ͷ�ṹ
	robot_interactive_data_t  	            interactData;//���ݶ�
	uint16_t		                            FrameTail;//֡β
}exCommunatianData_t;

/*--------------------------------- �Զ��� -------------------------------*/
/*execute_task*/
#define NORMAL_TASK           (float)000000
#define CLIMB_TUBE_OPTIC_TASK (float)111110 
#define CLIMB_TUBE_GYRO_TASK  (float)101010 
#define GET_BOMB_TASK         (float)222220
#define FEED_BOMB_TASK        (float)333330
#define RESCUE_TASK           (float)444440

#define SEPARATE_CHASSIS      (float)1
#define FOLLOW_CHASSIS        (float)2	
#define UNCONTROLLED_CHASSIS  (float)9
/*get_bomb_mode*/
#define GET_NO_CASE           (float)000000
#define GET_SINGLE_CASE       (float)111110
#define GET_TRIPLE_CASE       (float)333330
#define GET_PENTA_CASE        (float)555550
	
#define LOCATED_ON_GROUND     (float)0
#define LOCATED_ON_ISLAND     (float)1 
#define ROBOT_IN_DANGER       (float)999999
/*status_bar*/
#define VISION_ASSIST         (uint8_t)(7<<3|7)
#define SCAN_FAILED           (uint8_t)(5<<3|2)
#define SCAN_ASSIST           (uint8_t)(0<<3|7)
#define NO_ASSIST             (uint8_t)(0<<3|0)

//�ϴ��ͻ����Զ�������   cmd_id:0x0301   ����id:0xD180   ����Ƶ��:����10Hz
typedef __packed struct
{
	float   execute_task;      /*������ʾ������״̬  
	                             ǰ��λ:111��еģʽ 222������ģʽ 333���ɿ�ģʽ
	                             ����λ:000����ģʽ 111�ǵ�ģʽ 222ȡ��ģʽ 333����ģʽ 444��Ԯģʽ*/
	float   get_bomb_mode;     /*������ʾ����ģʽ�л� 
	                             ����111111 ����333333 ����555555 ����000000*/
	float   golf_num;          /*������ʾ���ֵ��� 
	                             �������һ��ȡ������+20 �������һ��ȡ������+6*/
	bool    golf_num_add_once; /*�ӷ�ÿ��ֻ��һ�ο*/
	uint8_t status_bar;        /*�����Ӿ�ȫ��111111 ɨ���϶��101010 ����ȫ��000000*/

}data_for_client_t;

extern data_for_client_t data_for_client;

/*--------------------------------- �������� -------------------------------*/
void judgement_init(void);
void receive_data_from_judgement(unsigned char *JudgementData);
void send_to_teammate(void);
void send_to_client(void);
bool is_red_or_blue(void);
void confirm_ID(void);
void judgement_send_task(void);

/*����CRC�ĺ�������*/
void CRC_init(void);
unsigned char Get_CRC8_Check_Sum(unsigned char *pchMessage,unsigned int dwLength,unsigned char ucCRC8);
unsigned int Verify_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
void Append_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage,uint32_t dwLength,uint16_t wCRC);
uint32_t Verify_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);
void Append_CRC16_Check_Sum(uint8_t * pchMessage,uint32_t dwLength);

#endif

