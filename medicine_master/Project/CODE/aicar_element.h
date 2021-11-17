#ifndef _aicar_element_h
#define _aicar_element_h
#include"common.h"
#define HD_IN_DELAY 50
#define HD_OUT_DELAY 90
#define HD_IN 10
#define HD_OUT 10
#define HD_TURN 20

extern uint8 hd_in;
extern uint8 hd_out;
extern uint8 hd_in_delay;
extern uint8 hd_out_delay;
extern uint8 hd_turn;
extern vuint8 ruhuan_turn;
extern int32 garage_turn_out;
extern int16 servo_turn_out;
extern int32 garage_turn_in;
extern int16 servo_turn_in;
extern uint8 uart_send;

//函数
void aicar_huandao();
void aicar_left_garage_out();
void aicar_right_garage_out();
void aicar_left_garage_in();
void aicar_right_garage_in();
void holder_l_turn();
void holder_r_turn();
void servo_l_turn();
void servo_r_turn();
void wait_animal();
void shot_fruit();
void sancha_stop();
void find_apriltag();
void sort_animalorfruit();
void gogogo_stop();
void holder_turn();
void led_red_on();
void led_red_off();
void led_green_on();
void led_green_off();
void led_yellow_on();
void led_yellow_off();
void led_all_init();
void turn_around();
void turn_l_90();
void turn_r_90();

#endif