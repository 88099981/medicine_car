/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               东秦ai视觉一队 编写                 00**
**00                  作者：刘坤龙                     00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            函数作用：  go                          00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/
#include "aicar_gogogo.h"
#include "headfile.h"

void aicar_gogogo()
{
    DisableGlobalIRQ();
    board_init();           //务必保留，本函数用于初始化MPU 时钟 调试串口
    systick_delay_ms(1000);

    aicar_init();
    //aicar_switch_get();跑车前选状态
    pit_init();                     //初始化pit外设
    pit_interrupt_ms(PIT_CH0,10);  //初始化pit通道0 周期
    NVIC_SetPriority(PIT_IRQn,5);  //设置中断优先级 范围0-15 越小优先级越高 四路PIT共用一个PIT中断函数
    servo_duty=3850;
    
    //初始化区，所有都是动态的，这里给一个初始化，后面可以移到init文件
    kp_l=KP_motor_left;
    ki_l=KI_motor_left;
    kp_r=KP_motor_right;
    ki_r=KI_motor_right;
    kp_ad=KP_ad_str;
    kd_ad=KD_ad_str;
    //a_cam=A_cam;//二次项系数，还不好用
    kp_cam=KP_cam;
    kd_cam=KD_cam;
    //ostu_thres=203;//阈值

    EnableGlobalIRQ(0); //总中断最后开启
    aicar_meau_init();
    while(1)
    {
        aicar_meau_scan();
        show_page(pointer_page);
        show_arrow(pointer_arrow);
        
        
//        lcd_showstr(0,1,"temp1:");    
//        lcd_showuint8(10*8,1,temp_buff[0]);
//        lcd_showstr(0,2,"temp2:");
//        lcd_showuint8(10*8,2,temp_buff[1]);
//        lcd_showstr(0,3,"temp3:");
//        lcd_showuint8(10*8,3,temp_buff[2]);
//        lcd_showstr(0,4,"temp4:");
//        lcd_showuint8(10*8,4,temp_buff[3]);
//        lcd_showstr(0,5,"temp5:");
//        lcd_showuint8(10*8,5,temp_buff[4]);
//        lcd_showstr(0,6,"temp6:");
//        lcd_showuint8(10*8,6,temp_buff[5]);
//        
//        uart_send = 0x0B;
//        uart_putchar(USART_4,uart_send);
    }
}