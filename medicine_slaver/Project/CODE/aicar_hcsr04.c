/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00                  智能送药小车                     00**
**00                  作者：刘坤龙                     00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            函数作用：  hcsr04                     00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/ 
#include "aicar_hcsr04.h"
#include "zf_gpio.h"
#include "zf_pit.h"
#include "zf_systick.h"

long wait_time = 0;                //超时检测变量  
uint32 distance_time = 0;          //测距的时间变量  
uint32 distance = 0;               //转换出来的距离  单位为厘米  

void hcsr04_init(void)  
{  
    //初始化IO端口为输出模式且上拉  
    gpio_init(TRIG_PIN, GPO, 0, GPIO_PIN_CONFIG);  
    //初始化IO端口为输出模式且上拉  
    gpio_init(ECHO_PIN, GPI, 0, GPIO_PIN_CONFIG);  
    //使用pit 必须先初始化 然后在调用pit其他函数使用对应的功能   
}  

void hcsr04_wait(void)
{
    gpio_set(TRIG_PIN,1);                           //触发引脚输出高电平  
    systick_delay_us(10);                           //延时10us      
    gpio_set(TRIG_PIN,0);  
    
    while(!gpio_get(ECHO_PIN));                     //检测到接收引脚为高电平则开始计时  
    pit_start(PIT_CH1);  
    wait_time = 0;  
    while(gpio_get(ECHO_PIN))                       //检测到接收引脚为低电平则结束计时  
    {  
        wait_time++;                                //超时检测  
        systick_delay(5);  
        if(wait_time>200000) break;                 //超过等待时间  
    }  

    if(wait_time<200000) distance_time = pit_get_us(PIT_CH1); //获取定时的时间  

    else                  distance_time = 200000;  

    pit_close(PIT_CH1);                             //关闭定时器以清除计时  
    distance = distance_time*340/2/1000;            //计算距离  单位毫米  

    systick_delay_ms(10); 
    
}