#ifndef _aicar_hcsr04_h
#define _aicar_hcsr04_h
#include "common.h"

#define TRIG_PIN B9                //���峬�����������Ŷ˿�  
#define ECHO_PIN B10               //���峬�����������Ŷ˿�  


extern uint32 distance;
void hcsr04_init(void);
void hcsr04_wait(void);

#endif