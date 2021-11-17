#ifndef _FD_GETMID_H__
#define _FD_GETMID_H__

#include "headfile.h"

#define IMG_DEBUG   //ͼ����Ա�־�����ڼ���ĳЩ������������ʾ

#define Black (0x00)
#define White (0xff)
#define Gray  (0x80)
#define Mark_Lane (0x60)
#define Mark_AprilTag (0xa0)
#define Mark_YRoad (0xc0)
#define IMG_X 188 //ͼ������С
#define IMG_Y 50 //ͼ�������С
#define PIX_IMG_X IMG_X/3
#define PIX_IMG_Y IMG_Y/3
#define Y_LIMIT 4   //���������߽�
#define X_LIMIT 4   //���������߽�
#define EDGE_MAX IMG_Y//�߽������С
#define MID_RESOLUTION 20   //���߽����� ԽСԽ�ӽ�ԭʼ����
#define CUR_RESOLUTION 10   //�ǶȽ����� ������ɾȥ��������������������

#define ROUND_ARM_COUNT_TIMES 20

#define WIDTH_BASE 80 //��ײ�������ȱ궨ֵ
#define WIDTH_K 0.98    //��խ����

//struct declear
typedef struct
{
    int16 Lx;
    int16 Rx;
}EDGE;

typedef struct
{
    float L;
    float R;
}ANGLE;

typedef struct 
{
    int8 offset_x;
    int8 offset_y;
}DIRECTION;

typedef struct
{
    uint16 Lane;
    uint16 AprilTag;
    uint16 YRoad;
}SUMINCD;

typedef enum
{
    MOD1=1,
    MOD2=2,
    MOD3=3,
}SEARCH_STRATEGY;

//function
void init(void);    //���ֱ�����ʼ������
void Y_Change(void);    //�ĳ�һ���ֱ������ϵ��ʽ��Ҫ��Ȼ����ˬ
float Cp_sqrt(float number);    //���ٿ�����
float Get_Angle(uint8 p1_y,uint8 p2_y,uint8 Target_y,uint8 l_or_r);
uint8 Set_Edge_BySlope(uint8 p1_y,uint8 p2_y,uint8 target_p_y,uint8 l_or_r);
uint8 Connect(EDGE Target[],uint8 l_or_r,uint8 p1_y,uint8 p2_y);
uint8 Mid_Connect(int16 Target[],uint8 p1_y,uint8 p2_y);
uint8 Hor_Search_Base(uint8 MidStart,uint8 y);
uint8 Hor_Search(uint8 MidStart,uint8 Y);    //ˮƽɨ�� MidStartΪ����ɨ����ʼ�е� yΪɨ���Ӧ�ĸ߶�
uint8 zoomin(float weight,uint8 PIX_DeBUG); //����Ч��
uint8 Ver_Search(uint8 Midstart);   //�Ӹ����е㴹ֱɨ�赽����
uint8 Uni_Ver_Search(uint8 MidStart);
uint8 Feature_Verify(uint8 T_x,uint8 T_y,uint8 dx,uint8 dy,uint8 *feature);    //�����ȽϺ����������������ͼ���Ӧλ�ý��бȽϣ��������ƶ�(0~100)]
uint8 Feature_Verify_Color(uint8 T_x,uint8 T_y,uint8 dx,uint8 dy,uint8 color,float expect_rate);
uint8 Feature_Verify_Box(uint8 T_x,uint8 T_y,uint8 dx,uint8 dy,uint8 thickness,uint8 white_or_black);
void Copy_pix_img(void);
uint8 SetSeed_Lane(uint8 MidStart,uint8 EdgeNum);
uint8 SetSeed_AprilTag(uint8 T_y,uint8 T_color);
void FloodSeedFill(uint8 x,uint8 y,uint8 Old_Color,uint8 New_Color);
uint8 If_Garage(void);  //�������  ���ȼ����ڻ�����ʮ��/T��
uint8 Judge_MOD3(void); //״̬�ж�
uint8 Width_Cali(uint8);    //������ȼ���
void Damn_Lose_Edge_all(void);  //˫�ඪ��
uint8 If_Lose_Edge(void);    //���߲�ȫ
void If_Straight(void); //ֱ���ж�
void Set_Mid(void); //��������
void Print_Mid(void);   //�滭����
void Search(void);  //������

//data
extern SEARCH_STRATEGY Search_Strategy;
extern uint8 temp1;
extern uint8 temp2;
extern uint8 img[IMG_Y][IMG_X];
extern uint8 pix_img[PIX_IMG_Y][PIX_IMG_X];
extern uint8 copy_pix_img[PIX_IMG_Y][PIX_IMG_X];
extern uint8 EdgeNum;
extern int16 mid[];
extern uint8 Round_Status;
extern uint16 Round_ad_limit;
extern uint16 RoundInCount;
extern uint8 RoundOutCount;
extern uint8 YRoadInCount;
extern uint16 SumInCD;
extern SUMINCD sumincd;

//flag
extern uint8 flag_Straight;
extern uint8 flag_T_Road;
extern uint8 flag_Y_Road;
extern uint16 flag_Y_Road_IN;
extern uint8 Y_Road_Status;
extern uint8 flag_Cross;
extern uint8 flag_Normal_Lose_L;
extern uint8 flag_Normal_Lose_R;
extern uint8 flag_Garage_L;
extern uint8 flag_Garage_R;
extern uint8 flag_AprilTag;

//----����/������־----//
extern uint8 flag_Indct_Help_Me;


#endif
