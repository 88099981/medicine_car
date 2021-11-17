#ifndef _aicar_init_h
#define _aicar_init_h
#include "common.h"

#define BEEP_PIN   B11       //定义蜂鸣器引脚
#define EPSOLON 1e-7
#define SPEED_SET 15

typedef enum
{
    HOLDER_LEFT,
    HOLDER_RIGHT,
    SERVO_LEFT,
    SERVO_RIGHT,
    ANIMAL,
    FRUIT
}KIND;

extern vuint8 zuo_yuanhuan_flag;
extern vuint8 you_yuanhuan_flag;
extern vint16 left_motor, right_motor;
extern uint16 bb_time;
extern int16 encoder1,encoder2;
extern uint16 encoder_str[];
extern int16 aim_speed_set;
extern int16 aim_speed;
extern uint32 servo_duty;
extern vuint8 break_flag;
extern vuint8 adc_isr_enable;
extern uint16 stop_cnt;
extern int64 turn_sum;
extern float chasu_k,chasu_b;
extern vuint8 magic_mode;
extern uint8 magic_data[];
extern uint16 use_time;

extern int64 sum_encoder1;
extern int64 sum_encoder2;

extern int32 turn_around_sum;
extern int32 turn_l_90_sum;
extern int32 turn_r_90_sum;

//与上位机通信的标志位
extern vuint8 number_ready;
extern vuint8 cross_ready;
extern int16 cam_error;
extern vuint8 double_l_or_r;

extern vuint8 up_side_down;
void aicar_init();

#endif