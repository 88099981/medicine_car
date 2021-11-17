#ifndef _aicar_wireless_h
#define _aicar_wireless_h
#include "common.h"
#include "SEEKFREE_WIRELESS.h"
#include "zf_uart.h"
#include "board.h"

#define DOLUBLE_LINE_LEN                6      //数据长度
extern uint8   double_buff[DOLUBLE_LINE_LEN];            //主机用于接收数据的BUFF
//函数
void double_get_slave_data(uint8 data);
void aicar_pid_wireless(void);
void aicar_camera_wireless(UARTN_enum uartn, uint8 *image, uint16 width, uint16 height);
void double_process_data(uint8 sign);
#endif
