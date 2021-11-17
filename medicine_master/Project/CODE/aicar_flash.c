/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               ����ai�Ӿ�һ�� ��д                 00**
**00                  ���ߣ�������                     00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            �������ã� flash                       00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/
#include "aicar_flash.h"
#include "aicar_init.h"
#include "aicar_element.h"
#include "fsl_cache.h"
#include "zf_flash.h"

uint32 write_buf[FLASH_SAVE_NUM];
uint32 read_buf[FLASH_SAVE_NUM];
uint8 status;

void aicar_flash_read()
{
    flash_read_page(EXAMPLE_FLASH_SECTOR, EXAMPLE_FLASH_SECTOR_PAGE, read_buf, FLASH_SAVE_NUM);
    //�ֱ��ȡ��Ӧ����
    hd_in = read_buf[0];
    hd_out = read_buf[1];
    hd_in_delay = read_buf[2];
    hd_out_delay = read_buf[3];      //��ȡ������ʱ��Ӧ��ʹ�ú꺯����uint32����ת��Ϊfloat
    hd_turn = read_buf[4]; 
}


void aicar_flash_write()
{
    bb_time=5;
    if(flash_check(EXAMPLE_FLASH_SECTOR,EXAMPLE_FLASH_SECTOR_PAGE))//У�鵱ǰ ��������ҳ�Ƿ������ݣ�����������������������
    {
        status = flash_erase_sector(EXAMPLE_FLASH_SECTOR);//������������������Ѿ�������������������֮������ٴ�д���µ�����
        if(status)  while(1);//����ʧ��
    }
    
    write_buf[0] = hd_in;
    write_buf[1] = hd_out;
    write_buf[2] = hd_in_delay;
    write_buf[3] = hd_out_delay; 
    write_buf[4] = hd_turn;    
    //�洢����ʱ������ȡ������ַȻ����uint32 *�����ʱ�����ȡ���ݡ�
    //write_buf[3] = *(uint32 *)&write_data4; 
    //����ʹ�ô����ʽwrite_buf[3] = (uint32)write_data4;�����ᵼ��ǿ��ת��Ϊ���ͣ�����С�����ֶ�ʧ
    //Ҳ����ʹ�ÿ��ṩ�ĺ궨�� ��ȡfloat���͵Ķ���������

    
    status = flash_page_program(EXAMPLE_FLASH_SECTOR, EXAMPLE_FLASH_SECTOR_PAGE, write_buf, FLASH_SAVE_NUM);
    if(status)  while(1);//д��ʧ��

}


void aicar_flash_init()
{
    hd_in=HD_IN;
    hd_out=HD_OUT;
    hd_in_delay=HD_IN_DELAY;
    hd_out_delay=HD_OUT_DELAY;
    hd_turn=HD_TURN;
    aicar_flash_write();
}