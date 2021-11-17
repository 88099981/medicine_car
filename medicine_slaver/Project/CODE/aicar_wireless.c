/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               东秦ai视觉一队 编写                 00**
**00                  作者：刘坤龙                     00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            函数作用：wireless                        00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/
#include "aicar_wireless.h"
#include "zf_uart.h"
#include "Serial_oscilloscope.h"
#include "SEEKFREE_MT9V03X.h"
#include "aicar_init.h"
#include "aicar_motor.h"
#include "aicar_uart.h"


uint8   double_buff[LINE_LEN]={0};            //主机用于接收数据的BUFF
uint8 double_uart_num = 0;


void aicar_pid_wireless(void)
{
    encoder_str[0]=encoder1;
    encoder_str[1]=encoder2;
    encoder_str[2]=(uint16)speed_out1/10;
    encoder_str[3]=(uint16)speed_out2/10;
    encoder_str[4]=(uint16)aim_speed/10;
    encoder_str[5]=(uint16)kp_l*100;
    encoder_str[6]=(uint16)ki_l*100;
    encoder_str[7]=0;
    Data_Send(WIRELESS_UART, encoder_str);
}

void aicar_camera_wireless(UARTN_enum uartn, uint8 *image, uint16 width, uint16 height)
{
    uart_putchar(uartn,0x01);uart_putchar(uartn,0xFE);//发送帧头
    uart_putbuff(uartn, image, width*height);  //发送图像
    uart_putchar(uartn,0xFE);uart_putchar(uartn,0x01);//发送帧尾
}

void double_get_slave_data(uint8 data)
{
    double_buff[double_uart_num++] = data;

    if(1 == double_uart_num)
    {
        //接收到的第一个字符不为0xA1，帧头错误
        if(0xA1 != double_buff[0])
        {
            double_uart_num = 0;
            uart_flag = E_FRAME_HEADER_ERROR;
        }
    }

    if(DOLUBLE_LINE_LEN == double_uart_num)
    {
        //接收到最后一个字节为0xEE
        if(0xB3 == double_buff[DOLUBLE_LINE_LEN - 1])
        {
            uart_flag = E_OK;
        }
        else    //接收到最后一个字节不是0xB3，帧尾错误
        {
            uart_flag = E_FRAME_RTAIL_ERROR;
        }
        double_uart_num = 0;
    }
    
}

void double_process_data(uint8 sign)
{
    temp_buff[0] = 0xA1;                         //帧头
    temp_buff[1] = sign;                         //功能字
    temp_buff[2] = 0;        //数据高8位
    temp_buff[3] = 0;      //数据低8位
    temp_buff[4] = 0;                         //功能字
    temp_buff[5] = 0xB3;

}