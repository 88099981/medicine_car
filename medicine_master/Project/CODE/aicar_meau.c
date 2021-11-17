/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               东秦ai视觉一队 编写                 00**
**00                  作者：刘坤龙                     00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            函数作用：  meau                         00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/
#include "aicar_init.h"
#include "aicar_key.h"
#include "aicar_meau.h"
#include "aicar_printf.h"
#include "aicar_adc.h"
#include "SEEKFREE_MT9V03X_CSI.h"
#include "SEEKFREE_18TFT.h"
#include "camera.h"
#include "aicar_error.h"
#include "aicar_chasu.h"
#include "aicar_wireless.h"
#include "aicar_element.h"
#include "fd_GetMid.h"
#include "aicar_hcsr04.h"
#include "zf_uart.h"

//uint8 img[IMG_H][IMG_W];		//收到的图像
uint8 meau_page=0;
uint8 meau_last_page=0;
int16 sancha_wait_in=500;
vuint8 sancha_wait_banma=0;
vuint8 apriltag_wait_banma=0;
vuint8 huandao_wait_banma=0;

void show_page(uint8 page)
{
    switch(page)
    {
    case MEAU_MAIN:print_main();break;
    case MEAU_DEBUG:print_debug();break;
    case MEAU_PARA:print_parameter();break;
    case MEAU_PARA_0:para_huandao();break;
    case MEAU_PARA_0_0:para_huandao();break;
    case MEAU_PARA_0_1:para_huandao();break;
    case MEAU_PARA_0_2:para_huandao();break;
    case MEAU_PARA_0_3:para_huandao();break;
    case MEAU_PARA_0_4:para_huandao();break;    
    case MEAU_GOGOGO:print_gogogo();break;
    case MEAU_OURTEAM:print_ourteam();break;
    case MEAU_DEBUG_0:debug_around();break;
    case MEAU_DEBUG_1:debug_l_90();break;
    case MEAU_DEBUG_2:debug_r_90();break;
    case MEAU_DEBUG_3:debug_encoder();break;
    case MEAU_DEBUG_4:debug_motor();break;
    case MEAU_DEBUG_5:debug_error();break;
    case MEAU_DEBUG_6:debug_chasu();break;
    case MEAU_DEBUG_7:debug_20602();break;
    case MEAU_DEBUG_8:debug_apriltag();break;
    case MEAU_DEBUG_9:debug_number();break;
    case MEAU_GOGOGO_0:gogogo_1();break;
    case MEAU_GOGOGO_1:gogogo_2();break;
    case MEAU_GOGOGO_2:gogogo_4();break;
//    case MEAU_GOGOGO_3:gogogo_choose();break;
//    case MEAU_GOGOGO_4:gogogo_choose();break;
//    case MEAU_GOGOGO_5:gogogo_choose();break;
//    case MEAU_GOGOGO_6:gogogo_run();break;
    case MEAU_NON:break;//可删去
    }
}


void show_arrow(uint8 arrow)
{
    if(arrow!=MEAU_NON)
    {
        for(int i=0;i<10;i++)
            (i==arrow)?lcd_showstr(14*8,i,"<-"):lcd_showstr(14*8,i,"  ");
    }

}

void gogogo_1()
{
    led_green_off(); 

    uart_send = 0x0A;
    uart_putchar(USART_4,uart_send);
    temp_buff[0]=0;
    temp_buff[1]=0;
    temp_buff[2]=0;
    temp_buff[3]=0;
    temp_buff[4]=0;
    temp_buff[5]=0;
    
    //等待数字识别
    while(temp_buff[2]==0x00)
    {
        lcd_showstr(0,1,"temp0:");    
        lcd_showuint8(10*8,1,temp_buff[0]);
        lcd_showstr(0,2,"temp1:");
        lcd_showuint8(10*8,2,temp_buff[1]);
        lcd_showstr(0,3,"temp2:");
        lcd_showuint8(10*8,3,temp_buff[2]);
        lcd_showstr(0,4,"temp3:");
        lcd_showuint8(10*8,4,temp_buff[3]);
        lcd_showstr(0,5,"temp4:");
        lcd_showuint8(10*8,5,temp_buff[4]);
        lcd_showstr(0,6,"temp5:");
        lcd_showuint8(10*8,6,temp_buff[5]);
        uart_send = 0x0A;
        uart_putchar(USART_4,uart_send);
    }
    
    while(1)//等待载物
    {
        hcsr04_wait();
        lcd_showstr(0,0,"DISTANCE:");    
        lcd_showuint16(9*8,0,distance);
        if(distance>400)         break;
    }
    bb_time=20;
    
    left_motor=90;
    right_motor=91;
    sum_encoder1=0;
    sum_encoder2=0;
    while(sum_encoder1<5200||sum_encoder2<5250)
    {
        cam_error = temp_buff[1]-80;
        lcd_showstr(0,2,"sum1:");  
        lcd_showint32(9*8,2,sum_encoder1, 5);
        lcd_showstr(0,3,"sum2:");  
        lcd_showint32(9*8,3,sum_encoder2, 5);  
        lcd_showstr(0,4,"error:");  
        lcd_showint8(9*8,4, cam_error);  
        car_pid(cam_error); 
    }


    if(temp_buff[2]==0x01)          servo_l_turn();
    else if(temp_buff[2]==0x02)     servo_r_turn();
    
    bb_time=20;
    
    left_motor=120;
    right_motor=121;
    sum_encoder1=0;
    sum_encoder2=0;
    while(sum_encoder1<3500||sum_encoder2<3600)
    {
        lcd_showstr(0,2,"sum1:");  
        lcd_showint32(9*8,2,sum_encoder1, 5);
        lcd_showstr(0,3,"sum2:");  
        lcd_showint32(9*8,3,sum_encoder2, 5);  
    }
    left_motor=0;
    right_motor=0;

    vint32 time=100;
    servo_duty=4250;
    while(time)
    {
        time--;
    }
    servo_duty=3850;
    time=100;
    while(time)
    {
        time--;
    }
    servo_duty=4250;
    time=100;
    while(time)
    {
        time--;
    }
    servo_duty=3850;
    time=100;
    while(time)
    {
        time--;
    }
    servo_duty=4250;
    time=100;
    while(time)
    {
        time--;
    }
    servo_duty=3850;
    time=100;
    while(time)
    {
        time--;
    }
    
    led_red_on();
    while(1)//等待取物
    {
        hcsr04_wait();
        lcd_showstr(0,0,"DISTANCE:");    
        lcd_showuint16(9*8,0,distance);
        if(distance<50) break;
    }
    led_red_off();
    
    turn_around();

    left_motor=120;
    right_motor=121;
    sum_encoder1=0;
    sum_encoder2=0;
    while(sum_encoder1<3500||sum_encoder2<3600)
    {
        lcd_showstr(0,2,"sum1:");  
        lcd_showint32(9*8,2,sum_encoder1, 5);
        lcd_showstr(0,3,"sum2:");  
        lcd_showint32(9*8,3,sum_encoder2, 5);  
    }
    left_motor=0;
    right_motor=0;
    
    if(temp_buff[2]==0x01)          servo_r_turn();
    else if(temp_buff[2]==0x02)     servo_l_turn();
    
    left_motor=120;
    right_motor=121;
    sum_encoder1=0;
    sum_encoder2=0;
    while(sum_encoder1<5200||sum_encoder2<5350)
    {
        lcd_showstr(0,2,"sum1:");  
        lcd_showint32(9*8,2,sum_encoder1, 5);
        lcd_showstr(0,3,"sum2:");  
        lcd_showint32(9*8,3,sum_encoder2, 5);  
    }
    left_motor=0;
    right_motor=0;
    led_green_on(); 
    
    lcd_clear(BLACK);
    pointer_page=MEAU_GOGOGO;
    pointer_arrow=0;
}

void gogogo_2()
{

    uart_send = 0x0B;
    uart_putchar(USART_4,uart_send);
    temp_buff[0]=0;
    temp_buff[1]=0;
    temp_buff[2]=0;
    temp_buff[3]=0;
    temp_buff[4]=0;
    temp_buff[5]=0;
    
    //等待数字识别
    while(temp_buff[3]==0x00)
    {
        lcd_showstr(0,1,"temp0:");    
        lcd_showuint8(10*8,1,temp_buff[0]);
        lcd_showstr(0,2,"temp1:");
        lcd_showuint8(10*8,2,temp_buff[1]);
        lcd_showstr(0,3,"temp2:");
        lcd_showuint8(10*8,3,temp_buff[2]);
        lcd_showstr(0,4,"temp3:");
        lcd_showuint8(10*8,4,temp_buff[3]);
        lcd_showstr(0,5,"temp4:");
        lcd_showuint8(10*8,5,temp_buff[4]);
        lcd_showstr(0,6,"temp5:");
        lcd_showuint8(10*8,6,temp_buff[5]);
        uart_send = 0x0B;
        uart_putchar(USART_4,uart_send);
    }
    
    while(1)//等待载物
    {
        hcsr04_wait();
        lcd_showstr(0,0,"DISTANCE:");    
        lcd_showuint16(9*8,0,distance);
        if(distance>500)         break;
    }
    bb_time=20;
    
    
    //去时循迹走
    left_motor=150;
    right_motor=150;
    sum_encoder1=0;
    sum_encoder2=0;
    while(sum_encoder1<9450||sum_encoder2<9550)
    {
        cam_error = temp_buff[1]-80;
        lcd_showstr(0,2,"sum1:");  
        lcd_showint32(9*8,2,sum_encoder1, 5);
        lcd_showstr(0,3,"sum2:");  
        lcd_showint32(9*8,3,sum_encoder2, 5);  
        lcd_showstr(0,4,"error:");  
        lcd_showint8(9*8,4, cam_error);  
        car_pid(cam_error);
    }
    left_motor=0;
    right_motor=0;
    uart_send = 0x02;
    uart_putchar(USART_4,uart_send);
    
    //等待转向判断
    while(temp_buff[2]==0x00)
    {
        lcd_showstr(0,1,"temp0:");    
        lcd_showuint8(10*8,1,temp_buff[0]);
        lcd_showstr(0,2,"temp1:");
        lcd_showuint8(10*8,2,temp_buff[1]);
        lcd_showstr(0,3,"temp2:");
        lcd_showuint8(10*8,3,temp_buff[2]);
        lcd_showstr(0,4,"temp3:");
        lcd_showuint8(10*8,4,temp_buff[3]);
        lcd_showstr(0,5,"temp4:");
        lcd_showuint8(10*8,5,temp_buff[4]);
        lcd_showstr(0,6,"temp5:");
        lcd_showuint8(10*8,6,temp_buff[5]);
    }
    
        
    left_motor=90;
    right_motor=91;
    sum_encoder1=0;
    sum_encoder2=0;
    while(sum_encoder1<1850||sum_encoder2<1870)
    {
        lcd_showstr(0,2,"sum1:");  
        lcd_showint32(9*8,2,sum_encoder1, 5);
        lcd_showstr(0,3,"sum2:");  
        lcd_showint32(9*8,3,sum_encoder2, 5);  
    }
    
    
    if(temp_buff[2]==0x01)          servo_l_turn();
    else if(temp_buff[2]==0x02)     servo_r_turn();
    
    bb_time=20;
    //开始进小段路程
    left_motor=90;
    right_motor=91;
    sum_encoder1=0;
    sum_encoder2=0;
    while(sum_encoder1<3500||sum_encoder2<3600)
    {
        lcd_showstr(0,2,"sum1:");  
        lcd_showint32(9*8,2,sum_encoder1, 5);
        lcd_showstr(0,3,"sum2:");  
        lcd_showint32(9*8,3,sum_encoder2, 5);  
    }
    left_motor=0;
    right_motor=0;

    vint32 time=100;
    servo_duty=4250;
    while(time)
    {
        time--;
    }
    servo_duty=3850;
    time=100;
    while(time)
    {
        time--;
    }
    servo_duty=4250;
    time=100;
    while(time)
    {
        time--;
    }
    servo_duty=3850;
    time=100;
    while(time)
    {
        time--;
    }
    servo_duty=4250;
    time=100;
    while(time)
    {
        time--;
    }
    servo_duty=3850;
    time=100;
    while(time)
    {
        time--;
    }
    led_red_on();
    while(1)//等待取物
    {
        hcsr04_wait();
        lcd_showstr(0,0,"DISTANCE:");    
        lcd_showuint16(9*8,0,distance);
        if(distance<500) break;
    }
    led_red_off();
    
    turn_around();

    left_motor=90;
    right_motor=91;
    sum_encoder1=0;
    sum_encoder2=0;
    while(sum_encoder1<3500||sum_encoder2<3600)
    {
        lcd_showstr(0,2,"sum1:");  
        lcd_showint32(9*8,2,sum_encoder1, 5);
        lcd_showstr(0,3,"sum2:");  
        lcd_showint32(9*8,3,sum_encoder2, 5);  
    }
    left_motor=0;
    right_motor=0;
    
    if(temp_buff[2]==0x01)          servo_r_turn();
    else if(temp_buff[2]==0x02)     servo_l_turn();
    
    uart_send = 0x02;
    uart_putchar(USART_4,uart_send);
    
    left_motor=150;
    right_motor=151;
    sum_encoder1=0;
    sum_encoder2=0;
    while(sum_encoder1<9450||sum_encoder2<9550)
    {
        cam_error = temp_buff[1]-80;
        lcd_showstr(0,2,"sum1:");  
        lcd_showint32(9*8,2,sum_encoder1, 5);
        lcd_showstr(0,3,"sum2:");  
        lcd_showint32(9*8,3,sum_encoder2, 5);  
        lcd_showstr(0,4,"error:");  
        lcd_showint8(9*8,4, cam_error);  
        car_pid(cam_error);
    }
    left_motor=90;
    right_motor=91;
    sum_encoder1=0;
    sum_encoder2=0;
    while(sum_encoder1<1850||sum_encoder2<1870)
    {
        lcd_showstr(0,2,"sum1:");  
        lcd_showint32(9*8,2,sum_encoder1, 5);
        lcd_showstr(0,3,"sum2:");  
        lcd_showint32(9*8,3,sum_encoder2, 5);  
    }
    
    left_motor=0;
    right_motor=0;
    led_green_on();
    
    lcd_clear(BLACK);
    pointer_page=MEAU_GOGOGO;
    pointer_arrow=0;
}

void gogogo_3()
{
    
}

void gogogo_4()//发挥一
{
    led_green_off();    
    uart_send = 0x0D;
    uart_putchar(USART_4,uart_send);
    temp_buff[0]=0;
    temp_buff[1]=0;
    temp_buff[2]=0;
    temp_buff[3]=0;
    temp_buff[4]=0;
    temp_buff[5]=0;
    
    double_buff[0]=0;
    double_buff[1]=0;
    double_buff[2]=0;
    double_buff[3]=0;
    double_buff[4]=0;
    double_buff[5]=0;
    
    //等待数字识别
    while(temp_buff[3]==0x00)
    {
        lcd_showstr(0,1,"temp0:");    
        lcd_showuint8(10*8,1,temp_buff[0]);
        lcd_showstr(0,2,"temp1:");
        lcd_showuint8(10*8,2,temp_buff[1]);
        lcd_showstr(0,3,"temp2:");
        lcd_showuint8(10*8,3,temp_buff[2]);
        lcd_showstr(0,4,"temp3:");
        lcd_showuint8(10*8,4,temp_buff[3]);
        lcd_showstr(0,5,"temp4:");
        lcd_showuint8(10*8,5,temp_buff[4]);
        lcd_showstr(0,6,"temp5:");
        lcd_showuint8(10*8,6,temp_buff[5]);
        uart_send = 0x0D;
        uart_putchar(USART_4,uart_send);
    }
    bb_time =20;
    
    while(1)//等待载物
    {
        hcsr04_wait();
        lcd_showstr(0,0,"DISTANCE:");    
        lcd_showuint16(9*8,0,distance);
        if(distance>500)         break;
    }
    
    bb_time=20;
    
    
    //去时循迹走
    left_motor=150;
    right_motor=150;
    sum_encoder1=0;
    sum_encoder2=0;
    while(sum_encoder1<9450||sum_encoder2<9550)
    {
        cam_error = temp_buff[1]-80;
        lcd_showstr(0,2,"sum1:");  
        lcd_showint32(9*8,2,sum_encoder1, 5);
        lcd_showstr(0,3,"sum2:");  
        lcd_showint32(9*8,3,sum_encoder2, 5);  
        lcd_showstr(0,4,"error:");  
        lcd_showint8(9*8,4, cam_error);  
        car_pid(cam_error);
    }
    left_motor=0;
    right_motor=0;
    uart_send = 0x02;
    uart_putchar(USART_4,uart_send);
    
    //等待转向判断
    while(temp_buff[2]==0x00)
    {
        lcd_showstr(0,1,"temp0:");    
        lcd_showuint8(10*8,1,temp_buff[0]);
        lcd_showstr(0,2,"temp1:");
        lcd_showuint8(10*8,2,temp_buff[1]);
        lcd_showstr(0,3,"temp2:");
        lcd_showuint8(10*8,3,temp_buff[2]);
        lcd_showstr(0,4,"temp3:");
        lcd_showuint8(10*8,4,temp_buff[3]);
        lcd_showstr(0,5,"temp4:");
        lcd_showuint8(10*8,5,temp_buff[4]);
        lcd_showstr(0,6,"temp5:");
        lcd_showuint8(10*8,6,temp_buff[5]);
    }
    
        
    left_motor=90;
    right_motor=91;
    sum_encoder1=0;
    sum_encoder2=0;
    while(sum_encoder1<1850||sum_encoder2<1870)
    {
        lcd_showstr(0,2,"sum1:");  
        lcd_showint32(9*8,2,sum_encoder1, 5);
        lcd_showstr(0,3,"sum2:");  
        lcd_showint32(9*8,3,sum_encoder2, 5);  
    }
    
    
    if(temp_buff[2]==0x01)          servo_l_turn();
    else if(temp_buff[2]==0x02)     servo_r_turn();
    
    bb_time=20;
    //开始进小段路程
    left_motor=90;
    right_motor=91;
    sum_encoder1=0;
    sum_encoder2=0;
    while(sum_encoder1<3500||sum_encoder2<3600)
    {
        lcd_showstr(0,2,"sum1:");  
        lcd_showint32(9*8,2,sum_encoder1, 5);
        lcd_showstr(0,3,"sum2:");  
        lcd_showint32(9*8,3,sum_encoder2, 5);  
    }
    left_motor=0;
    right_motor=0;

    
    led_red_on();
    while(double_buff[1]!=0x22)//等待二启动
    {      
        //让小车二启动
        uart_putchar(WIRELESS_UART,0xA1);
        uart_putchar(WIRELESS_UART,0x11);
        uart_putchar(WIRELESS_UART,double_l_or_r);
        uart_putchar(WIRELESS_UART,0x00);
        uart_putchar(WIRELESS_UART,0x00);
        uart_putchar(WIRELESS_UART,0xB3);
    }
    
    vint32 time=100;
    servo_duty=4250;
    while(time)
    {
        time--;
    }
    servo_duty=3850;
    time=100;
    while(time)
    {
        time--;
    }
    servo_duty=4250;
    time=100;
    while(time)
    {
        time--;
    }
    servo_duty=3850;
    time=100;
    while(time)
    {
        time--;
    }
    servo_duty=4250;
    time=100;
    while(time)
    {
        time--;
    }
    servo_duty=3850;
    time=100;
    while(time)
    {
        time--;
    }
    while(1)//等待取物
    {
        hcsr04_wait();
        lcd_showstr(0,0,"DISTANCE:");    
        lcd_showuint16(9*8,0,distance);
        if(distance<500) break;  
    }
    led_red_off();
    uart_putchar(WIRELESS_UART,0xA1);
    uart_putchar(WIRELESS_UART,0x33);
    uart_putchar(WIRELESS_UART,double_l_or_r);
    uart_putchar(WIRELESS_UART,0x00);
    uart_putchar(WIRELESS_UART,0x00);
    uart_putchar(WIRELESS_UART,0xB3);
    
    turn_around();

    left_motor=200;
    right_motor=201;
    sum_encoder1=0;
    sum_encoder2=0;
    while(sum_encoder1<3500||sum_encoder2<3600)
    {
        lcd_showstr(0,2,"sum1:");  
        lcd_showint32(9*8,2,sum_encoder1, 5);
        lcd_showstr(0,3,"sum2:");  
        lcd_showint32(9*8,3,sum_encoder2, 5);  
    }
    left_motor=0;
    right_motor=0;
    
    if(temp_buff[2]==0x01)          servo_r_turn();
    else if(temp_buff[2]==0x02)     servo_l_turn();
    
    uart_send = 0x02;
    uart_putchar(USART_4,uart_send);
    
    left_motor=150;
    right_motor=151;
    sum_encoder1=0;
    sum_encoder2=0;
    while(sum_encoder1<9450||sum_encoder2<9550)
    {
        cam_error = temp_buff[1]-80;
        lcd_showstr(0,2,"sum1:");  
        lcd_showint32(9*8,2,sum_encoder1, 5);
        lcd_showstr(0,3,"sum2:");  
        lcd_showint32(9*8,3,sum_encoder2, 5);  
        lcd_showstr(0,4,"error:");  
        lcd_showint8(9*8,4, cam_error);  
        car_pid(cam_error);
    }
    left_motor=90;
    right_motor=91;
    sum_encoder1=0;
    sum_encoder2=0;
    while(sum_encoder1<1850||sum_encoder2<1870)
    {
        lcd_showstr(0,2,"sum1:");  
        lcd_showint32(9*8,2,sum_encoder1, 5);
        lcd_showstr(0,3,"sum2:");  
        lcd_showint32(9*8,3,sum_encoder2, 5);  
    }
    
    left_motor=0;
    right_motor=0;
    led_green_on();
    
    lcd_clear(BLACK);
    pointer_page=MEAU_GOGOGO;
    pointer_arrow=0;
}
    


void debug_error()
{
   
    while(key4_flag!=1)
    {
        aicar_key_get();//按键检测
        aicar_switch_get();//拨码开关
        
        lcd_showstr(0,5,"kp_cam:");  
        lcd_showfloat(9*8,5,kp_cam, 2,3);
        lcd_showstr(0,6,"ki_cam:");  
        lcd_showfloat(9*8,6,kd_cam, 2,3);
        if(key1_flag)   
        {
            if(sw1_status)
            {
                key1_flag = 0;//使用按键之后，应该清除标志位
                kd_cam+=0.01;                
            }
            else
            {
                key1_flag = 0;//使用按键之后，应该清除标志位
                kp_cam+=0.01;
            }
        }
                
        if(key2_flag)       
        {           
            if(sw1_status)
            {
                key2_flag = 0;//使用按键之后，应该清除标志位
                kd_cam-=0.01;
            }
            else
            {
                key2_flag = 0;//使用按键之后，应该清除标志位
                kp_cam-=0.01;
            }           
        }
        
        if(key3_flag)           
        {
            key3_flag = 0;
            left_motor = 90;
            right_motor = 90;
            
            while(key4_flag!=1)
            {  
                
                uart_send = 0x0B;
                uart_putchar(USART_4,uart_send);
                aicar_key_get();//按键检测
                
                cam_error = temp_buff[1]-80;
                lcd_showstr(0,0,"enc1:");  
                lcd_showint16(10*8,0,encoder1);
                lcd_showstr(0,1,"enc2:");  
                lcd_showint16(10*8,1,encoder2); 
                lcd_showstr(0,3,"error:");  
                lcd_showint8(9*8,3, cam_error);  
                car_pid(cam_error);
            }
            key4_flag=0;
            lcd_clear(BLACK);
            left_motor = 0;
            right_motor = 0;
            
        }
    }
    key4_flag=0;
    lcd_clear(BLACK);
    pointer_page=MEAU_DEBUG;
    pointer_arrow=5;
    
}

void debug_around()
{
    while(key4_flag!=1)
    {
        aicar_key_get();//按键检测
        aicar_switch_get();//拨码开关
        
        if(key1_flag)
        {
            key1_flag=0;
            turn_around_sum+=100;
        }
        if(key2_flag)
        {
            key2_flag=0;
            turn_around_sum-=100;
        }     
        if(key3_flag)
        {
            key3_flag=0;
            turn_around();
        }  
        lcd_showstr(0,0,"a_sum:");  
        lcd_showint32(9*8,0,turn_around_sum, 5);
         
        
    }
    key4_flag=0;
    lcd_clear(BLACK);
    pointer_page=MEAU_DEBUG;
    pointer_arrow=0;
}

void debug_r_90()
{
    while(key4_flag!=1)
    {
        aicar_key_get();//按键检测
        aicar_switch_get();//拨码开关
        
        if(key1_flag)
        {
            key1_flag=0;
            turn_r_90_sum+=100;
        }
        if(key2_flag)
        {
            key2_flag=0;
            turn_r_90_sum-=100;
        }     
        if(key3_flag)
        {
            key3_flag=0;
            turn_r_90();
        }  
        lcd_showstr(0,0,"90_sum:");  
        lcd_showint32(9*8,0,turn_r_90_sum, 5);
         
    }
    key4_flag=0;
    lcd_clear(BLACK);
    pointer_page=MEAU_DEBUG;
    pointer_arrow=2;
}

void debug_l_90()
{
    while(key4_flag!=1)
    {
        aicar_key_get();//按键检测
        aicar_switch_get();//拨码开关
        
        if(key1_flag)
        {
            key1_flag=0;
            turn_l_90_sum+=100;
        }
        if(key2_flag)
        {
            key2_flag=0;
            turn_l_90_sum-=100;
        }     
        if(key3_flag)
        {
            key3_flag=0;
            turn_l_90();
        }  
        lcd_showstr(0,0,"90_sum:");  
        lcd_showint32(9*8,0,turn_l_90_sum, 5);
         
    }
    key4_flag=0;
    lcd_clear(BLACK);
    pointer_page=MEAU_DEBUG;
    pointer_arrow=1;
}

void debug_encoder()
{
    sum_encoder1=0;
    sum_encoder2=0;
    while(key4_flag!=1)
    {
        aicar_key_get();//按键检测
        aicar_switch_get();//拨码开关
        
        lcd_showstr(0,0,"enc1:");  
        lcd_showint16(10*8,0,encoder1);
        lcd_showstr(0,1,"enc2:");  
        lcd_showint16(10*8,1,encoder2);
        lcd_showstr(0,2,"sum1:");  
        lcd_showint32(9*8,2,sum_encoder1, 5);
        lcd_showstr(0,3,"sum2:");  
        lcd_showint32(9*8,3,sum_encoder2, 5);   
        if(key4_flag)
        {
            lcd_clear(BLACK);
            pointer_page=MEAU_DEBUG;
            pointer_arrow=3;
        }
    }
    key4_flag=0;
}
void debug_camera()
{
    while(key4_flag!=1)
    {
        aicar_key_get();//按键检测
        aicar_switch_get();//拨码开关
        if(mt9v03x_csi_finish_flag)
        {      
            mt9v03x_csi_finish_flag = 0;
            cut_image_to_img2();//copy						
            binary_img();			
            Search();
            Y_Change();
            lcd_displayimage032_zoom(img[0], MT9V03X_CSI_W, MT9V03X_CSI_H, 128, 50);
            lcd_showstr(0,3,"RoundStatus:");
            lcd_showuint8(0,4,Round_Status);
        }        
        if(key1_flag)
        {
            key1_flag=0;
            ostu_thres+=1;
        }
        else if(key2_flag)
        {
            key2_flag=0;
            ostu_thres-=1;
        }
        else if(key3_flag)
        {
            key3_flag=0;
            //aicar_camera_wireless(USART_8, img[0], MT9V03X_CSI_W, MT9V03X_CSI_H);//山外上位机
            csi_seekfree_sendimg_03x(USART_8,img[0],MT9V03X_CSI_W,MT9V03X_CSI_H);//逐飞上位机
        }
        else if(key4_flag)
        {
            lcd_clear(BLACK);
            pointer_page=MEAU_DEBUG;
            pointer_arrow=0;
        }    
        if(sw1_status==1)
            aicar_camera_error();
        else
            servo_duty=3850;
    }
    key4_flag=0;
    servo_duty=3850;
}


void debug_adc()
{
    while(key4_flag!=1)
    {
        aicar_key_get();//按键检测
        aicar_switch_get();//拨码开关
        aicar_adc_get();
        aicar_adc_printf();
        if(key4_flag)
        {
            lcd_clear(BLACK);
            pointer_page=MEAU_DEBUG;
            pointer_arrow=1;
        }    
        if(sw1_status==1)
            aicar_adc_error();
        else
            servo_duty=3850;
    }
    key4_flag=0;
    servo_duty=3850;
}


void debug_wireless()
{
//    stop_cnt=0;
//    break_flag=0;
//    aim_speed=30;
//    adc_isr_enable=1;
//    while(key4_flag!=1)
//    {
//        aicar_key_get();//按键检测
//        aicar_switch_get();//拨码开关
//        if(mt9v03x_csi_finish_flag)
//        {      
//            mt9v03x_csi_finish_flag = 0;
//            cut_image_to_img2();//copy						
//            binary_img();			
//            Search();
//          
//            //aicar_camera_wireless(USART_8, img[0], MT9V03X_CSI_W, MT9V03X_CSI_H);//山外上位机
//            csi_seekfree_sendimg_03x(USART_8,img[0],MT9V03X_CSI_W,MT9V03X_CSI_H);//逐飞上位机
//        }      
//        aicar_n_chasu();
//        if(key4_flag)
//        {
//            lcd_clear(BLACK);
//            pointer_page=MEAU_DEBUG;
//            pointer_arrow=2;
//        }
//    }
//    break_flag=0;
//    key4_flag=0;
//    aim_speed=0;
//    adc_isr_enable=0;
//    servo_duty=3850;
    while(key4_flag!=1)
    {
        aicar_key_get();//按键检测
        aicar_switch_get();//拨码开关
        //aicar_adc_get();
        //aicar_servopid_printf();
        if(key1_flag&&sw2_status==1)
        {
            key1_flag=0;
            garage_turn_out+=1000;
        }
        else if(key2_flag&&sw2_status==1)
        {
            key2_flag=0;
            garage_turn_out-=1000;
        }
        else if(key1_flag&&sw2_status==0)
        {
            key1_flag=0;
            servo_turn_out+=50;
        }
        else if(key2_flag&&sw2_status==0)
        {
            key2_flag=0;
            servo_turn_out-=50;
        }
        if(key3_flag)
        {
            key3_flag=0;
            break_flag=0;
            aim_speed=SPEED_SET;
            aicar_left_garage_out();
            break_flag=1;
            
        }
        if(key4_flag)
        {
            lcd_clear(BLACK);
            pointer_page=MEAU_DEBUG;
            pointer_arrow=2;
        }   
        lcd_showstr(0,4,"turn_out:");
        lcd_showint32(10*8,4,garage_turn_out,5);
        lcd_showstr(0,5,"ser_out:");
        lcd_showint16(10*8,5,servo_turn_out);
    }
    key4_flag=0;
    servo_duty=3850;
}


void debug_servo()
{
    while(key4_flag!=1)
    {
        aicar_key_get();//按键检测
        aicar_switch_get();//拨码开关
        //aicar_adc_get();
        //aicar_servopid_printf();
        if(key1_flag&&sw2_status==1)
        {
            key1_flag=0;
            garage_turn_in+=1000;
        }
        else if(key2_flag&&sw2_status==1)
        {
            key2_flag=0;
            garage_turn_in-=1000;
        }
        else if(key1_flag&&sw2_status==0)
        {
            key1_flag=0;
            servo_turn_in+=50;
        }
        else if(key2_flag&&sw2_status==0)
        {
            key2_flag=0;
            servo_turn_in-=50;
        }
        if(key3_flag)
        {
            key3_flag=0;
            Search_Strategy=MOD3;
            stop_cnt=0;
            break_flag=0;
            aim_speed=SPEED_SET;
                        
            while(key4_flag!=1)
            {
                sancha_wait_banma=1;
                aicar_key_get();//按键检测
                aicar_switch_get();//拨码开关
                aicar_adc_get();//停车用
                if(key1_flag)
                {
                    key1_flag=0;
                    aim_speed+=10;
                }
                else if(key2_flag)
                {
                    key2_flag=0;
                    aim_speed-=10;
                }     
        //读取摄像头
                if(mt9v03x_csi_finish_flag)
                {      
                    mt9v03x_csi_finish_flag = 0;
                    mv_image_to_img2();//copy	
                    binary_img();
                    Search();
                }        
                //aicar_huandao();//环岛由摄像头给出
                if(flag_Cross==1)
                    aicar_adc_error();
                else
                    aicar_mix_error();
                
                if(sw1_status==1)
                    aicar_chasu();
                else
                    aicar_n_chasu();
                
                lcd_showstr(0,0,"sancha:");
                lcd_showuint8(10*8,0,sancha_wait_banma);
                lcd_showstr(0,1,"apriltag:");
                lcd_showuint8(10*8,1,apriltag_wait_banma);
                lcd_showstr(0,2,"huandao:");
                lcd_showuint8(10*8,2,huandao_wait_banma);
                lcd_showstr(0,3,"turn_sum:");
                lcd_showint32(10*8,3,turn_sum,5);
                lcd_showstr(0,4,"turn_in:");
                lcd_showint32(10*8,4,garage_turn_in,5);
                lcd_showstr(0,5,"ser_in:");
                lcd_showint16(10*8,5,servo_turn_in);
                lcd_showstr(0,6,"icm_gyro_z:");
                lcd_showint16(10*8,6,icm_gyro_z);
                lcd_showstr(0,7,"Garage L"); 

        //        lcd_showstr(0,9,"tag_num:");
        //        lcd_showuint8(10*6,8,temp1);
        //        lcd_showuint8(10*10,8,temp2);

        //        lcd_showstr(0,2,"bk_flag:");
        //        lcd_showuint8(12*8,2,break_flag);
        //        lcd_showstr(0,4,"kp_ad:");
        //        lcd_showfloat(12*8,4,kp_ad,3,2);
        //        lcd_showstr(0,5,"kd_ad:");
        //        lcd_showfloat(12*8,5,kd_ad,3,2);
        //        lcd_showstr(0,6,"left:");
        //        lcd_showint16(12*8,6,left_motor);
        //        lcd_showstr(0,7,"right:");
        //        lcd_showint16(12*8,7,right_motor);
                //aicar_chasu_printf();
                if(key4_flag||flag_Garage_L)
                {
                    lcd_clear(BLACK);
                    pointer_page=MEAU_GOGOGO;
                    pointer_arrow=0;
                }
            }
            break_flag=1;
            key4_flag=0;
            aim_speed=0;
            servo_duty=3850;
            
        }
        if(key4_flag)
        {
            lcd_clear(BLACK);
            pointer_page=MEAU_DEBUG;
            pointer_arrow=3;
        }   
        lcd_showstr(0,4,"turn_in:");
        lcd_showint32(10*8,4,garage_turn_in,5);
        lcd_showstr(0,5,"ser_in:");
        lcd_showint16(10*8,5,servo_turn_in);
    }
    key4_flag=0;
    servo_duty=3850;
    sancha_wait_banma=0;
    apriltag_wait_banma=0;
    huandao_wait_banma=0;
}


void debug_motor()
{
    stop_cnt=0;
    break_flag=0;
    while(key4_flag!=1)
    {
        aicar_key_get();//按键检测
        aicar_switch_get();//拨码开关
        aicar_motorpid_printf();
        aicar_pid_wireless();
        if(key1_flag)   
        {
            if(sw1_status)
            {
                key1_flag = 0;//使用按键之后，应该清除标志位
                left_motor+=5;
            }
            else
            {
                key1_flag = 0;//使用按键之后，应该清除标志位
                kp_l+=1.0;
            }
        }
                
        if(key2_flag)       
        {           
            if(sw1_status)
            {
                key2_flag = 0;//使用按键之后，应该清除标志位
                
                left_motor-=5;
            }
            else
            {
                key2_flag = 0;//使用按键之后，应该清除标志位
                kp_l-=1.0;
            }           
        }
        
        if(key3_flag)           
        {
            if(sw1_status)
            {
                key3_flag = 0;//使用按键之后，应该清除标志位
                ki_l+=1.0;
            }
            else
            {
                key3_flag = 0;//使用按键之后，应该清除标志位
                ki_l-=1.0;
            }
        }
        
        
        if(key4_flag)
        {
            lcd_clear(BLACK);
            pointer_page=MEAU_DEBUG;
            pointer_arrow=4;
        }    
        aicar_n_chasu();
    }
    key4_flag=0;
    break_flag=1;
    aim_speed=0;
    servo_duty=3850;   
}


void debug_huandao()
{
    while(key4_flag!=1)
    {
        aicar_key_get();//按键检测
        aicar_switch_get();//拨码开关
        aicar_adc_get();

        if(mt9v03x_csi_finish_flag)
        {      
            mt9v03x_csi_finish_flag = 0;
            cut_image_to_img2();//copy	
            binary_img();				   				
            Search();    
        }       
        aicar_huandao();
        aicar_huandao_printf();

        if(key1_flag)
        {
            key1_flag=0;
            zuo_yuanhuan_flag=1;
        }
        if(key2_flag)
        {
            key2_flag=0;
            you_yuanhuan_flag=1;
        }
        if(key4_flag)
        {
            lcd_clear(BLACK);
            pointer_page=MEAU_DEBUG;
            pointer_arrow=5;
        }    
        if(sw1_status==1)
            aicar_mix_error();
        else
            servo_duty=3850;
    }
    key4_flag=0;
    servo_duty=3850;
}


void debug_chasu()
{
    stop_cnt=0;
    break_flag=0;
    aim_speed=SPEED_SET;
    while(key4_flag!=1)
    {
        aicar_key_get();//按键检测
        aicar_switch_get();//拨码开关
        aicar_adc_get();//停车用
        if(key1_flag)
        {
            key1_flag=0;
            aim_speed+=10;
        }
        else if(key2_flag)
        {
            key2_flag=0;
            aim_speed-=10;
        }     
        if(mt9v03x_csi_finish_flag)
        {      
            mt9v03x_csi_finish_flag = 0;
            cut_image_to_img2();//copy	
            binary_img();				   				
            Search();    
        }        
        if(flag_Cross==1)
            aicar_adc_error();
        else
            aicar_mix_error();
       
        aicar_chasu_printf();
        if(key4_flag)
        {
            lcd_clear(BLACK);
            pointer_page=MEAU_DEBUG;
            pointer_arrow=6;
        }
    }
    break_flag=1;
    key4_flag=0;
    aim_speed=0;
    servo_duty=3850;
}

void debug_20602()
{
    turn_sum=0;
    while(key4_flag!=1)
    {
        aicar_key_get();//按键检测
        aicar_switch_get();//拨码开关
        aicar_adc_get();

        get_icm20602_accdata_spi();
        get_icm20602_gyro_spi();
        turn_sum+=icm_gyro_z;
            
        aicar_20602_printf();        
        if(key1_flag)
        {
            key1_flag=0;
        }
        if(key2_flag)
        {
            key2_flag=0;
        }
        if(key4_flag)
        {
            lcd_clear(BLACK);
            pointer_page=MEAU_DEBUG;
            pointer_arrow=7;
        }    
    }
    key4_flag=0;
    servo_duty=3850;
    
}


void debug_apriltag()
{
    find_apriltag();
    lcd_clear(BLACK);
    aim_speed=0;
    servo_duty=3850;
    pointer_page=MEAU_DEBUG;
    pointer_arrow=8;
}

void debug_number()
{
    sancha_stop();
    lcd_clear(BLACK);
    aim_speed=0;
    servo_duty=3850;
    pointer_page=MEAU_DEBUG;
    pointer_arrow=9;
}

void gogogo_camera()
{
    stop_cnt=0;
    break_flag=0;
    aim_speed=SPEED_SET;
    
    if(meau_garage_mode==GARAGE_LEFT)//车库标志位，左1右2
        aicar_left_garage_out();
    else if(meau_garage_mode==GARAGE_RIGHT)
        aicar_right_garage_out();
    
    while(key4_flag!=1)
    {
        aicar_key_get();//按键检测
        aicar_switch_get();//拨码开关
        aicar_adc_get();//停车用
        if(key1_flag)
        {
            key1_flag=0;
            aim_speed+=10;
        }
        else if(key2_flag)
        {
            key2_flag=0;
            aim_speed-=10;
        }     
        if(mt9v03x_csi_finish_flag)
        {      
            mt9v03x_csi_finish_flag = 0;
            cut_image_to_img2();//copy	
            binary_img();				   				
            Search();    
        }        
        aicar_camera_error();
        
        if(sw1_status==1)
            aicar_chasu();
        else
            aicar_n_chasu();
        lcd_showstr(0,3,"aim_speed:");
        lcd_showint16(10*8,3,aim_speed);
        lcd_showstr(0,4,"chasu:");
        lcd_showuint8(10*8,4,sw1_status);
        lcd_showstr(0,5,"RoundStatus:");
        lcd_showuint8(10*10,5,Round_Status);
        lcd_showstr(0,6,"RoundIN:");
        lcd_showuint8(10*10,6,RoundInCount);
        lcd_showstr(0,7,"RoundOUT:");
        lcd_showuint8(10*10,7,RoundOutCount);
        if(sw2_status==1)
        {
            Y_Change();
            lcd_displayimage032_zoom(img[0], MT9V03X_CSI_W, MT9V03X_CSI_H, 128, 50);
        } 
//        lcd_showstr(0,2,"bk_flag:");
//        lcd_showuint8(12*8,2,break_flag);
//        lcd_showstr(0,4,"kp_ad:");
//        lcd_showfloat(12*8,4,kp_ad,3,2);
//        lcd_showstr(0,5,"kd_ad:");
//        lcd_showfloat(12*8,5,kd_ad,3,2);
//        lcd_showstr(0,6,"left:");
//        lcd_showint16(12*8,6,left_motor);
//        lcd_showstr(0,7,"right:");
//        lcd_showint16(12*8,7,right_motor);
        //aicar_chasu_printf();
        if(key4_flag)
        {
            lcd_clear(BLACK);
            pointer_page=MEAU_GOGOGO;
            pointer_arrow=0;
        }
    }
    break_flag=1;
    key4_flag=0;
    aim_speed=0;
    servo_duty=3850;
}


void gogogo_adc()
{
    Search_Strategy=MOD3;
    stop_cnt=0;
    break_flag=0;
    aim_speed=SPEED_SET;

    if(meau_garage_mode==GARAGE_LEFT)//车库标志位，左1右2
        aicar_left_garage_out();
    else if(meau_garage_mode==GARAGE_RIGHT)
        aicar_right_garage_out();


    while(key4_flag!=1)
    {
        aicar_key_get();//按键检测
        aicar_switch_get();//拨码开关
        aicar_adc_get();//停车用
        if(key1_flag)
        {
            key1_flag=0;
            aim_speed+=10;
        }
        else if(key2_flag)
        {
            key2_flag=0;
            aim_speed-=10;
        }
//开始判断apriltag
        if(apriltag_delay!=0)   apriltag_delay--;
        if(apriltag_delay==0&&(temp_buff[1]==0x01||temp_buff[1]==0x02))//看到apriltag
        {
            find_apriltag();
            apriltag_delay=50;
        }
//三岔入环计数
        if(sancha_wait_in)
        {
            sancha_wait_in--;
            if(sancha_wait_in<=0)
                
                bb_time=100;
        }
//读取摄像头
        if(mt9v03x_csi_finish_flag)
        {
            mt9v03x_csi_finish_flag = 0;
            mv_image_to_img2();//copy
            binary_img();
            Search();
        }
        //aicar_huandao();//环岛由摄像头给出
        aicar_adc_error();

        aicar_chasu();

        lcd_showstr(0,3,"aim_speed:");
        lcd_showint16(10*8,3,aim_speed);
        lcd_showstr(0,4,"RoundStatus:");
        lcd_showuint8(10*10,4,Round_Status);
        lcd_showstr(0,5,"RoundIN:");
        lcd_showuint8(10*10,5,RoundInCount);
        lcd_showstr(0,6,"RoundOUT:");
        lcd_showuint8(10*10,6,RoundOutCount);
        if(sw2_status==1)
        {
            Y_Change();
            lcd_displayimage032_zoom(img[0], MT9V03X_CSI_W, MT9V03X_CSI_H, 128, 50);
            if(Y_Road_Status==1)
            {
                lcd_showstr(0,7,"Y_Road1");
            }
            else if(Y_Road_Status==2)
            {
                lcd_showstr(0,7,"Y_Road2");
            }
            else if(Round_Status)
            {
                lcd_showstr(0,7,"Round");
            }
            else if(flag_Garage_L || flag_Garage_R)
            {
                lcd_showstr(0,7,"Garage");
            }
            else if(flag_Cross)
            {
                lcd_showstr(0,7,"Cross");
            }
            else if(flag_AprilTag)
            {
                lcd_showstr(0,7,"A_Tag");
            }
            else
            {
                lcd_showstr(0,7,"Normal");
            }

            lcd_showstr(0,8,"Y_1:");
            lcd_showuint8(10*6,8,temp1);
            lcd_showuint8(10*10,8,temp2);
            lcd_showstr(0,9,"Y_SUM:");
            lcd_showuint16(10*5,9,sumincd.YRoad);
            }
//        else
//        {
//            uint8 temp;
//            for (uint8 i = 0; i < PIX_IMG_Y / 2; i++)
//            {
//                for (uint8 j = 0; j < PIX_IMG_X; j++)
//                {
//                    temp = copy_pix_img[i][j];
//                    copy_pix_img[i][j] = copy_pix_img[PIX_IMG_Y - i - 1][j];
//                    copy_pix_img[PIX_IMG_Y - i - 1][j] = temp;
//                }
//            }
//
//            lcd_displayimage032_zoom(*copy_pix_img,PIX_IMG_X,PIX_IMG_Y, 128, 50);
//        }


//        lcd_showstr(0,9,"tag_num:");
//        lcd_showuint8(10*6,8,temp1);
//        lcd_showuint8(10*10,8,temp2);

//        lcd_showstr(0,2,"bk_flag:");
//        lcd_showuint8(12*8,2,break_flag);
//        lcd_showstr(0,4,"kp_ad:");
//        lcd_showfloat(12*8,4,kp_ad,3,2);
//        lcd_showstr(0,5,"kd_ad:");
//        lcd_showfloat(12*8,5,kd_ad,3,2);
//        lcd_showstr(0,6,"left:");
//        lcd_showint16(12*8,6,left_motor);
//        lcd_showstr(0,7,"right:");
//        lcd_showint16(12*8,7,right_motor);
        //aicar_chasu_printf();
        if(key4_flag)
        {
            lcd_clear(BLACK);
            pointer_page=MEAU_GOGOGO;
            pointer_arrow=0;
        }
    }
    //        lcd_showstr(0,2,"bk_flag:");
    //        lcd_showuint8(12*8,2,break_flag);
    //        lcd_showstr(0,4,"kp_ad:");
    //        lcd_showfloat(12*8,4,kp_ad,3,2);
    //        lcd_showstr(0,5,"kd_ad:");
    //        lcd_showfloat(12*8,5,kd_ad,3,2);
    //        lcd_showstr(0,6,"left:");
    //        lcd_showint16(12*8,6,left_motor);
    //        lcd_showstr(0,7,"right:");
    //        lcd_showint16(12*8,7,right_motor);
            //aicar_chasu_printf();
    if (key4_flag)
    {
        lcd_clear(BLACK);
        pointer_page = MEAU_GOGOGO;
        pointer_arrow = 1;
    }
    
    break_flag = 1;
    key4_flag = 0;
    aim_speed = 0;
    servo_duty = 3850;
}


void gogogo_mix()
{
    Search_Strategy=MOD3;
    stop_cnt=0;
    break_flag=0;
    aim_speed=SPEED_SET;
    
    if(meau_garage_mode==GARAGE_LEFT)//车库标志位，左1右2
        aicar_left_garage_out();
    else if(meau_garage_mode==GARAGE_RIGHT)
        aicar_right_garage_out();

    
    while(key4_flag!=1)
    {
        aicar_key_get();//按键检测
        aicar_switch_get();//拨码开关
        aicar_adc_get();//停车用
        if(key1_flag)
        {
            key1_flag=0;
            aim_speed+=10;
        }
        else if(key2_flag)
        {
            key2_flag=0;
            aim_speed-=10;
        }     
//开始判断apriltag        
        if(apriltag_delay!=0)   apriltag_delay--;
        if(apriltag_delay==0&&(temp_buff[1]==0x01||temp_buff[1]==0x02))//看到apriltag
        {
            find_apriltag();
            apriltag_delay=50;
        }
        //三岔入环计数
        if(sancha_wait_in)
        {
            sancha_wait_in--;
            if(sancha_wait_in<=0)
            {
                bb_time=100;
                sancha_wait_in=0;
            }
                
        }
//读取摄像头
        if(mt9v03x_csi_finish_flag)
        {      
            mt9v03x_csi_finish_flag = 0;
            mv_image_to_img2();//copy	
            binary_img();
            Search();
        }        
        //aicar_huandao();//环岛由摄像头给出
        if(flag_Cross==1)
            aicar_adc_error();
        else
            aicar_mix_error();
        
        
            aicar_chasu();

        lcd_showstr(0,3,"aim_speed:");
        lcd_showint16(10*8,3,aim_speed);
        lcd_showstr(0,4,"RoundStatus:");
        lcd_showuint8(10*10,4,Round_Status);
        lcd_showstr(0,5,"RoundIN:");
        lcd_showuint8(10*10,5,RoundInCount);
        lcd_showstr(0,6,"RoundOUT:");
        lcd_showuint8(10*10,6,RoundOutCount);
        if(sw2_status==1)
        {
            Y_Change();
            lcd_displayimage032_zoom(img[0], MT9V03X_CSI_W, MT9V03X_CSI_H, 128, 50);
        }
//        else
//        {
//            uint8 temp;
//            for (uint8 i = 0; i < PIX_IMG_Y / 2; i++)
//            {
//                for (uint8 j = 0; j < PIX_IMG_X; j++)
//                {
//                    temp = copy_pix_img[i][j];
//                    copy_pix_img[i][j] = copy_pix_img[PIX_IMG_Y - i - 1][j];
//                    copy_pix_img[PIX_IMG_Y - i - 1][j] = temp;
//                }
//            }
//
//            lcd_displayimage032_zoom(*copy_pix_img,PIX_IMG_X,PIX_IMG_Y, 128, 50);
//        }
//        
//        if(Y_Road_Status==1)
//        {            
//            lcd_showstr(0,7,"Y_Road1");
//        }
//        else if(Y_Road_Status==2)
//        {
//            lcd_showstr(0,7,"Y_Road2");
//        }
//        else if(Round_Status)
//        {
//            lcd_showstr(0,7,"Round");
//        }
//        else if(flag_Garage_L || flag_Garage_R)
//        {
//            lcd_showstr(0,7,"Garage");
//        }
//        else if(flag_Cross)
//        {
//            lcd_showstr(0,7,"Cross");
//        }
//        else if(flag_AprilTag)
//        {
//            lcd_showstr(0,7,"A_Tag");
//        }
//        else
//        {
//            lcd_showstr(0,7,"Normal");
//        }
//
//        lcd_showstr(0,8,"Y_1:");
//        lcd_showuint8(10*6,8,temp1);
//        lcd_showuint8(10*10,8,temp2);
//        lcd_showstr(0,9,"Y_SUM:");
//        lcd_showuint16(10*5,9,sumincd.YRoad);
        
        
        
//        lcd_showstr(0,9,"tag_num:");
//        lcd_showuint8(10*6,8,temp1);
//        lcd_showuint8(10*10,8,temp2);

//        lcd_showstr(0,2,"bk_flag:");
//        lcd_showuint8(12*8,2,break_flag);
//        lcd_showstr(0,4,"kp_ad:");
//        lcd_showfloat(12*8,4,kp_ad,3,2);
//        lcd_showstr(0,5,"kd_ad:");
//        lcd_showfloat(12*8,5,kd_ad,3,2);
//        lcd_showstr(0,6,"left:");
//        lcd_showint16(12*8,6,left_motor);
//        lcd_showstr(0,7,"right:");
//        lcd_showint16(12*8,7,right_motor);
        //aicar_chasu_printf();
        if(key4_flag)
        {
            lcd_clear(BLACK);
            pointer_page=MEAU_GOGOGO;
            pointer_arrow=0;
        }
    }
    break_flag=1;
    key4_flag=0;
    aim_speed=0;
    servo_duty=3850;
}


void para_huandao()
{
    lcd_showstr(0,0,"in:");
    lcd_showstr(0,1,"out:");
    lcd_showstr(0,2,"in_delay:");
    lcd_showstr(0,3,"out_delay:");
    lcd_showstr(0,4,"turn:");
    lcd_showstr(0,5,"init");    
    lcd_showuint8(10*8,0,hd_in);
    lcd_showuint8(10*8,1,hd_out);
    lcd_showuint8(10*8,2,hd_in_delay);
    lcd_showuint8(10*8,3,hd_out_delay);
    lcd_showuint8(10*8,4,hd_turn);    
}

void gogogo_choose()
{
    lcd_showstr(0,0,"run_mode:");
    lcd_showstr(0,1,"garage:");
    lcd_showstr(0,2,"magic");
    lcd_showstr(0,3,"apriltag");
    lcd_showstr(0,4,"servo");
    lcd_showstr(0,5,"ani_or_fru");  
    lcd_showstr(0,6,"GOGOGO!");
    switch(meau_run_mode)
    {
    case GOGOGO_ADC: 
        lcd_showstr(10*8,0,"ADC  ");break;
    case GOGOGO_MIX:
        lcd_showstr(10*8,0,"MIX  ");break;
    case GOGOGO_CAM:
        lcd_showstr(10*8,0,"CAM  ");break;
    }
    switch(meau_garage_mode)
    {
    case GARAGE_LEFT: 
        lcd_showstr(10*8,1,"L  ");break;
    case GARAGE_RIGHT:
        lcd_showstr(10*8,1,"R  ");break;
    case GARAGE_NON:
        lcd_showstr(10*8,1,"NON");break;
    }
    if(magic_mode==1)
    {
        lcd_showstr(10*8,2,"ON");
    }
    else if(magic_mode==0)
    {           
        lcd_showstr(10*8,2,"OFF");
    }
}

void gogogo_run()
{
    switch(meau_run_mode)
    {
    case GOGOGO_ADC: 
        gogogo_adc();break;
    case GOGOGO_MIX:
        gogogo_mix();break;
    case GOGOGO_CAM:
        gogogo_camera();break;
    }
}
