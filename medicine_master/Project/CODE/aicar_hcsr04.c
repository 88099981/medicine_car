/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00                  ������ҩС��                     00**
**00                  ���ߣ�������                     00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            �������ã�  hcsr04                     00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/ 
#include "aicar_hcsr04.h"
#include "zf_gpio.h"
#include "zf_pit.h"
#include "zf_systick.h"

long wait_time = 0;                //��ʱ������  
uint32 distance_time = 0;          //����ʱ�����  
uint32 distance = 0;               //ת�������ľ���  ��λΪ����  

void hcsr04_init(void)  
{  
    //��ʼ��IO�˿�Ϊ���ģʽ������  
    gpio_init(TRIG_PIN, GPO, 0, GPIO_PIN_CONFIG);  
    //��ʼ��IO�˿�Ϊ���ģʽ������  
    gpio_init(ECHO_PIN, GPI, 0, GPIO_PIN_CONFIG);  
    //ʹ��pit �����ȳ�ʼ�� Ȼ���ڵ���pit��������ʹ�ö�Ӧ�Ĺ���   
}  

void hcsr04_wait(void)
{
    gpio_set(TRIG_PIN,1);                           //������������ߵ�ƽ  
    systick_delay_us(10);                           //��ʱ10us      
    gpio_set(TRIG_PIN,0);  
    
    while(!gpio_get(ECHO_PIN));                     //��⵽��������Ϊ�ߵ�ƽ��ʼ��ʱ  
    pit_start(PIT_CH1);  
    wait_time = 0;  
    while(gpio_get(ECHO_PIN))                       //��⵽��������Ϊ�͵�ƽ�������ʱ  
    {  
        wait_time++;                                //��ʱ���  
        systick_delay(5);  
        if(wait_time>200000) break;                 //�����ȴ�ʱ��  
    }  

    if(wait_time<200000) distance_time = pit_get_us(PIT_CH1); //��ȡ��ʱ��ʱ��  

    else                  distance_time = 200000;  

    pit_close(PIT_CH1);                             //�رն�ʱ���������ʱ  
    distance = distance_time*340/2/1000;            //�������  ��λ����  

    systick_delay_ms(10); 
    
}