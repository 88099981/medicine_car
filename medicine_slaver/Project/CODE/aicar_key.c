/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               东秦ai视觉一队 编写                 00**
**00                  作者：刘坤龙                     00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            函数作用：  key                         00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/
#include "SEEKFREE_FONT.h"
#include "aicar_key.h"
#include "SEEKFREE_18TFT.h"
#include "zf_gpio.h"
#include "zf_systick.h"
#include "aicar_init.h"
#include "aicar_motor.h"

uint8 sw1_status;
uint8 sw2_status;

//开关状态变量
uint8 key1_status = 1;
uint8 key2_status = 1;
uint8 key3_status = 1;
uint8 key4_status = 1;

//上一次开关状态变量
uint8 key1_last_status;
uint8 key2_last_status;
uint8 key3_last_status;
uint8 key4_last_status;

//开关标志位
uint8 key1_flag;
uint8 key2_flag;
uint8 key3_flag;
uint8 key4_flag;

void aicar_key_init()
{
    gpio_init(KEY1,GPI,0,GPIO_PIN_CONFIG);
    gpio_init(KEY2,GPI,0,GPIO_PIN_CONFIG);
    gpio_init(KEY3,GPI,0,GPIO_PIN_CONFIG);
    gpio_init(KEY4,GPI,0,GPIO_PIN_CONFIG);
}

void aicar_switch_init()
{
    gpio_init(SW1,GPI,0,GPIO_PIN_CONFIG);
    gpio_init(SW2,GPI,0,GPIO_PIN_CONFIG);//12学习板上反了
}

void aicar_key_get()
{
    key1_last_status = key1_status;
    key2_last_status = key2_status;
    key3_last_status = key3_status;
    key4_last_status = key4_status;
   //读取当前按键状态
    key1_status = gpio_get(KEY1);
    key2_status = gpio_get(KEY2);
    key3_status = gpio_get(KEY3);
    key4_status = gpio_get(KEY4);
        
    //检测到按键按下之后  并放开置位标志位
    if(key1_status && !key1_last_status)    key1_flag = 1;
    if(key2_status && !key2_last_status)    key2_flag = 1;
    if(key3_status && !key3_last_status)    key3_flag = 1;
    if(key4_status && !key4_last_status)    key4_flag = 1;
        
    //标志位置位之后，可以使用标志位执行自己想要做的事件 
}
    
void aicar_switch_get()
{
     sw1_status = gpio_get(SW1);
     sw2_status = gpio_get(SW2);
}

