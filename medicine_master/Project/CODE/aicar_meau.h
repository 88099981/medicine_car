#ifndef _aicar_meau_h
#define _aicar_meau_h
#include "common.h"

extern uint8 meau_page;
extern uint8 pointer_page;
extern uint8 pointer_arrow;
extern int16 sancha_wait_in;
extern vuint8 sancha_wait_banma;
extern vuint8 apriltag_wait_banma;
extern vuint8 huandao_wait_banma;

typedef enum
{
    MEAU_MAIN,
    MEAU_DEBUG,//debug���˵�
    MEAU_DEBUG_0,//����ͷ��ʾͼ��
    MEAU_DEBUG_1,//�����ʾֵ
    MEAU_DEBUG_2,//����ʾ������ͼ��
    MEAU_DEBUG_3,//���pid
    MEAU_DEBUG_4,//���pid
    MEAU_DEBUG_5,//����
    MEAU_DEBUG_6,//����
    MEAU_DEBUG_7,//������20602
    MEAU_DEBUG_8,//apriltagsʶ��
    MEAU_DEBUG_9,//numberʶ��
    MEAU_PARA,//������ʾ
    MEAU_PARA_0,//��������
    MEAU_PARA_0_0,//ruhuan
    MEAU_PARA_0_1,//chuhuan
    MEAU_PARA_0_2,//ruhuan_delay
    MEAU_PARA_0_3,//chuhuan_delay
    MEAU_PARA_0_4,//ruhuan_turn
    MEAU_GOGOGO,
    MEAU_GOGOGO_0,//ģʽѡ��
    MEAU_GOGOGO_1,//���ҳ���
    MEAU_GOGOGO_2,//magic
    MEAU_GOGOGO_3,//apriltag
    MEAU_GOGOGO_4,//number
    MEAU_GOGOGO_5,//animal_fruit
    MEAU_GOGOGO_6,//GOGOGO!
    MEAU_OURTEAM,
    MEAU_NON,//����ʾ
    
}MEAU_NUM;

typedef enum
{
    GOGOGO_ADC,//���ģʽ
    GOGOGO_CAM,//����ͷģʽ
    GOGOGO_MIX,//���ģʽ
    GOGOGO_MAGIC,//����ģʽ
    GOGOGO_NON//����ʾ
}RUN_MODE;

typedef enum
{
    GARAGE_LEFT,//�����
    GARAGE_RIGHT,//�ҳ���
    GARAGE_NON,//������
}GARAGE_MODE;
void aicar_meau();
void show_page(uint8);
void show_arrow(uint8);
void debug_camera();
void debug_adc();
void debug_wireless();
void debug_servo();
void debug_motor();
void debug_huandao();
void debug_chasu();
void debug_20602();
void debug_apriltag();
void debug_number();
void debug_encoder();
void debug_around();
void debug_l_90();
void debug_r_90();
void debug_error();
void gogogo_1();
void gogogo_2();
void gogogo_3();
void gogogo_4();
void gogogo_camera();
void gogogo_adc();
void gogogo_mix();
void gogogo_choose();
void gogogo_run();
void para_huandao();

#endif