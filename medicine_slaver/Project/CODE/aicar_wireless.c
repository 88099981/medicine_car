/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               ����ai�Ӿ�һ�� ��д                 00**
**00                  ���ߣ�������                     00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            �������ã�wireless                        00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/
#include "aicar_wireless.h"
#include "zf_uart.h"
#include "Serial_oscilloscope.h"
#include "SEEKFREE_MT9V03X.h"
#include "aicar_init.h"
#include "aicar_motor.h"
#include "aicar_uart.h"


uint8   double_buff[LINE_LEN]={0};            //�������ڽ������ݵ�BUFF
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
    uart_putchar(uartn,0x01);uart_putchar(uartn,0xFE);//����֡ͷ
    uart_putbuff(uartn, image, width*height);  //����ͼ��
    uart_putchar(uartn,0xFE);uart_putchar(uartn,0x01);//����֡β
}

void double_get_slave_data(uint8 data)
{
    double_buff[double_uart_num++] = data;

    if(1 == double_uart_num)
    {
        //���յ��ĵ�һ���ַ���Ϊ0xA1��֡ͷ����
        if(0xA1 != double_buff[0])
        {
            double_uart_num = 0;
            uart_flag = E_FRAME_HEADER_ERROR;
        }
    }

    if(DOLUBLE_LINE_LEN == double_uart_num)
    {
        //���յ����һ���ֽ�Ϊ0xEE
        if(0xB3 == double_buff[DOLUBLE_LINE_LEN - 1])
        {
            uart_flag = E_OK;
        }
        else    //���յ����һ���ֽڲ���0xB3��֡β����
        {
            uart_flag = E_FRAME_RTAIL_ERROR;
        }
        double_uart_num = 0;
    }
    
}

void double_process_data(uint8 sign)
{
    temp_buff[0] = 0xA1;                         //֡ͷ
    temp_buff[1] = sign;                         //������
    temp_buff[2] = 0;        //���ݸ�8λ
    temp_buff[3] = 0;      //���ݵ�8λ
    temp_buff[4] = 0;                         //������
    temp_buff[5] = 0xB3;

}