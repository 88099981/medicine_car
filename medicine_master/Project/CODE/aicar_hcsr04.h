#ifndef _aicar_hcsr04_h
#define _aicar_hcsr04_h
#include "common.h"

#define TRIG_PIN B9                //定义超声波触发引脚端口  
#define ECHO_PIN B10               //定义超声波触发引脚端口  


extern uint32 distance;
void hcsr04_init(void);
void hcsr04_wait(void);

#endif