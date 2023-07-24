// #include <stdio.h>
// #include <fcntl.h>
// #include <stdlib.h>
#ifndef __LUX_HAL_MOTOR_H__
#define __LUX_HAL_MOTOR_H__

#ifdef __cplusplus
extern "C"
{
#endif/*__cplusplus*/

#define MOTOR_MOVE_STOP		0x0
#define MOTOR_MOVE_RUN		0x1

/* directional_attr */
#define MOTOR_DIRECTIONAL_UP	0x0
#define MOTOR_DIRECTIONAL_DOWN	0x1
#define MOTOR_DIRECTIONAL_LEFT	0x2
#define MOTOR_DIRECTIONAL_RIGHT	0x3

#define MOTOR1_MAX_SPEED	900
#define MOTOR1_MIN_SPEED	10

/* ioctl cmd */
#define MOTOR_STOP		0x1
#define MOTOR_RESET		0x2
#define MOTOR_MOVE		0x3
#define MOTOR_GET_STATUS	0x4
#define MOTOR_SPEED		0x5
#define MOTOR_GOBACK	0x6
#define MOTOR_CRUISE	0x7



#define HORIZONE_STEPS_MAX  10400
#define VERTICAL_STEPS_MAX  900

#define HALF_VERTICAL_STEPS_MAX HORIZONE_STEPS_MAX / 2 //考虑垂直电机活动范围，使用HORIZONE_STEPS_MAX做初始化
#define HALF_HORIZONE_STEPS_MAX HORIZONE_STEPS_MAX / 2 

#define MOV_ONE_STEP_SPEED  650


/*电机状态*/
typedef enum {
	MOTOR_IS_STOP,
	MOTOR_IS_RUNNING,
}LUX_HAL_MOTOR_STATUS_EN; 

/*电机运动模式*/
typedef enum
{
	LUX_HAL_MOTOR_MODES_STOP,			
    LUX_HAL_MOTOR_MODES_CRUISE,      /*巡航*/
	LUX_HAL_MOTOR_MODES_GOBACK,		/*复位*/
}LUX_HAL_MOTOR_MODE_EN;

/*电机的状态信息*/
typedef struct {
	int x;
	int y;
	LUX_HAL_MOTOR_STATUS_EN status;
	int speed;
}LUX_HAL_MOTOR_MSG_ST;

typedef struct{
	int x;
	int y;
}LUX_HAL_MOTOR_STEPS_ST;


typedef enum
{
    LUX_HAL_MOTOR_UP,
    LUX_HAL_MOTOR_UP_RIGHT,
    LUX_HAL_MOTOR_UP_LEFT,
    LUX_HAL_MOTOR_LEFT,
    LUX_HAL_MOTOR_RIGHT,
    LUX_HAL_MOTOR_DOWN,
    LUX_HAL_MOTOR_DOWN_RIGHT,
    LUX_HAL_MOTOR_DOWN_LEFT,        /*电机运动的8种方向*/

}LUX_HAL_MOTOR_MOV_DIRECTION_EN;

typedef enum
{
    LUX_HAL_MOTOR_MOVE_HORIZONE,    /*水平移动*/
    LUX_HAL_MOTOR_MOVE_VERTICAL,    /*垂直移动*/
    LUX_HAL_MOTOR_MOVE_OBLIQUE,     /*斜向移动*/
    LUX_HAL_MOTOR_MOVE_BUTTON,

}LUX_HAL_MOTOR_DIRECTION_EN;


/*巡航模式*/
typedef enum
{
    LUX_HAL_MOTOR_CRUISE_ALLDAY,
    LUX_HAL_MOTOR_CRUISE_TIMING,
    LUX_HAL_MOTOR_CRUISE_STOP,
    LUX_HAL_MOTOR_CRUISE_BOTTOM,

}LUX_HAL_MOTOR_CRUISE_EN;

typedef struct  {
	unsigned int x_max_steps;
	unsigned int y_max_steps;
	unsigned int x_cur_step;
	unsigned int y_cur_step;

}LUX_HAL_MOTOR_RESET_DATA_ST;


typedef struct {
	int motor_directional;
	int motor_move_steps;
	int motor_move_speed;

}LUX_HAL_MOTOR_MOVE_ACTION_ST;


/**
 * @description: 控制电机移动方向
 * @param [in] direction  电机运动方向
 * @return 0 成功， 非零失败
 */
int LUX_HAL_Motor_CtrlMovOneStep(LUX_HAL_MOTOR_MOV_DIRECTION_EN direction);

/**
 * @description: 停止电机转动
 * @return 0 成功， 非零失败
 */
int LUX_HAL_MOTOR_Stop();

/**
 * @description: 设置PTZ巡航模式
 * @param [int]  speed  速度
 * @return 0 成功， 非零失败
 */
int LUX_HAL_MOTOR_SetCruiseMode(LUX_HAL_MOTOR_CRUISE_EN cruiseMode, int speed);

/**
 * @description: 电机移动控制初始化
 * @return 0 成功， 非零失败
 */
int LUX_HAL_Motor_CtrlInit();

/**
 * @description: 设置PTZ巡航模式
 * @param [in]  speed  速度
 * @return 0 成功， 非零失败
 */
int LUX_HAL_MOTOR_SetCruiseMode(LUX_HAL_MOTOR_CRUISE_EN cruiseMode, int speed);

/**
 * @description: 设置PTZ巡航模式启动配置
 * @return 0 成功， 非零失败
 */
int LUX_HAL_MOTOR_CruiseModeConfig(void);

/**
 * @description: 电机人形追踪功能; 根据输入计算电机速度及步数，确定电机转动状态
 * @param [in] dx, 目标中心与原点坐标水平方向上的位移
 * @param [in] dy, 目标中心与原点坐标水平方向上的位移
 * @param [in] tracking, 算法是否在跟踪状态，否则在检测状态
 * @param [in] capture_lost, 目标丢失代价，0，未丢失，>0，丢失;
 * @return 0 成功， 非零失败
 */ 
int LUX_HAL_MOTOR_PersonTracker(int dx, int dy, int tracking, int capture_lost);

#if 0

struct motor_status_st {
	int directional_attr;
	int total_steps;
	int current_steps;
	int min_speed;
	int cur_speed;
	int max_speed;
	int move_is_min;
	int move_is_max;
};

struct motor_patrol_pos{
	int x;
	int y;
	int id;
};

#endif



#ifdef __cplusplus
}
#endif/*__cplusplus*/
#endif /*__LUX_HAL_MOTOR_H__*/
