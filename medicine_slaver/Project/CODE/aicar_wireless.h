#ifndef _aicar_wireless_h
#define _aicar_wireless_h
#include "common.h"
#include "SEEKFREE_WIRELESS.h"
#include "zf_uart.h"
#include "board.h"

#define DOLUBLE_LINE_LEN                6      //���ݳ���
extern uint8   double_buff[DOLUBLE_LINE_LEN];            //�������ڽ������ݵ�BUFF
//����
void double_get_slave_data(uint8 data);
void aicar_pid_wireless(void);
void aicar_camera_wireless(UARTN_enum uartn, uint8 *image, uint16 width, uint16 height);
void double_process_data(uint8 sign);
#endif
