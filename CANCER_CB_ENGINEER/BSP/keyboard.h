#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "system.h"

/* get information task period time (ms) */
#define INFO_GET_PERIOD  1 //�����ĸ�ʱ���

/* key acceleration time */
#define NORMAL_ACC_TIME  250  //ms ����ģʽ��������ʱ��
#define CTRL_ACC_TIME    100  //ms ����ģʽ��������ʱ��
#define SHIFT_ACC_TIME   100  //ms ����ģʽ��������ʱ��

/* turn around time */
#define TUR_ARO_TIME     870  //ms ��ͷʱ��

/* turn left time */
#define TUR_LEF_TIME     500   //ms ��תʱ��

/* turn right time */
#define TUR_RIG_TIME     500   //ms ��תʱ��

/* spin brake time */
#define SPIN_BRAKE_TIME  350   //ms ��תɲ��ʱ��

//      direction key
#define FORWARD                        (KEY_W_PRESSED ? 1 : 0)
#define BACK                           (KEY_S_PRESSED ? 1 : 0)
#define LEFT                           (KEY_A_PRESSED ? 1 : 0)
#define RIGHT                          (KEY_D_PRESSED ? 1 : 0)
//      speed key
#define FAST_SPD                       (KEY_SHIFT_PRESSED ? 1 : 0)
#define SLOW_SPD                       (KEY_CTRL_PRESSED ? 1 : 0)
//      force speed key
#define FAST_SPD_ENABLE                 1
#define SLOW_SPD_ENABLE                 1
#define FAST_SPD_DISABLE                0
#define SLOW_SPD_DISABLE                0
//      if_gyro_blow_up
#define GYRO_BLOW_UP_CTRL               ((KEY_C_PRESSED ? 1 : 0) && KEY_X_PRESSED && KEY_Z_PRESSED) //Z+X+C ���������ը��ȡ��������ģʽ
//      climb_mode key
#define RUN_PLAIN                       ((KEY_Z_PRESSED ? 1 : 0) && KEY_CTRL_PRESSED)  //CTRL+Z   ƽԭģʽ
#define CLIMB_UP_OPTIC                  ((KEY_Q_PRESSED ? 1 : 0) && KEY_CTRL_PRESSED)  //CTRL+Q   ����ģʽ
#define CLIMB_DOWN_OPTIC                ((KEY_E_PRESSED ? 1 : 0) && KEY_CTRL_PRESSED)  //CTRL+E   �¸�ģʽ
#define CLIMB_UP_GYRO                   ((KEY_Q_PRESSED ? 1 : 0) && KEY_SHIFT_PRESSED) //SHIFT+Q  ����ģʽ
#define CLIMB_DOWN_GYRO                 ((KEY_E_PRESSED ? 1 : 0) && KEY_SHIFT_PRESSED) //SHIFT+E  �¸�ģʽ
//      tube claw ctrl
#define TUBE_CLAW_VISION_CTRL           ((KEY_V_PRESSED ? 1 : 0) && KEY_SHIFT_PRESSED)//SHIFT+V �Ӿ�������λ
#define TUBE_CLAW_CATCH_CTRL            (MOUSE_R_PRESSED ? 1 : 0) //����Ҽ���������צ��
//      turn_around 
#define TURN_AROUND                     ((KEY_C_PRESSED ? 1 : 0) && KEY_CTRL_PRESSED)//CTRL+C ԭ����ת180
#define TURN_AROUND_DISABLE             0
//      turn_left 
#define TURN_LEFT                       (MOUSE_L_PRESSED ? 1 : 0)//��������ת90��
#define TURN_LEFT_DISABLE               0
//      turn_right 
#define TURN_RIGHT                      (MOUSE_R_PRESSED ? 1 : 0)//����Ҽ���ת90��
#define TURN_RIGHT_DISABLE              0
//      spin_around 
#define SPIN_AROUND                     ((KEY_C_PRESSED ? 1 : 0) && KEY_SHIFT_PRESSED) //SHIFT+C ԭ����ת
#define SPIN_AROUND_DISABLE             0
//      get_bomb key
#define GET_BOMB_CTRL                   ((KEY_R_PRESSED ? 1 : 0) && KEY_CTRL_PRESSED)                      //CTRL+R �����˳�ȡ��ģʽ
#define GET_BOMB_NO_TURN_AROUND_CTRL    ((KEY_R_PRESSED ? 1 : 0) && KEY_CTRL_PRESSED && KEY_SHIFT_PRESSED) //SHIFT+CTRL+R �����˳�ȡ��ģʽ(����ͷ)
#define GET_BOMB_CLEAR_CTRL             ((KEY_Z_PRESSED ? 1 : 0) && KEY_CTRL_PRESSED)                      //CTRL+Z ���������İ���
//      bomb_claw ctrl
#define BOMB_CLAW_ROLL_CTRL             (MOUSE_L_PRESSED ? 1 : 0)                     //��������������צ��
#define BOMB_CLAW_CATCH_CTRL            (MOUSE_R_PRESSED ? 1 : 0)                     //��������Ҽ���������צ��
#define BOMB_CLAW_PUSH_CTRL             ((KEY_V_PRESSED ? 1 : 0) && KEY_CTRL_PRESSED) //CTRL+V ǰ�ź���ѡ��
#define BOMB_CLAW_TRIPLE_CTRL           ((KEY_F_PRESSED ? 1 : 0) && KEY_CTRL_PRESSED) //CTRL+F �л�����
#define BOMB_CLAW_PENTA_CTRL            ((KEY_F_PRESSED ? 1 : 0) && KEY_SHIFT_PRESSED)//SHIFT+F �л�����
#define BOMB_CLAW_SCAN_CTRL             ((KEY_R_PRESSED ? 1 : 0) && KEY_SHIFT_PRESSED)//SHIFT+R �Զ�ɨ��Ŀ���
#define BOMB_CLAW_VISION_CTRL           ((KEY_V_PRESSED ? 1 : 0) && KEY_SHIFT_PRESSED)//SHIFT+V �Ӿ�����ȡ��
#define BOMB_CLAW_CCTV_CTRL             (KEY_G_PRESSED ? 1 : 0)                       //G С�����л�
////      supporting_wheel ctrl
//#define SUPPORTING_WHEEL_CTRL ((KEY_F_PRESSED ? 1 : 0) && KEY_SHIFT_PRESSED)//SHIFT+F ����֧���ȵĿ���
//      rescue ctrl
#define RESCUE_CTRL                     ((KEY_X_PRESSED ? 1 : 0) && KEY_CTRL_PRESSED)                      //CTRL+X �����˳���Ԯģʽ
#define RESCUE_NO_TURN_AROUND_CTRL      ((KEY_X_PRESSED ? 1 : 0) && KEY_CTRL_PRESSED && KEY_SHIFT_PRESSED) //SHIFT+CTRL+X �����˳���Ԯģʽ(����ͷ)
#define RESCUE_SIGHT_CTRL               (MOUSE_L_PRESSED ? 1 : 0)                                          //������ �ֶ��л��ӽ�
#define ELECTROMAGNET_CTRL              (MOUSE_R_PRESSED ? 1 : 0)                                          //����Ҽ� �ֶ����ƹ���
#define TRAILER_HOOK_CTRL               (MOUSE_R_PRESSED ? 1 : 0)                                          //����Ҽ� �ֶ����ƹ���
#define HOOK_CONFIRM_CTRL               ((KEY_F_PRESSED ? 1 : 0) && KEY_CTRL_PRESSED)                      //CTRL+F ȷ�Ϲ�ס
//      feed_bomb_ctrl
#define FEED_BOMB_CTRL                  ((KEY_G_PRESSED ? 1 : 0) && KEY_CTRL_PRESSED)                      //CTRL+G �����˳�����ģʽ
#define FEED_BOMB_NO_TURN_AROUND_CTRL   ((KEY_G_PRESSED ? 1 : 0) && KEY_CTRL_PRESSED && KEY_SHIFT_PRESSED) //SHIFT+CTRL+G �����˳�����ģʽ(����ͷ)
#define FEED_BOMB_SIGHT_CTRL            (MOUSE_L_PRESSED ? 1 : 0)                                          //������  �ֶ��л��ӽ�
#define MAGAZINE_LIFTER_CTRL            (MOUSE_R_PRESSED ? 1 : 0)                                          //����Ҽ�  �ֶ����Ƶ�ҩ����̧��(����)
#define MAGAZINE_EXIT_CTRL              ((KEY_F_PRESSED ? 1 : 0) && KEY_CTRL_PRESSED)                      //CTRL+F �ֶ����Ƶ�ҩ����ڿ���
#define FEED_BOMB_VISION_CTRL           ((KEY_V_PRESSED ? 1 : 0) && KEY_SHIFT_PRESSED)                     //SHIFT+V �Ӿ���������

typedef enum 
{
  NORMAL_SPEED = 0, //�����ٶ����ڳ�ʼ״̬
  FAST_SPEED   = 1,
  SLOW_SPEED   = 2,
} kb_chassis_speed;

typedef __packed  struct
{
  float vx;
  float vy;
  float vw; 
  
  bool kb_enable;
	
	bool tube_claw_enable;	//����צ��ʹ��λ
	bool bomb_claw_enable;	//ȡ��צ��ʹ��λ

  kb_chassis_speed move_speed;
  
	uint16_t x_spd_limit; //X�����ٶ����� (����)
  uint16_t y_spd_limit; //Y�����ٶ����� (����)
	uint16_t w_spd_limit; //W�����ٶ����� (����)
  uint16_t acc_time; //W�����ٶ����� (����)
} kb_ctrl_t;

/*������������*/
typedef __packed  struct
{
  bool climb_tube_task_enable;
	bool get_bomb_task_enable;	
	bool rescue_task_enable;	
	bool feed_bomb_task_enable;	
} global_task_t;

extern kb_ctrl_t kb;
extern global_task_t global_task;

extern bool flag_turn_around;       //�ⲿ�����Զ���ͷ ���ھ�Ԯ������
extern bool flag_spin_around;       //chassis.h

extern bool flag_tube_claw_catch;   //��relay.h�еĺ�׼��
extern bool flag_tube_claw_vision;

extern bool flag_bomb_claw_catch;   //��relay.h�еĺ�׼��
extern bool flag_bomb_claw_roll;    //��get_bomb.c׼��
extern bool flag_bomb_claw_push;    //��relay.h�еĺ�׼��
extern bool flag_bomb_claw_triple;
extern bool flag_bomb_claw_penta;
extern bool flag_bomb_claw_vision;
extern bool flag_bomb_claw_scan; 
extern bool flag_bomb_claw_CCTV; 
//extern bool flag_get_bomb;          //��get_bomb.c�л�case��

extern bool get_bomb_clear_record;    //�˳�ִ�о������

extern bool flag_electromagnet;
extern bool flag_trailer_hook;        //��relay.h�еĺ�׼��  
extern bool flag_rescue_sight;

extern bool flag_magazine_exit;          //��servo.h�еĺ�׼��  
extern bool flag_magazine_lifter;       
extern bool flag_feed_bomb_vision;
extern bool flag_feed_bomb_sight;
//extern bool flag_supporting_wheel;  //��relay.h�еĺ�׼��


extern bool flag_gyro_blow_up;

void global_task_init(void);

void keyboard_chassis_handler(void);//���̵��̿���

void keyboard_climb_tube_handler(void);//���˼��̿���

void keyboard_get_bomb_handler(void);//ȡ�����̿���

void keyboard_rescue_handler(void);//��Ԯ���̿���

void keyboard_feed_bomb_handler(void);//�������̿���

#endif

