#ifndef _aicar_uart_h
#define _aicar_uart_h

#include"common.h"

#define E_START                 0       //׼��״̬
#define E_OK                    1       //�ɹ�
#define E_FRAME_HEADER_ERROR    2       //֡ͷ����
#define E_FRAME_RTAIL_ERROR     3       //֡β����

#define LINE_LEN                6      //���ݳ���
extern uint8   temp_buff[LINE_LEN];            //�������ڽ������ݵ�BUFF
extern vuint8  uart_flag;                      //�������ݱ�־λ


extern vuint8 show_flag;                        //������ʾ��־λ
extern vuint8 test_getdata;
extern uint8 apriltag_delay;
//����
void get_slave_data(uint8 data);
void data_analysis(uint8 *line);
void aicar_uart_init();
void aicar_uart_callback();
#endif