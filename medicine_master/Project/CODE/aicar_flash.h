#ifndef _aicar_flash_h
#define _aicar_flash_h

#include"common.h"
//����flash���һ������������������ͻ
//�洢������ô����һ��������ʼʹ��
#define EXAMPLE_FLASH_SECTOR        (FLASH_SECTOR_NUM-1)

//��������������ҳ���
#define EXAMPLE_FLASH_SECTOR_PAGE   (0)

#define FLASH_SAVE_NUM  5//�궨����Ҫ�洢�����ĸ���    һ��ҳ���Ϊ256�ֽڣ�һ��������Ҫռ4���ֽڣ�����������Ϊ256/4=64
extern uint32 write_buf[FLASH_SAVE_NUM];
extern uint32 read_buf[FLASH_SAVE_NUM];

void aicar_flash_read();
void aicar_flash_write();
void aicar_flash_init();

#endif