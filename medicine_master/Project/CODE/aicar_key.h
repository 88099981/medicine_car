#ifndef _aicar_key_h
#define _aicar_key_h

#include"common.h"
#include "SEEKFREE_FONT.h"

#define KEY1    C31
#define KEY2    C27
#define KEY3    C26
#define KEY4    C4
//���岦�뿪������
#define SW1     D4
#define SW2     D27


//���뿪��״̬����
extern uint8 sw1_status;
extern uint8 sw2_status;

//����״̬����
extern uint8 key1_status;
extern uint8 key2_status;
extern uint8 key3_status;
extern uint8 key4_status;

//��һ�ο���״̬����
extern uint8 key1_last_status;
extern uint8 key2_last_status;
extern uint8 key3_last_status;
extern uint8 key4_last_status;

//���ر�־λ
extern uint8 key1_flag;
extern uint8 key2_flag;
extern uint8 key3_flag;
extern uint8 key4_flag;

void aicar_key_init();
void aicar_switch_init();
void aicar_key_get();
void aicar_switch_get();

#endif