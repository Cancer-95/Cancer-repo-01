#include "get_bomb.h"

get_bomb_t get_bomb;

void get_bomb_param_init(void)
{
	lifter.ctrl_mode = LIFTER_ZERO;     //框架慢慢降下来	

	kb.bomb_claw_enable = DISABLE;           //取弹爪子
//	SUPPORTING_WHEEL_PULL;                   //辅助支撑腿收回
	BOMB_CLAW_CATCH;                         //取弹爪子合上
	BOMB_EJECT_PULL;                         //弹射收回
	
	BOMB_CLAW_MOVE_LEFT;                     //
//	BOMB_CLAW_MOVE_MID;                      //取弹爪子平移至中
	BOMB_CLAW_ROLL_ZERO;                     //取弹爪子翻转归零	
//	BOMB_CLAW_ROLL_ENTIRE; 
	BOMB_CLAW_PULL;                          //取弹爪子收回

	get_bomb.once_triple = 0;							
	flag_bomb_claw_triple = 0;               //关掉三箱模式 让下次按ctrl+R 默认是单箱模式
	
	get_bomb.once_penta = 0;							
	flag_bomb_claw_penta = 0;                //关掉五箱模式 让下次按ctrl+R 默认是单箱模式
	
	vision.enable = DISABLE;
//	vision.aim = HAVENT;
//	get_bomb.vision_assist = 0;	
	flag_bomb_claw_vision = 0;               //关掉视觉模式 让下次按ctrl+R 默认是单箱模式
	
	get_bomb.scan = 0;	
  get_bomb.flag_scan_failed = NO;	
	get_bomb.scan_finished = NO;	
	optic_switch.value_record	= ENABLE;	
  get_bomb.scan_delay_record = ENABLE;	
	flag_bomb_claw_scan = 0;                 //关掉扫描模式 让下次按ctrl+R 默认是单箱模式
	
	get_bomb.turn_around_record = ENABLE;    //岛下取弹掉头使能
	get_bomb.turn_around_enable = ENABLE;
	
	get_bomb.flag_push_aim_single = HAVENT;    //还没完成推出对准模式	
	get_bomb.flag_push_aim_multiple = HAVENT;  //还没完成推出对准模式
	get_bomb.flag_push_aim_scan = HAVENT;      //还没完成推出对准模式
	
	get_bomb.get_pour_step = 0;              //取弹倒弹模式step归零	
}

bool flag_get_bomb_init = 1;

bool flag_push_aim_single_init = 1;
bool flag_push_aim_multiple_init = 1;
bool flag_push_aim_scan_init = 1;
bool flag_scan_safe_delay = 0;//延时一小段时间

bool flag_eject_push_Time_record = 1;
bool flag_bomb_claw_record = 0;  

void get_bomb_ctrl(void)
{
	/*若初始化就Y_CLAW_HALF会出问题 故放在这里初始化*/	
	if(flag_get_bomb_init)
	{
		get_bomb_param_init();
		BOMB_CLAW_MOVE_MID;                      //取弹爪子平移至中
		BOMB_CLAW_ROLL_ENTIRE; 
		
	  flag_push_aim_single_init = 1;
	  flag_push_aim_multiple_init = 1;
		flag_push_aim_scan_init = 1;
		flag_scan_safe_delay = 0;
		
		flag_get_bomb_init = 0;
	}

	keyboard_get_bomb_handler();//keyboard	
	
  if(get_bomb.get_bomb_enable == ENABLE)	
	{
//		get_bomb.vision_assist = ENABLE;//全程允许视觉对位
		
		switch(get_bomb.ctrl_mode)
		{	
			case STAND_BY_MODE://归中待命模式
			{		
				get_bomb.turn_around_record = ENABLE;   //岛下取弹掉头使能
				get_bomb.turn_around_enable = ENABLE;
				
		    chassis.ass_mode_enable = DISABLE; 	    //关掉车尾模式  
				chassis.force_push_in_enable = DISABLE; //关掉推进模式
				chassis.ctrl_mode = CHASSIS_FOLLOW;	    //底盘陀螺仪模式
				
				flag_bomb_claw_CCTV = 0;
				flag_bomb_claw_CCTV_switch = 0;
				
				flag_eject_push_Time_record = 1;
				
				flag_push_aim_single_init = 1;
				flag_push_aim_multiple_init = 1;
				flag_push_aim_scan_init = 1;
				flag_scan_safe_delay = 0;
				
				get_bomb.once_triple = 0;							
	      flag_bomb_claw_triple = 0;               //关掉三箱模式 让下次按ctrl+R 默认是单箱模式
				
				get_bomb.once_penta = 0;							
	      flag_bomb_claw_penta = 0;                //关掉五箱模式 让下次按ctrl+R 默认是单箱模式
				
//				get_bomb.vision_assist = DISABLE;
				vision.enable = DISABLE;
//				vision.aim = HAVENT;
//				get_bomb.vision_assist = 0;	
				flag_bomb_claw_vision = 0;               //关掉视觉模式 让下次按ctrl+R 默认是单箱模式
				
			  get_bomb.scan = 0;	
				get_bomb.flag_scan_failed = NO;
        get_bomb.scan_finished = NO;	
        optic_switch.value_record	= ENABLE;	
				get_bomb.scan_delay_record = ENABLE;			
		    flag_bomb_claw_scan = 0;             //关掉扫描模式 让下次按ctrl+R 默认是单箱模式
				
				/*修正中心值Y轴*/	
				if(bomb_claw.target_half[Y] > bomb_claw.Y_target[M])
				{
					bomb_claw.from_claw_entire[Y_axis] = YES;
					bomb_claw.from_claw_zero[Y_axis] = NO;
				}	
				else if(bomb_claw.target_half[Y] < bomb_claw.Y_target[M])
				{
					bomb_claw.from_claw_entire[Y_axis] = NO;
					bomb_claw.from_claw_zero[Y_axis] = YES;
				}		
				bomb_claw.target_half[Y] = bomb_claw.Y_target[M];
				/*修正框架目标值*/ 
				if(lifter.target_bomb_bit[L] != lifter.target_bomb_bit_normal[L] || 
					 lifter.target_bomb_bit[R] != lifter.target_bomb_bit_normal[R])		
				{
					lifter.target_bomb_bit[L] = lifter.target_bomb_bit_normal[L];
					lifter.target_bomb_bit[R] = lifter.target_bomb_bit_normal[R];
					if((lifter.total_angle[L] > lifter.target_bomb_bit[L]) && 
						 (lifter.total_angle[R] < lifter.target_bomb_bit[R]))
					{
						from_lifter_zero = YES;
						from_lifter_bomb_entire = NO; 			
					}
					else if((lifter.total_angle[L] < lifter.target_bomb_bit[L]) && 
									(lifter.total_angle[R] > lifter.target_bomb_bit[R]))
					{
						from_lifter_zero = NO;
						from_lifter_bomb_entire = YES; 			
					}
				}		
				/*修正平台左右目标值*/
				if(bomb_claw.target_entire[Y] !=  bomb_claw.Y_target[R] ||
 				   bomb_claw.target_zero[Y] != bomb_claw.Y_target[L])
				{
					bomb_claw.target_entire[Y] =  bomb_claw.Y_target[R];
					bomb_claw.target_zero[Y] = bomb_claw.Y_target[L];
				}
				
				
				kb.bomb_claw_enable = DISABLE;    	 //取弹爪撞不可控	 关掉了按ctrl+E是没有什么用的
				
//	      SUPPORTING_WHEEL_PULL;               //辅助支撑腿收回				
				BOMB_CLAW_CATCH;                   	 //取弹爪子合上
				BOMB_EJECT_PULL;                     //弹射收回

        BOMB_CLAW_MOVE_MID;               	 //取弹爪子平移至中
        BOMB_CLAW_ROLL_ENTIRE;               //取弹爪子翻转归零			
				BOMB_CLAW_PULL;                   	 //取弹爪子收回		
				
				GIM_SERVO_ZERO;                      //云台前面转到最前面

				get_bomb.flag_push_aim_single = HAVENT;     //还没完成推出对准模式	
				get_bomb.flag_push_aim_multiple = HAVENT;   //还没完成推出对准模式	
				get_bomb.flag_push_aim_scan = HAVENT;       //还没完成推出对准模式
				
				get_bomb.get_pour_step = 0;          //取弹倒弹模式step归零
					
//        if(servo_get_bomb_delay_record)//延时一小段时间让舵机转回来先
//				{					
//					servo_get_bomb_delay_time = millis();
//					servo_get_bomb_delay_record = 0;
//				}
//				if(millis() - servo_get_bomb_delay_time > 800)
//				{
				if((abs(bomb_claw.total_angle[L] - bomb_claw.target_entire[L]) < 100) || 
					 (abs(bomb_claw.total_angle[R] - bomb_claw.target_entire[R]) < 100))	//爪子翻过来了 假想气缸总是要比爪子翻得快的				
				{
					get_bomb.get_bomb_enable = DISABLE; //让这个case跑一次就退出了
					lifter.ctrl_mode = LIFTER_ZERO;     //框架慢慢降下来
				}										
//				}		
			}break;
					
			case PUSH_AIM_SINGLE_MODE://单箱模式下的推出对准模式
			{	
				if((climb_tube.location == ON_GROUND) && 
					 (climb_tube.ctrl_mode == PLAIN_MODE) &&
				    chassis.ctrl_mode == CHASSIS_FOLLOW &&
				    get_bomb.turn_around_enable == ENABLE)//岛下先掉个头
				{
					if(get_bomb.turn_around_record)
					{
						get_bomb.turn_around_time = millis();
						flag_turn_around = 1;
						chassis.position_temp = imu.yaw;
						ramp_init(&turn_around_ramp, TUR_ARO_TIME/INFO_GET_PERIOD);		
						get_bomb.turn_around_record = DISABLE;
					}
					else if((!flag_turn_around) && ((millis() - get_bomb.turn_around_time) > 1200))//掉头完成前保持陀螺仪模式 延时一小段时间尽量让它转到位
					{
						chassis.ass_mode_enable = ENABLE;               //开启车尾模式
						chassis.force_push_in_enable = ENABLE;          //开启推进模式
						if(vision.enable == DISABLE)
						{
							chassis.ctrl_mode = CHASSIS_SEPARATE;         //底盘机械模式
						}							
					}
				}
	      else //岛上该干嘛干嘛
				{
					chassis.ass_mode_enable = ENABLE;               //开启车尾模式
					chassis.force_push_in_enable = ENABLE;          //开启推进模式
					if(vision.enable == DISABLE)
					{
						chassis.ctrl_mode = CHASSIS_SEPARATE;         //底盘机械模式
					}							
				}					
	
		  	{
				  /*修正中心值Y轴*/	
					if(bomb_claw.target_half[Y] > bomb_claw.Y_target[M])
					{
						bomb_claw.from_claw_entire[Y_axis] = YES;
						bomb_claw.from_claw_zero[Y_axis] = NO;
					}	
					else if(bomb_claw.target_half[Y] < bomb_claw.Y_target[M])
					{
						bomb_claw.from_claw_entire[Y_axis] = NO;
						bomb_claw.from_claw_zero[Y_axis] = YES;
					}		
					bomb_claw.target_half[Y] = bomb_claw.Y_target[M];
					if(!vision.enable)
					{
						/*修正框架目标值*/ 
						if(lifter.target_bomb_bit[L] != lifter.target_bomb_bit_normal[L] || 
							 lifter.target_bomb_bit[R] != lifter.target_bomb_bit_normal[R])		
						{
							lifter.target_bomb_bit[L] = lifter.target_bomb_bit_normal[L];
							lifter.target_bomb_bit[R] = lifter.target_bomb_bit_normal[R];
							if((lifter.total_angle[L] > lifter.target_bomb_bit[L]) && 
								 (lifter.total_angle[R] < lifter.target_bomb_bit[R]))
							{
								from_lifter_zero = YES;
								from_lifter_bomb_entire = NO; 			
							}
							else if((lifter.total_angle[L] < lifter.target_bomb_bit[L]) && 
											(lifter.total_angle[R] > lifter.target_bomb_bit[R]))
							{
								from_lifter_zero = NO;
								from_lifter_bomb_entire = YES; 			
							}	
						}					
					}
					/*修正平台左右目标值*/
					if(bomb_claw.target_entire[Y] !=  bomb_claw.Y_target[R] || 
						 bomb_claw.target_zero[Y] != bomb_claw.Y_target[L])
					{
						bomb_claw.target_entire[Y] =  bomb_claw.Y_target[R];
						bomb_claw.target_zero[Y] = bomb_claw.Y_target[L];
					}			
					
	//				servo_get_bomb_delay_record = 1;
	//				servo_get_bomb_scan_delay_record = 1;
					
					get_bomb.once_triple = 0;							
					flag_bomb_claw_triple = 0;               //关掉三箱模式 让下次按ctrl+R 默认是单箱模式
					
					get_bomb.once_penta = 0;							
					flag_bomb_claw_penta = 0;                //关掉五箱模式 让下次按ctrl+R 默认是单箱模式
					
	//				vision.enable = DISABLE;
	//				vision.aim = HAVENT;
	//				get_bomb.vision_assist = 0;	
	//				flag_bomb_claw_vision = 0;               //关掉视觉模式 让下次按ctrl+R 默认是单箱模式
					
					get_bomb.scan = 0;	
					get_bomb.flag_scan_failed = NO;
					optic_switch.value_record	= ENABLE;		
					get_bomb.scan_delay_record = ENABLE;						
					get_bomb.scan_finished = NO;				
					flag_bomb_claw_scan = 0;             //关掉扫描模式 让下次按ctrl+R 默认是单箱模式
					
					get_bomb.flag_push_aim_single = HAVENT;     //还没完成单箱模式下推出对准模式
					get_bomb.flag_push_aim_multiple = HAVENT;   //还没完成三箱模式下推出对准模式
					get_bomb.flag_push_aim_scan = HAVENT;       //还没完成推出对准模式
					
					BOMB_CLAW_MOVE_MID;                 //强行移到中间
					get_bomb.get_pour_step = 0;         //后面的取弹步骤清零		
					
					if(flag_push_aim_single_init)
					{
						BOMB_CLAW_ROLL_HALF;//强制翻转爪子至HALF		

//						if((abs(lifter.target_bomb_bit[L] - lifter.total_angle[L]) < 100) || 
//							 (abs(lifter.target_bomb_bit[R] - lifter.total_angle[R]) < 100))
//						{
							GIM_SERVO_SIDE;
//						}						
						
						if((abs(bomb_claw.total_angle[L] - bomb_claw.target_half[L]) < 100) || 
							 (abs(bomb_claw.total_angle[R] - bomb_claw.target_half[R]) < 100))			
						{
	//						if(climb_tube.location == ON_ISLAND)
	//						{
	//					     BOMB_CLAW_PUSH;
	//						}
	//						else BOMB_CLAW_PULL;					
							
							BOMB_CLAW_PULL;             

							BOMB_EJECT_PULL;//弹射收回														
							BOMB_CLAW_LOOSE;//强制张开爪子 
							
							if(flag_eject_push_Time_record)
							{
								get_bomb.eject_push_Time = millis();//记录弹射的时刻ms	
								flag_eject_push_Time_record = 0;
							}
							if((millis() - get_bomb.eject_push_Time > 350) && 
								 (millis() - get_bomb.eject_push_Time < 700))//ms
							{
								BOMB_EJECT_PUSH;
							}
							else if(millis() - get_bomb.eject_push_Time > 1050)//ms
							{
								BOMB_EJECT_PULL;
								flag_push_aim_single_init = 0;
								flag_push_aim_multiple_init = 1;
								flag_push_aim_scan_init = 1;
								flag_scan_safe_delay = 0;
								get_bomb_clear_record = 0;//退出纠错重来的控制							
							}					
						}
					}
					
					if((abs(lifter.target_bomb_bit[L] - lifter.total_angle[L]) < 100) ||
						 (abs(lifter.target_bomb_bit[R] - lifter.total_angle[R]) < 100))//抬升到目标角度
					{ /*框架抬到了位置才执行伸出对准*/	
							kb.bomb_claw_enable = ENABLE;//取弹爪子可控								
					
						if((flag_bomb_claw_catch) && ((abs(bomb_claw.target_zero[L] - bomb_claw.total_angle[L]) < 100) || (abs(bomb_claw.target_zero[R] - bomb_claw.total_angle[R]) < 100)))							
						{						
							get_bomb.flag_push_aim_single = FINISHED;//标记完成了推出对准模式
						}		
					}
					else lifter.ctrl_mode = BOMB_LIFTER_BIT;//框架抬升一点
				}					
																		
			}break;
						
			case PUSH_AIM_MULTIPLE_MODE://多箱模式下的推出对准模式
			{					
				
				chassis.ass_mode_enable = ENABLE;                //开启车尾模式
				chassis.force_push_in_enable = ENABLE;           //开启推进模式
				if(vision.enable == DISABLE)
				{
					chassis.ctrl_mode = CHASSIS_SEPARATE;          //底盘机械模式
				}
				
			
//				vision.enable = DISABLE;
//				vision.aim = HAVENT;				
//				get_bomb.vision_assist = 0;	
//	      flag_bomb_claw_vision = 0;                  //关掉视觉模式 让下次按ctrl+R 默认是单箱模式
				
				get_bomb.scan = 0;	
				get_bomb.flag_scan_failed = NO;
				optic_switch.value_record	= ENABLE;			
				get_bomb.scan_delay_record = ENABLE;	
        get_bomb.scan_finished = NO;								
		    flag_bomb_claw_scan = 0;                    //关掉扫描模式 让下次按ctrl+R 默认是单箱模式
					
//				if(get_bomb.once_triple)
//				{
//					get_bomb.once_penta = 0;							
//					flag_bomb_claw_penta = 0;
//				}
//				else if(get_bomb.once_penta)
//				{
//					get_bomb.once_triple = 0;	
//					flag_bomb_claw_triple = 0;
//				}
				
				get_bomb.flag_push_aim_single = HAVENT;     //还没完成单箱模式下推出对准模式	
				get_bomb.flag_push_aim_multiple = HAVENT;   //还没完成三箱模式下推出对准模式
				get_bomb.flag_push_aim_scan = HAVENT;       //还没完成三箱模式下推出对准模式

				if(get_bomb.box_num == 3 || get_bomb.box_num == 4)//五箱之中的最后第二箱开始
				{ /*强行标记为推出*/
					BOMB_CLAW_PUSH;
					flag_bomb_claw_record = flag_bomb_claw_push;
					/*修改平台左右目标值*/
					bomb_claw.target_entire[Y] = (bomb_claw.Y_target[R] + bomb_claw.Y_target[M])/2;
	        bomb_claw.target_zero[Y] = (bomb_claw.Y_target[L] + bomb_claw.Y_target[M])/2;				
				}
				
				
				switch(get_bomb.box_num)
				{
					case 0:
					{					
						BOMB_CLAW_MOVE_MID;
						get_bomb.get_pour_step = 0;//后面的取弹步骤清零	
						
						if(flag_push_aim_multiple_init)
						{
							flag_push_aim_multiple_init = 0;
							flag_push_aim_single_init = 1;
							flag_push_aim_scan_init = 1;
							flag_scan_safe_delay = 0;
							
							BOMB_CLAW_LOOSE;//强制张开爪子
							BOMB_CLAW_ROLL_HALF;//强制翻转爪子至HALF	
							
//							/*在岛上的时候默认取弹爪子推出 岛下的时候默认不推出*/					
//							if(climb_tube.location == ON_ISLAND) 
//							{
//								BOMB_CLAW_PUSH;
//							}							
//							else BOMB_CLAW_PULL;
							BOMB_CLAW_PULL;
						}
									
						if((abs(lifter.target_bomb_bit[L] - lifter.total_angle[L]) < 100) || 
							 (abs(lifter.target_bomb_bit[R] - lifter.total_angle[R]) < 100))//抬升到目标角度
						{ /*框架抬到了位置才执行伸出对准*/							  
							kb.bomb_claw_enable = ENABLE;//取弹爪子可控								
						  
							if(get_bomb.once_penta) flag_bomb_claw_push = 0;//取五箱的时候第一箱是不推出去的
							
							if((flag_bomb_claw_catch) && 
								((abs(bomb_claw.target_zero[L] - bomb_claw.total_angle[L]) < 100) || 
							   (abs(bomb_claw.target_zero[R] - bomb_claw.total_angle[R]) < 100)))							
							{
							  flag_bomb_claw_record = flag_bomb_claw_push;//记录是按下了推出键吗？
								get_bomb.flag_push_aim_multiple = FINISHED; //标记完成了推出对准模式
							}		
						}
						else lifter.ctrl_mode = BOMB_LIFTER_BIT;//框架抬升一点										
					}break;
					
					case 1:
					case 3:
					{
						chassis.ctrl_mode = CHASSIS_STOP;      //底盘速度环锁住
						
						kb.bomb_claw_enable = ENABLE;//取弹爪子不可控

//						BOMB_CLAW_MOVE_LEFT;							
//						if(abs(bomb_claw.target_zero[Y] - bomb_claw.total_angle[Y]) < 100)//取弹爪子挪到了位置
					  BOMB_CLAW_MOVE_RIGHT;							
						if(abs(bomb_claw.target_entire[Y] - bomb_claw.total_angle[Y]) < 100)//取弹爪子挪到了位置
						{
							BOMB_CLAW_ROLL_ZERO;
							if((abs(bomb_claw.target_zero[L] - bomb_claw.total_angle[L]) < 100) || 
								 (abs(bomb_claw.target_zero[R] - bomb_claw.total_angle[R]) < 100))//取弹爪子翻转到了位置
							{
								BOMB_CLAW_CATCH;                             //取弹爪子抓取
								get_bomb.flag_push_aim_multiple = FINISHED;  //标记完成了推出对准模式
							}
						}
						
					}break;
						
					case 2:
					case 4:
					{
						chassis.ctrl_mode = CHASSIS_STOP;      //底盘速度环锁住
						
						kb.bomb_claw_enable = ENABLE;//取弹爪子不可控										

						BOMB_CLAW_MOVE_LEFT;							
						if(abs(bomb_claw.target_zero[Y] - bomb_claw.total_angle[Y]) < 100)//取弹爪子挪到了位置
//						BOMB_CLAW_MOVE_RIGHT;							
//						if(abs(bomb_claw.target_entire[Y] - bomb_claw.total_angle[Y]) < 100)//取弹爪子挪到了位置
						{
							BOMB_CLAW_ROLL_ZERO;
							if((abs(bomb_claw.target_zero[L] - bomb_claw.total_angle[L]) < 100) || 
								 (abs(bomb_claw.target_zero[R] - bomb_claw.total_angle[R]) < 100))//取弹爪子翻转到了位置
							{
								BOMB_CLAW_CATCH;                             //取弹爪子抓
								get_bomb.flag_push_aim_multiple = FINISHED;  //标记完成了推出对准模式
							}
						}
					}break;
					
					
					default: break;					
				}											
																		
			}break;
							
			case PUSH_AIM_SCAN_MODE://扫描模式下的推出对准模式
			{	
				
			  chassis.ass_mode_enable = ENABLE;               //开启车尾模式
				chassis.force_push_in_enable = ENABLE;          //开启推进模式
				if(vision.enable == DISABLE)
				{
					chassis.ctrl_mode = CHASSIS_STOP;             //底盘速度环锁住
				}
				
				
				get_bomb.once_triple = 0;							
	      flag_bomb_claw_triple = 0;           //关掉三箱模式 让下次按ctrl+R 默认是单箱模式
					
				get_bomb.once_penta = 0;							
				flag_bomb_claw_penta = 0;            //关掉五箱模式 让下次按ctrl+R 默认是单箱模式
				
//				vision.enable = DISABLE;
//				vision.aim = HAVENT;
//				get_bomb.vision_assist = 0;	
//	      flag_bomb_claw_vision = 0;               //关掉视觉模式 让下次按ctrl+R 默认是单箱模式
				
				get_bomb.flag_push_aim_single = HAVENT;     //还没完成单箱模式下推出对准模式	
				get_bomb.flag_push_aim_multiple = HAVENT;   //还没完成三箱模式下推出对准模式
				get_bomb.flag_push_aim_scan = HAVENT;       //还没完成三箱模式下推出对准模式
				
				get_bomb.get_pour_step = 0;         //后面的取弹步骤清零	
				
				if(flag_bomb_claw_push)//如果按shift+R之前爪子是伸出去的 做个标记一会延时一下
				{
					BOMB_CLAW_PULL;//爪子收回来
					get_bomb.scan_safe_Time = millis();//记录弹射的时刻ms
					flag_scan_safe_delay = 1;//延时使能开起来 给点时间我爪子伸回来
				}
				if(flag_scan_safe_delay)
				{
					if(millis() - get_bomb.scan_safe_Time > 1000)//ms
					{
					  flag_scan_safe_delay = 0;
					}
				}
				else
				{
					if(flag_push_aim_scan_init)
					{
//					  if(servo_get_bomb_scan_delay_record)
//						{
//							servo_get_bomb_scan_delay_time = millis();
//							servo_get_bomb_scan_delay_record = 0;
//							GIM_SERVO_BACK;
//						}	
//						else if(millis() - servo_get_bomb_scan_delay_time > 500)
//						{						
							flag_push_aim_multiple_init = 1;
							flag_push_aim_single_init = 1;
							
							BOMB_CLAW_LOOSE;//强制张开爪子
							BOMB_CLAW_ROLL_HALF;//强制翻转爪子至HALF	
							
							
							lifter.target_bomb_bit[L] = lifter.target_bomb_bit_scan[L];
							lifter.target_bomb_bit[R] = lifter.target_bomb_bit_scan[R];	
							from_lifter_zero = NO;
							from_lifter_bomb_entire = YES; //修正框架目标值

							lifter.ctrl_mode = BOMB_LIFTER_BIT;//框架抬升一点(修改)
							
							if(((abs(bomb_claw.total_angle[L] - bomb_claw.target_half[L]) < 100) || 
									(abs(bomb_claw.total_angle[R] - bomb_claw.target_half[R]) < 100)) && 
								 ((abs(lifter.target_bomb_bit[L] - lifter.total_angle[L]) < 50) || 
									(abs(lifter.target_bomb_bit[R] - lifter.total_angle[R]) < 50)))			
							{	
								if(get_bomb.scan_delay_record)//延时一小段时间 让判断更准
								{
									get_bomb.scan_delay_time = millis();
									get_bomb.scan_delay_record = 0;
								}
								else if(millis() - get_bomb.scan_delay_time > 333)
								{
									if(optic_switch.value_record)//判断怼到哪里去了
									{
										optic_switch.value_start[M] = optic_switch.value[M];
										optic_switch.value_record = DISABLE;
									}												
									if(climb_tube.location == ON_ISLAND)
									{
										if(optic_switch.value_start[M])
										{
											/*怼到箱子上*/			
											if((optic_switch.value[M] == 1) && (get_bomb.scan_finished == NO)) //检测到箱子
											{
												BOMB_CLAW_MOVE_RIGHT;                //强行移到右边	
												if(abs(bomb_claw.target_entire[Y] - bomb_claw.total_angle[Y]) < 100)
												{
													get_bomb.flag_scan_failed = YES;  //标记扫描失�
													get_bomb.scan_finished = YES;
												}													
											}
											else//第一次翻转修改target_mid
											{
												bomb_claw.target_half[Y] = bomb_claw.total_angle[Y] - 21000;	
												if((bomb_claw.target_half[Y] > bomb_claw.target_entire[Y]) ||
													 (bomb_claw.target_half[Y] < bomb_claw.target_zero[Y]) ||
												    get_bomb.flag_scan_failed)
												{
													bomb_claw.target_half[Y] = bomb_claw.Y_target[M];
													get_bomb.flag_scan_failed = YES;  //标记扫描失败
												}	
												
												get_bomb.scan_finished = YES;       //标记扫描完成
												BOMB_CLAW_MOVE_MID;                 //强行移到中间
												flag_push_aim_scan_init = 0;
											}								
										}
										else
										{
											/*怼到缝里了*/
											if((optic_switch.value[M] == 0) && (get_bomb.scan_finished == NO))
											{
												BOMB_CLAW_MOVE_RIGHT;                //强行移到右边															
												if(abs(bomb_claw.target_entire[Y] - bomb_claw.total_angle[Y]) < 100)
												{
													get_bomb.flag_scan_failed = YES;  //标记扫描失败
													get_bomb.scan_finished = YES;
												}												
											}
											else //第一次翻转修改target_mid
											{
												bomb_claw.target_half[Y] = bomb_claw.total_angle[Y] + 16000;
												if((bomb_claw.target_half[Y] > bomb_claw.target_entire[Y]) ||
													 (bomb_claw.target_half[Y] < bomb_claw.target_zero[Y]) ||
												    get_bomb.flag_scan_failed)
												{
													bomb_claw.target_half[Y] = bomb_claw.Y_target[M];
													get_bomb.flag_scan_failed = YES;  //标记扫描失败
												}	
												/*下面两条语句是为了速度可调*/
												bomb_claw.from_claw_entire[Y_axis] = NO;
												bomb_claw.from_claw_zero[Y_axis] = YES;									

												get_bomb.scan_finished = YES;       //标记扫描完成
												BOMB_CLAW_MOVE_MID;                 //强行移到中间
												flag_push_aim_scan_init = 0;
											}						
										}
									}
									else if(climb_tube.location == ON_GROUND)
									{
										if(optic_switch.value_start[M] == 0)
										{
											/*只有怼到箱子上 两挡板之间才能触发单箱扫描*/			
											if((optic_switch.value[M] == 0) && (get_bomb.scan_finished == NO)) //检测到箱子
											{
												BOMB_CLAW_MOVE_RIGHT;                //强行移到右边				
												if(abs(bomb_claw.target_entire[Y] - bomb_claw.total_angle[Y]) < 100)
												{
													get_bomb.flag_scan_failed = YES;  //标记扫描失败
													get_bomb.scan_finished = YES;
												}												
											}
											else //第一次翻转修改target_mid
											{
												bomb_claw.target_half[Y] = bomb_claw.total_angle[Y] - 8500;	
												if((bomb_claw.target_half[Y] > bomb_claw.target_entire[Y]) ||
													 (bomb_claw.target_half[Y] < bomb_claw.target_zero[Y]) ||
												    get_bomb.flag_scan_failed)
												{
													bomb_claw.target_half[Y] = bomb_claw.Y_target[M];
													get_bomb.flag_scan_failed = YES;  //标记扫描失败
												}	
												
												get_bomb.scan_finished = YES;       //标记扫描完成
												BOMB_CLAW_MOVE_MID;                 //强行移到中间
												flag_push_aim_scan_init = 0;
											}								
										}
										else get_bomb_clear_record = 1;//启动退出重来程序
									}							
								}
							}							
//						}
					}	
				}
				
        if(get_bomb.scan_finished)				
				{		
		      /*修正框架目标值*/ 
          if(lifter.target_bomb_bit[L] != lifter.target_bomb_bit_normal[L] || 
						 lifter.target_bomb_bit[R] != lifter.target_bomb_bit_normal[R])		
	       	{					
						lifter.target_bomb_bit[L] = lifter.target_bomb_bit_normal[L];
						lifter.target_bomb_bit[R] = lifter.target_bomb_bit_normal[R];
						if((lifter.total_angle[L] > lifter.target_bomb_bit[L]) && 
							 (lifter.total_angle[R] < lifter.target_bomb_bit[R]))
						{
							from_lifter_zero = YES;
							from_lifter_bomb_entire = NO; 			
						}
						else if((lifter.total_angle[L] < lifter.target_bomb_bit[L]) && 
										(lifter.total_angle[R] > lifter.target_bomb_bit[R]))
						{
							from_lifter_zero = NO;
							from_lifter_bomb_entire = YES; 			
						}
					}
					
					if((abs(lifter.target_bomb_bit[L] - lifter.total_angle[L]) < 100) || 
						 (abs(lifter.target_bomb_bit[R] - lifter.total_angle[R]) < 100))
					{
					  kb.bomb_claw_enable = ENABLE;//取弹爪子可控	
            GIM_SERVO_SIDE;						

						if(climb_tube.location == ON_ISLAND && !get_bomb.flag_scan_failed)
						{
							 BOMB_CLAW_PUSH;
						}
//						else BOMB_CLAW_PULL;

						if((flag_bomb_claw_catch) && 
						((abs(bomb_claw.target_zero[L] - bomb_claw.total_angle[L]) < 100) || 
						 (abs(bomb_claw.target_zero[R] - bomb_claw.total_angle[R]) < 100)))							
						{								
							get_bomb.flag_push_aim_scan = FINISHED;//标记完成了推出对准模式
						}					
					}	
					
				}		
				
			}break;				
					
			case GET_POUR_MODE://取弹倒弹模式
			{		
			  chassis.ass_mode_enable = ENABLE;      //开启车尾模式
				chassis.force_push_in_enable = ENABLE; //开启推进模式
				chassis.ctrl_mode = CHASSIS_STOP;      //底盘速度环锁住
				vision.enable = DISABLE;
				flag_bomb_claw_vision = 0;		
				
				if(get_bomb.get_pour_step == 0)//step 0
				{	 
					kb.bomb_claw_enable = DISABLE;        //取弹爪子不可控	
					lifter.ctrl_mode = BOMB_LIFTER_ENTIRE;//框架抬升拔出箱子	
					if((abs(lifter.target_bomb_entire[L] - lifter.total_angle[L]) < 100) || 
						 (abs(lifter.target_bomb_entire[R] - lifter.total_angle[R]) < 100))//抬升到目标角度
					{ /*框架抬到了位置才执行倒弹*/
						BOMB_CLAW_PULL;                    //取弹爪子收回
						/*修正中心值*/	
						if(bomb_claw.target_half[Y] > bomb_claw.Y_target[M])
						{
							bomb_claw.from_claw_entire[Y_axis] = YES;
							bomb_claw.from_claw_zero[Y_axis] = NO;
						}	
						else if(bomb_claw.target_half[Y] < bomb_claw.Y_target[M])
						{
							bomb_claw.from_claw_entire[Y_axis] = NO;
							bomb_claw.from_claw_zero[Y_axis] = YES;
						}													
						bomb_claw.target_half[Y] = bomb_claw.Y_target[M];  
						
						BOMB_CLAW_MOVE_MID;                //取弹爪子移到中间
						BOMB_CLAW_ROLL_BIT;                //取弹爪子翻转到BIT
						get_bomb.get_pour_step ++;
					}		
				}							
						
			  else if(get_bomb.get_pour_step == 1)//step 1
				{
//					if((abs(bomb_claw.target_half[L] - bomb_claw.total_angle[L]) < 100) || 
//						 (abs(bomb_claw.target_half[R] - bomb_claw.total_angle[R]) < 100))							
//					{
					lifter.ctrl_mode = BOMB_LIFTER_BIT;//框架回到取弹那个位置							
//					}
							
					if((abs(bomb_claw.target_bit[L] - bomb_claw.total_angle[L]) < 100) || 
						 (abs(bomb_claw.target_bit[R] - bomb_claw.total_angle[R]) < 100))							
					{										
//						BOMB_CLAW_LOOSE;                    //取弹爪子松开
						get_bomb.claw_loose_Time = millis();//记录爪子松开的时刻ms
						get_bomb.get_pour_step ++;
					}								
				}
						
				else if(get_bomb.get_pour_step == 2)//step 2
				{
					if((millis() - get_bomb.claw_loose_Time > 200) && 
						 (millis() - get_bomb.claw_loose_Time < 350))//ms
					{
						BOMB_CLAW_LOOSE;                    //取弹爪子松开
					}
					else if((millis() - get_bomb.claw_loose_Time > 350) && 
						      (millis() - get_bomb.claw_loose_Time < 500))//ms
					{
						BOMB_CLAW_ROLL_HALF;                //
					}					
					else if(millis() - get_bomb.claw_loose_Time > 500)//ms
					{
						BOMB_EJECT_PUSH;                    //弹射推出
						get_bomb.eject_push_Time = millis();//记录弹射的时刻ms
						get_bomb.get_pour_step ++;
					}
				}
						
				else if(get_bomb.get_pour_step == 3)//step 3
				{
					if(millis() - get_bomb.eject_push_Time > 350)//ms
					{
						get_bomb.get_pour_step = 0;//取弹步骤清零		
						
						get_bomb.flag_push_aim_single = HAVENT;    //还没完成推出对准模式											
						get_bomb.flag_push_aim_multiple = HAVENT;  //还没完成推出对准模式	
						get_bomb.flag_push_aim_scan = HAVENT;      //还没完成推出对准模式	
											
						BOMB_EJECT_PULL;        //弹射收回								
						flag_bomb_claw_roll = 0;//使退出后再进入PUH_AIM后ENABLE时爪子能自动HALF
								
						/*在岛上的时候默认取弹爪子推出 岛下的时候默认不推出*/
//						if(((climb_tube.location == ON_ISLAND) || (flag_bomb_claw_record  == 1)) && (get_bomb.box_num != 2))//岛上或是记录了推出
						if(flag_bomb_claw_record  == 1 && get_bomb.box_num != 2)//岛上或是记录了推出
						{
							flag_bomb_claw_push = 1;		
						}				
						else//岛下 
						{
							flag_bomb_claw_push = 0;//单箱模式下默认不推出	
              flag_bomb_claw_record  = 0;							
						}
						
						
						if(get_bomb.once_triple)/*三箱都取完之后再关掉三箱模式*/
						{							
							if(get_bomb.box_num < 2) 
							{
								get_bomb.box_num ++;
							}
							else 
							{
								get_bomb.box_num = 0;	
								
                get_bomb.once_triple = 0;	
                flag_bomb_claw_triple = 0; //关掉三箱模式								
							}						
						}		
						else if(get_bomb.once_penta)/*五箱都取完之后再关掉五箱模式*/
						{							
							if(get_bomb.box_num < 4) 
							{
								get_bomb.box_num ++;
							}
							else 
							{
								get_bomb.box_num = 0;	
								
                get_bomb.once_penta = 0;	
                flag_bomb_claw_penta = 0; //关掉五箱模式		

								flag_bomb_claw_push = 0;//单箱模式下默认不推出	
								flag_bomb_claw_record  = 0;									
							}						
						}	
						
            /*关于扫描的一些东西*/						
//						if(get_bomb.scan)
//						{		
							get_bomb.scan = 0;	
						  get_bomb.flag_scan_failed = NO;
						  optic_switch.value_record	= ENABLE;	
              get_bomb.scan_delay_record = ENABLE;							
	            get_bomb.scan_finished = NO;	
              flag_bomb_claw_scan = 0; //关掉扫描模式		
//						}
						
					}					
				}
					
			}break;
				
//			case VISION_ASSIST_MODE://视觉对位模式
//			{								
//				vision.enable = ENABLE;
//				if(vision.aim == FINISHED)
//				{
//					vision.enable = DISABLE;
//					vision.aim = HAVENT;
//					get_bomb.vision_assist = 0;	
//					flag_bomb_claw_vision = 0;               //关掉视觉模式 让下次按ctrl+R 默认是单箱模式		
//				}	

//			}break;			
					
			case SUSPEND_MODE://阻塞态模式
			{	
        //do nothing 				
			}break;
			default: break;		
		}		
	}
	
}

void get_bomb_task(void)
{
//	if(bomb_claw.ALL_reset)
//	{
		get_bomb_ctrl();
//	}
}


