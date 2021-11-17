#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <stdio.h>


//typedef unsigned char       uint8;  /*  8 bits */
//typedef unsigned short int  uint16; /* 16 bits */
//typedef unsigned long int   uint32; /* 32 bits */
//typedef unsigned long long  uint64; /* 64 bits */

//typedef char                int8;   /*  8 bits */
//ypedef short int           int16;  /* 16 bits */
//typedef long  int           int32;  /* 32 bits */
//typedef long  long          int64;  /* 64 bits */

#define White (0xff)				//��ɫ
#define Gray (0x80)					//��ɫ
#define Black (0x00)				//��ɫ
#define IMAGE_H 50				//��ȡͼ��ĸ߶� 120
#define IMAGE_W 188             //��ȡͼ��Ŀ��
#define IMG_H 50					//Ԥ�����ͼ��ĸ߶� 
#define IMG_W 188					//Ԥ�����ͼ��Ŀ��
#define IMG_MID	94					//IMG_W/2
#define Num_edge 200				//Ԥ�Ʊ��ش��ڵĸ���
#define Num_mid 200					//Ԥ���е���ڵĸ���
#define Num_up_search_skip 2		//�ӵ�N������������
#define Num_lim_search_UP 2			//�����ϱ߽缫�ޣ�������ڵ���2
#define Num_lim_search_LR 2			//�������ұ߽缫�ޣ�������ڵ���2
#define Num_search_UP_Turn1 30		//ͼ���Ϸ����򻮷�Ϊ���ݣ��Ƿ�ֱ�������ұ���ͬ��
#define Num_search_UP_Turn2 158		//ͼ���Ϸ����򻮷�Ϊ���ݣ��Ƿ�ֱ�������ұ��෴��
#define Num_search_LR_Turn1 10		//ͼ�����ұ߽绮��Ϊ���ݣ����зֽ�㣩
#define Num_search_LR_Turn2 25		//ͼ�����ұ߽绮��Ϊ���ݣ����Ϸֽ�㣩
#define Search_NUM_1_X (edge_X-1)	//�Ź���ɨ������
#define Search_NUM_2_X edge_X		//�Ź���ɨ������
#define Search_NUM_3_X (edge_X+1)	//�Ź���ɨ������
#define Search_NUM_4_X (edge_X-1)	//�Ź���ɨ������
#define Search_NUM_5_X (edge_X+1)	//�Ź���ɨ������
#define Search_NUM_6_X (edge_X-1)	//�Ź���ɨ������
#define Search_NUM_7_X edge_X		//�Ź���ɨ������
#define Search_NUM_8_X (edge_X+1)	//�Ź���ɨ������
#define Search_NUM_1_Y (edge_Y+1)	//�Ź���ɨ������
#define Search_NUM_2_Y (edge_Y+1)	//�Ź���ɨ������
#define Search_NUM_3_Y (edge_Y+1)	//�Ź���ɨ������
#define Search_NUM_4_Y edge_Y		//�Ź���ɨ������
#define Search_NUM_5_Y edge_Y		//�Ź���ɨ������
#define Search_NUM_6_Y (edge_Y-1)	//�Ź���ɨ������
#define Search_NUM_7_Y (edge_Y-1)	//�Ź���ɨ������
#define Search_NUM_8_Y (edge_Y-1)	//�Ź���ɨ������
#define NUM_Search_8_special_Mod 12 //�Ź���ɨ�������������
#define NUM_curve_Mod 4				//ת��ģ�͸���
#define NUM_curve_special_Mod 9		//�������ģ�͸���
#define NUM_curve_S_Mod 5			//��ת��ģ�͸���
#define NUM_cross_Mod 9				//ʮ��·ģ�͸���
#define NUM_No_Mod 5				//ȱ��һ��ģ�͸���
#define SP_curve_or_Cross 20		//���б�ڣ�ʮ�ֻ���������߽���ֵ
#define Num_Croos_find_turn_point 3 //ʮ�ֹյ�����Ϊ��ɫ���������ж����ݣ�
#define Num_Search_to_UP_skip 6		//����������ʱ��һ�ο��������������������
#define Cross_3_Make_edge 30		//ʮ�ֵ����׶����߻�����
#define Search_Start_NUM_ 15			//Ѱ����ʼ��ʱ���������ʼ��
#define NUM_block_route 4			//���ٸ�������Ϊͨ·
#define Search_up_NOedge_30_Move 10			//��������ʱ��30û�Ҽ�����X�ƶ�������ƶ�����
#define Cross_Turn_point_too_LOW 6			//ʮ�ֹյ�̫��,Y����ͱ߽�����
#define Route_NUM 7							//��ͨ����
#define angle_is_S 129				//��Ǽ���
#define angle_is_L 4				//ֱ�߼��ޣ������ڣ�С�ڣ�

#define TwoPointIsSame 5			//�����Χ����Ϊ����������ͬ��
#define LineJudgeEdgeNumMin 20		//ֱ���ж�ʱҪ������ߵ��������N
#define	WarningNum 20				//����ģʽɨ���һ����ֵ����IMG_W���
#define SearchToUPMaxSearchNum 6	//��������ʱ�ж�����ͬɫ�����������ֵ
#define CrossEdgeNumMustGreaterThen 70 //������ʮ��ÿ���ߵ�����������N

//extern uint8 mt9v03x_csi_image[IMAGE_H][IMAGE_W];
extern uint8 img[IMG_H][IMG_W];		//�յ���ͼ��
extern uint8 IMG_mid_L;				//ͼ��ƫ���е�
extern uint8 IMG_mid_R;				//ͼ��ƫ���е�
#endif
