#include "fd_GetMid.h"

//data

//-------------------���Ա���------------------//
uint8 yuanhuan_status56;//״̬78��������
uint8 temp1; //����ʱʹ�õ�ȫ�ֱ���
uint8 temp2; //����ʱʹ�õ�ȫ�ֱ���
//-------------------ģʽ����------------------//
SEARCH_STRATEGY Search_Strategy=MOD3;  //default MOD3
//-------------------ͼ������------------------//
uint8 img[IMG_Y][IMG_X];
uint8 pix_img[PIX_IMG_Y][PIX_IMG_X];
uint8 copy_pix_img[PIX_IMG_Y][PIX_IMG_X];
//-------------------�������------------------//
const DIRECTION Direction_4[]={{-1,0},{1,0},{0,1},{0,-1}};                                //4��ͨ��ƫ��ֵ
const DIRECTION Direction_8[]={{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0}};    //8��ͨ��ƫ��ֵ
//-------------------��������------------------//
EDGE edge[EDGE_MAX];      //�߽�ṹ������ �߽���ֵΪ0��ʱ��Ϊ����
ANGLE angle[EDGE_MAX];    //���ؽǶ�����
int16 mid[EDGE_MAX];      //��������
uint8 EdgeNum;            //������
uint8 MidStart=IMG_X/2;   //�ױ�������ʼ�������
//-------------------ʶ����ֵ------------------//
uint16 Round_ad_limit=600;  //�뻷ad��ֵ
uint16 Croess_ad_limit=120;
uint16 YRoad_ad_limit=30;
uint16 YRoad_SumInCD_Limit=200;   //Y��Road�ڶ��׶��ж���������ͨ�����
uint8 Garage_Rule=2;    //Ȧ�� Ԥ��Ϊ2Ȧ
//-------------------��������------------------//
uint16 RoundInCount=0; //�뻷����
uint8 RoundOutCount=0;
uint8 YRoadInCount=0;
uint8 CrossInCount=0;
uint8 AprilTagInCount=0;
uint8 GarageInCount=0;
uint16 SumInCD=0;   //��ͨ���ڵ����(���ػ���)
SUMINCD sumincd={0,0,0};
uint16 SumInCD_YRoad[2]={0,0};  //YRoad�������������


//flag

//-------------------������־,ԭ���Ͻ���ɨ�ߺ�����״̬������------------------//
uint8 flag_LoseEdge_part_L=0; //�󲿷ֶ���     ����֡��ʼ����
uint8 flag_LoseEdge_part_R=0; //              ����֡��ʼ����
uint8 flag_LoseEdge_all_L=0;  //              ����֡��ʼ����
uint8 flag_LoseEdge_all_R=0;  //              ����֡��ʼ����
uint8 flag_Straight_L=0;  //��ֱ��    ���ֱ���ȼ������򵥵Ķ�            ����֡��ʼ����
uint8 flag_Straight_R=0;  //��ֱ��                                         ����֡��ʼ����

//------------------�߼���־,ԭ������״̬��ȷ�����ߺ�������-------------------//
uint8 flag_Straight=0;
uint8 flag_T_Road=0;   //T��·����ָ��ϼ���ȫ�ڣ����³��ִ�������ඪ�ߵ�״��
uint8 flag_Y_Road=0;   //����
uint8 Y_Road_Status=0;  //����״̬  0��1ʶ��Y��·��2ȷ��Ϊ����
uint16 flag_Y_Road_IN=0; //������
uint8 flag_Cross=0;    //ʮ��                 ����֡��ʼ����
uint8 Round_Status=0; //0ɶ��û�� 1��һ�ο������� 3���������ҵ�д�����ֵ 5�ڶ��ο������� 7���� 9���� (ͬ����Ϊż)
uint8 flag_Normal_Lose_L=0;   //һ�㶪���                 ����֡��ʼ����
uint8 flag_Normal_Lose_R=0;   //һ�㶪�ұ�                 ����֡��ʼ����
uint8 flag_Garage_ARM=0;
uint8 flag_Garage_L=0;        //���������
uint8 flag_Garage_R=0;        //�������Ҳ�
uint8 flag_AprilTag=0;        //AprilTag����                                     ����֡��ʼ����

//--------------------------------����/������־------------------------------//
uint8 flag_Indct_Help_Me;   //�������ѭ��  ������ǵø�λ               ����֡��ʼ����



//******************************* Base Function *********************************/
void init(void)
{
    //****��֡��־λ��ʼ��****
    temp1=0;
    temp2=0;
    flag_LoseEdge_part_L=0;
    flag_LoseEdge_part_R=0;
    flag_LoseEdge_all_L=0;
    flag_LoseEdge_all_R=0;
    flag_Y_Road=0;
    flag_T_Road=0;
    flag_Cross=0;
    flag_Indct_Help_Me=0;
    flag_Straight_L=0;
    flag_Straight_R=0;
    flag_Normal_Lose_L=0;
    flag_Normal_Lose_R=0;
    flag_AprilTag=0;
    flag_Straight=0;
    sumincd.AprilTag=0;
    sumincd.Lane=0;
    sumincd.YRoad=0;
    //************************

    for(uint8 i=0;i<EDGE_MAX;i++)
    {
        edge[i].Lx=0;
        edge[i].Rx=IMG_X-1;
        angle[i].L=-1;  //Ĭ��Ϊ��
        angle[i].R=-1;
        mid[i]=0;
    }
}




void Y_Change(void)
{
    uint8 temp;
    for (uint8 i = 0; i < IMG_Y / 2; i++)
    {
        for (uint8 j = 0; j < IMG_X; j++)
        {
            temp = img[i][j];
            img[i][j] = img[IMG_Y - i - 1][j];
            img[IMG_Y - i - 1][j] = temp;
        }
    }
}



//���ٿ��� ����ѧ���ǳ�������
float Cp_sqrt(float number)
{
	long i;
	float x, y;
	const float f = 1.5F;

	x = number * 0.5F;
	y = number;
	i = *(long*)&y;	//	ȡ
	i = 0x5f3759df - (i >> 1);  //�����
	//i  = 0x5f375a86 - ( i >> 1 );  //Lomont
	y = *(float*)&i;
	y = y * (f - (x * y * y));
	y = y * (f - (x * y * y));
	return number * y;
}



//��ȡ�Ƕ� ���������н�
float Get_Angle(uint8 p1_y,uint8 p2_y,uint8 Target_y,uint8 l_or_r)
{
    float cos=0;
    float FA_x=0;   //����FA
    float FA_y=0;
    float FB_x=0;   //����FB
    float FB_y=0;

    if(l_or_r)
    {
        FA_x=edge[p1_y].Lx-edge[Target_y].Lx;
        FB_x=edge[p2_y].Lx-edge[Target_y].Lx;

        FA_y=p1_y-Target_y;
        FB_y=p2_y-Target_y;
    }
    else
    {
        FA_x=edge[p1_y].Rx-edge[Target_y].Rx;
        FB_x=edge[p2_y].Rx-edge[Target_y].Rx;

        FA_y=p1_y-Target_y;
        FB_y=p2_y-Target_y;
    }

    if(FA_x==0 || FB_x==0 || FA_y==0 || FB_y==0)
    {
        return 0;
    }
    
    cos=(FA_x*FB_x + FA_y*FB_y); //����
    cos/=Cp_sqrt(FA_x*FA_x + FA_y*FA_y)*Cp_sqrt(FB_x*FB_x + FB_y*FB_y); //��ĸ

    return (cos);
}


void Del_Edge_Data(uint8 p1_y,uint8 p2_y)
{
    for(uint8 i=p1_y;i<=p2_y;i++)
    {
        edge[i].Lx=0;
        edge[i].Rx=0;
    }
}




//ʹ��ǰ���������ߵ�б������Ŀ���
uint8 Set_Edge_BySlope(uint8 p1_y,uint8 p2_y,uint8 target_p_y,uint8 l_or_r) //ͨ����Ϊ�߶Ⱥͱ��ص��Ƕ�Ӧ�ģ�������������������ʾ��Ӧ�ĵ�
{
    float Slope=0;  //б�� ��ʵ��cot
    
    if(l_or_r)
    {
        Slope=(edge[p1_y].Lx - edge[p2_y].Lx)/(p1_y - p2_y);
        edge[target_p_y].Lx = edge[p1_y].Lx + (uint8)(Slope*target_p_y);
    }
    else
    {
        Slope=(edge[p1_y].Rx - edge[p2_y].Rx)/(p1_y - p2_y);
        edge[target_p_y].Rx = edge[p1_y].Lx + (uint8)(Slope*target_p_y);
    }

    return 1;
}



//�������� ͨ����Ϊ�߶Ⱥͱ��ص��Ƕ�Ӧ�ģ�������������������ʾ��Ӧ�ĵ� //ATTENTION p1_yС��p2_y
uint8 Connect(EDGE Target[],uint8 l_or_r,uint8 p1_y,uint8 p2_y)
{
    float Slope=0;   //б�� ��ʵ��cot
    if(l_or_r)
    {
        if((p1_y-p2_y) != 0)  //��ֱ��ʱ��Ͳ�������
        {
            Slope=(Target[p2_y].Lx-Target[p1_y].Lx)/(p2_y-p1_y);

            for(int i=1;i<=p2_y-p1_y;i++)
            {
                Target[p1_y+i].Lx=(uint8)(i*Slope+Target[p1_y].Lx);
            }
        }

        return 1;
    }
    else
    {
        if((p1_y-p2_y) != 0)  //��ֱ��ʱ��Ͳ�������
        {
            Slope=(Target[p2_y].Rx-Target[p1_y].Rx)/(p2_y-p1_y);

            for(int i=1;i<=p2_y-p1_y;i++)
            {
                Target[p1_y+i].Rx=(uint8)(i*Slope+Target[p1_y].Rx);
            }
        }

        return 1;
    }

}





uint8 Connect_pp(uint8 l_or_r,uint8 p1_x,uint8 p1_y,uint8 p2_x,uint8 p2_y)
{
    float Slope=0;
    if(p2_y==p1_y)
    {
        Slope=0;
    }
    else
    {
        Slope=(p2_x-p1_x)/(p2_y-p1_y);
    }

    if(l_or_r)
    {

        for(uint8 i=p1_y;i<=p2_y;i++)
        {
            edge[i].Lx=(int16)(p1_x+i*Slope);
        }

        return 1;
    }
    else
    {

        for(uint8 i=p1_y;i<=p2_y;i++)
        {
            edge[i].Rx=(int16)(p1_x+i*Slope);
        }

        return 1;
    }
}



//�������� ͨ����Ϊ�߶Ⱥͱ��ص��Ƕ�Ӧ�ģ�������������������ʾ��Ӧ�ĵ�    //ATTENTION p1_yС��p2_y
uint8 Mid_Connect(int16 Target[],uint8 p1_y,uint8 p2_y) 
{
    float Slope=0;   //б�� ��ʵ��cot

    if(p2_y==p1_y)
    {
        Slope=0;
    }
    else
    {
        Slope=(Target[p2_y]-Target[p1_y])/(p2_y-p1_y);
    }

    for(uint8 i=p1_y;i<=p2_y;i++)
    {
        Target[i]=Target[p1_y]+(int16)(i*Slope);
    }

    /*
    if((Target[p1_y]-Target[p2_y]) != 0)  //��ֱ��ʱ��Ͳ�������
    {
        Slope=(p1_y-p2_y)/(Target[p1_y]-Target[p2_y]);

        for(int i=1;i<=p2_y-p1_y;i++)
        {
            Target[p1_y+i]=(uint8)(i*Slope+Target[p1_y]);
        }
    }
    */

    return 1;
}


//ˮƽɨ�ߣ���ײ㣩 //ATTENTION yΪ������Ƶ׿�ĸ߶� yΪ�ڲ��ݹ�(����)ʹ�õĲ������ⲿ��������ֵ
uint8 Hor_Search_Base(uint8 MidStart,uint8 y)
{

    if(MidStart>IMG_X-X_LIMIT || MidStart<X_LIMIT || y>IMG_Y-2*Y_LIMIT )  //��Χ��֤
    {

        return 0;
    }

    if(img[y+Y_LIMIT][MidStart]==Black) //����������е��ں�����
    {


        uint8 Sum_of_white_zone=0;  //��ǰ�а������� Ĭ��������� �޸ĸ�����ע��ͬʱ�޸�temp_edge�����Ԫ�ظ���,�Լ�������һ�Ѷ�Ҫ��
        EDGE temp_edge;  //��ʱ�߽��

        temp_edge.Lx=0;  
        temp_edge.Rx=IMG_X-1;

        for(uint8 i=MidStart;i>=X_LIMIT;i--)    //��������
        {
            if(Sum_of_white_zone!=1 &&img[y+Y_LIMIT][i]==White)    //ɨ�������Ͳ����޸��ұ���
            {
                temp_edge.Rx=i;
                Sum_of_white_zone=1;    //��־�Ѿ������ҵ�����
            }

            if(Sum_of_white_zone==1 && img[y+Y_LIMIT][i]==Black)
            {
                temp_edge.Lx=i;
            }
        }

        for(uint8 i=MidStart;i<IMG_X-X_LIMIT;i++)    //��������
        {
            if(Sum_of_white_zone!=2 && img[y+Y_LIMIT][i]==White)    //ɨ�������Ͳ����޸������
            {
                temp_edge.Lx=i;
                Sum_of_white_zone=2;    //��־�Ѿ������ҵ�����
            }

            if(Sum_of_white_zone==2 && img[y+Y_LIMIT][i]==Black)
            {
                temp_edge.Rx=i;
            }
        }

        if(!Sum_of_white_zone)  //û����
        {
            flag_Indct_Help_Me=1;   //�����л������ѭ��

            return 0;
        }


        MidStart=(temp_edge.Rx+temp_edge.Lx)/2;

    }

    for(uint8 i=MidStart;i>=X_LIMIT;i--)   //��������
    {
        if(img[y+Y_LIMIT][i]==Black)  //TODO �Ľ��ж����ķ���
        {
            edge[y].Lx=i;                         //ע�⣡����ֱ��ΪĬ��ֵ0 �������Ҳ�Ǹ���0�жϱ����Ƿ�Ϊ���������
            break;
        }

        if(i==X_LIMIT)  //����Ե��δ�ҵ����� �ж϶���
        {
            flag_LoseEdge_part_L++;
        }
    }

    for(uint8 i=MidStart;i<=IMG_X-X_LIMIT;i++)   //��������
    {
        if(img[y+Y_LIMIT][i]==Black)
        {
            edge[y].Rx=i;
            break;
        }

        if(i==IMG_X-X_LIMIT)
        {
            flag_LoseEdge_part_R++;
        }
    }

    if(edge[y].Lx==0)   //�ײ㶪�� �Ǿ�����ͼ����ش�����
    {
        edge[y].Lx=X_LIMIT;
    }

    if(edge[y].Rx==0)
    {
        edge[y].Rx=IMG_X-X_LIMIT;
    }

    MidStart=(edge[0].Lx+edge[0].Rx)/2;

    return 0;
}



//ˮƽɨ�ߣ����У� //ATTENTION yΪ������Ƶ׿�ĸ߶�
uint8 Hor_Search(uint8 MidStart,uint8 y)
{
    if(MidStart>IMG_X-X_LIMIT || MidStart<X_LIMIT || y>IMG_Y-2*Y_LIMIT )  //��Χ��֤
    {
        return 0;
    }

    if(img[y+Y_LIMIT][MidStart]==Black) //����������е��ں�����
    {
        return 0;
    }

    for(uint8 i=MidStart;i>=X_LIMIT;i--)   //��������
    {
        if(img[y+Y_LIMIT][i]==Black) 
        {
            edge[y].Lx=i;                         //TODO ȷ����ע�͵���ȷ��-> ע�⣡����ֱ��ΪĬ��ֵ0 �������Ҳ�Ǹ���0�жϱ����Ƿ�Ϊ���������
            break;
        }

        if(i==X_LIMIT)  //����Ե��δ�ҵ����� �ж϶���
        {
            flag_LoseEdge_part_L++;
        }
    }
    for(uint8 i=MidStart;i<=IMG_X-X_LIMIT;i++)   //��������
    {
        if(img[y+Y_LIMIT][i]==Black)
        {
            edge[y].Rx=i;
            break;
        }

        if(i==IMG_X-X_LIMIT)
        {
            flag_LoseEdge_part_R++;
        }
    }
    /*
    if(edge[y].Rx-edge[y].Lx<=4)    //�����ǰ�����
    {
        EdgeNum=y;  //�������߶ȣ���������߼�ƫ�����Ӱ��
    }
    */

    if(y)   //������ױ�
    {
        if(edge[y].Lx==0)
        {
            flag_LoseEdge_part_L++;
        }
        else if(edge[y].Rx==0)
        {
            flag_LoseEdge_part_R++;
        }
    }

    return 0;
}



uint8 zoomin(float weight,uint8 PIX_DeBUG)
{
    uint16 pixsum=0;
    uint16 pixsum2=0;

    for(uint8 i=1,m=0;i<IMG_X-1;i+=3,m++)
    {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
        for(uint8 j=1,n=0;j<IMG_Y-1;j+=3,n++)
        {
            pixsum=(uint16)(img[j][i]*weight);

            for(uint8 k=0;k<8;k++)
            {
                pixsum2+=(uint16)(img[j+Direction_8[k].offset_y][i+Direction_8[k].offset_x]*(1-weight));
            }

            pixsum2/=8;
            pixsum+=pixsum2;

            if(pixsum>0xff)
            {
                pixsum=0xff;
            }
            else
            {
                 pixsum=0x00;
            }

            //  �ڴ���֮ǰ���øöλᵼ��ԭͼ���ݱ��ƻ�
            if(PIX_DeBUG)
            {
                img[j][i]=pixsum;

                for(uint8 k=0;k<8;k++)
                {
                    img[j+Direction_8[k].offset_y][i+Direction_8[k].offset_x]=(uint8)pixsum;
                }
            }

            pix_img[n][m]=pixsum;
        }
    }

    return 1;
}
//******************************* Base Function *********************************/


//*******************************************************************************/

 //��ʼ��ֱɨ��
uint8 Ver_Search(uint8 MidStart)
{
    EdgeNum=0;

    for(uint8 i=Y_LIMIT;i<=IMG_Y-Y_LIMIT;i++)
    {
        EdgeNum=i;  //�趨���߸߶� ��ɨ�����߶ȣ� EdgeNum ����Ϊ���鳤�� ��Y_LIMIT��ʼ
        if(img[i][MidStart]==Black)
        {
            break;
        }
    }

    return(EdgeNum);
}




//��ֱɨ�� ͨ�ô�ֱɨ�� ����ı�EdgeNum��ֵ   *ע�⣡ ��������Ǵ���ױ߿�ʼɨ��� //������Ϊɶ��д��һ�������������
uint8 Uni_Ver_Search(uint8 MidStart)
{
    uint8 IN_EdgeNum=0;

    for(uint8 i=0;i<=IMG_Y-Y_LIMIT;i++)
    {
        IN_EdgeNum=i;  //�趨���߸߶� ��ɨ�����߶ȣ� EdgeNum ����Ϊ���鳤�� ��Y_LIMIT��ʼ
        if(img[i][MidStart]==Black)
        {
            break;
        }
    }

    IN_EdgeNum-=Y_LIMIT*2;  //�ӵױ߿�ʼɨ�� ��ȥ˫���ı�Ե�߶�

    return(IN_EdgeNum);
}





void If_Straight(void)
{

    flag_Straight_L = 0; //�ȼ���Ϊֱ��
    flag_Straight_R = 0;

    for (uint8 i = 0; i < EdgeNum; i++)
    {
        angle[i].L = Get_Angle(0,EdgeNum-1, i, 1);
        angle[i].R = Get_Angle(0,EdgeNum-1, i, 0);
    }

    for (uint8 i = 0; i < EdgeNum; i++)
    {
        if (angle[i].L < -0.86) //��160��Ϊ�� 180�Ǳ�׼ֱ��
        {
            flag_Straight_L = 1;
        }

        if (angle[i].R < -0.86)
        {
            flag_Straight_R = 1;
        }
    }
}





//�����ȽϺ����������������ͼ���Ӧλ�ý��бȽϣ��������ƶ�(0~100) //ATTENTION T_x T_yΪ���½�����,����ע���������鲻ҪС��dx*dy
uint8 Feature_Verify(uint8 T_x,uint8 T_y,uint8 dx,uint8 dy,uint8 *feature)    
{
    float rate=0;

    if(T_y+dy>=IMG_Y || T_x+dx>=IMG_X)  //��Χ���
    {
        return(101);
    }

    for(uint8 i=0;i<dy;i++)
    {
        for(uint8 j=0;j<dx;j++)
        {
            if(img[T_y+i][T_x+j]==feature[i*dx+j])    //����������/ͼ��Ƚ�
            {
                rate++;
                //img[T_y+i][T_x+j]=Gray;
            }
        }
    }

    rate=(rate/(dx*dy))*100;

    return((uint8)rate);
}




//�����ȽϺ���(��ɫ),ɫ�ʷ�Χ0-255,expect_rate������0~100
uint8 Feature_Verify_Color(uint8 T_x,uint8 T_y,uint8 dx,uint8 dy,uint8 color,float expect_rate)    
{
    uint16 rate=0;
    uint16 unexpect_rate=(uint16)(((100-expect_rate)/100)*(dx*dy));

    if(T_y+dy>=IMG_Y || T_x+dx>=IMG_X)  //��Χ���
    {
        return(101);
    }

    for(uint8 i=0;i<dy;i++)
    {
        for(uint8 j=0;j<dx;j++)
        {
            if(img[T_y+i][T_x+j]!=color)    //����������/ͼ��Ƚ�
            {
                rate++;

                if(rate-unexpect_rate>0)  //����ģ��ƥ�䣬����������������ʱ���˳�
                {
                    return 0;
                }
                //img[T_y+i][T_x+j]=Gray;
            }
        }
    }

    /*
    rate=(dx*dy)-rate;
    rate=(rate/(dx*dy))*100;

    if(rate)
    return((uint8)rate);
    */
   return 1;    //���е��˴�ʱ˵���ѷ���Ҫ��
}




//�����ȽϺ���(���򣩣�thicknessΪ�����ȣ������������ͼ���Ӧλ�ý��бȽϣ��������ƶ�(0~100) 
uint8 Feature_Verify_Box(uint8 T_x,uint8 T_y,uint8 dx,uint8 dy,uint8 thickness,uint8 white_or_black)
{
    float rate=0;
    uint8 feature=white_or_black?White:Black; //ȷ����Ƚϵ���ɫ

    if(T_y+dy>=IMG_Y || T_x+dx>=IMG_X)  //��Χ���
    {
        return(101);
    }

    for(uint8 i=0;i<dy;i++)
    {
        for(uint8 j=0;j<dx;j++)
        {
            if(i<thickness || i>=dy-thickness)
            {
                if(img[T_y+i][T_x+j]==feature)
                rate++;

                //img[T_y+i][T_x+j]=Gray;

            }
            else
            {
                if(j<thickness || j>=dx-thickness)
                {
                    if(img[T_y+i][T_x+j]==feature)
                    rate++;

                    //img[T_y+i][T_x+j]=Gray;
                }
            }
        }
    }

    rate=(rate/(dx*dy-(dx-2*thickness)*(dy-2*thickness)))*100;
		
		return((uint8)rate);
}


//�����ȽϺ���(Mark) ���ڰ��ղ�ͬ��ͨ��ı�ǽ��бȽ�   //ATTENTION �ú����Ƚϵ���copy_pix_img
uint8 Feature_Verify_Mark(uint8 T_x,uint8 T_y,uint8 dx,uint8 dy,uint8 Mark,float expect_rate)
{
    uint16 rate=0;
    uint16 unexpect_rate=(uint16)(((100-expect_rate)/100)*(dx*dy));

    if(T_y+dy>=PIX_IMG_Y || T_x+dx>=PIX_IMG_X)  //��Χ���
    {
        return(101);
    }

    for(uint8 i=0;i<dy;i++)
    {
        for(uint8 j=0;j<dx;j++)
        {
            if(copy_pix_img[T_y+i][T_x+j]!=Mark)    //����������/ͼ��Ƚ�
            {
                rate++;

                if(rate-unexpect_rate>0)  //����ģ��ƥ�䣬����������������ʱ���˳�
                {
                    return 0;
                }
                //img[T_y+i][T_x+j]=Gray;
            }
        }
    }

   return 1;    //���е��˴�ʱ˵���ѷ���Ҫ��
}





//������������
void Copy_pix_img(void)
{
	for(uint8 i=0;i<PIX_IMG_Y;i++)  //���ٿ���һ���µ����ػ�ͼ�����������ͨ��ʱ������Ⱦ
	{
            memmove(&copy_pix_img[i][0],&pix_img[i][0],PIX_IMG_X);
	}
}





//����������ͨ�����ɵ�
uint8 SetSeed_Lane(uint8 MidStart,uint8 EdgeNum)
{
    uint8 Seed_x=MidStart/3;
    uint8 Seed_y=EdgeNum/4;

    if(copy_pix_img[Seed_y][Seed_x]==White)
    {
        uint8 SumWhite=0;
        for(uint8 i=0;i<8;i++)
        {
            if(copy_pix_img[Seed_y+Direction_8[i].offset_y][Seed_x+Direction_8[i].offset_x]==White)
            {
                SumWhite++;
            }
        }
        
        if(SumWhite>=5)
        {
            return 1;
        }
    }
    
    return 0;
}




//����AprilTagʶ�����ӵ� T_yΪʶ��߶ȣ�T_colorΪ������ͨ��ı�־ɫ
uint8 SetSeed_AprilTag(uint8 T_y,uint8 T_color)
{
    uint8 Contu_T_L=0;  //ɨ��T-B�����ǰ��������ͨ����ɫ�����
    uint8 Jump_L=0;
    uint8 Jump_R=0;
    uint8 SeedFound=0;

    for(uint8 i=1;i<PIX_IMG_X;i++)  //������ɨ����һ��T-B�����
    {
        if(copy_pix_img[T_y][i-1]==T_color && copy_pix_img[T_y][i]==T_color)
        {
            Contu_T_L++;
        }

        if(Contu_T_L>=2 && (copy_pix_img[T_y][i-1]==T_color && copy_pix_img[T_y][i]==Black))
        {
            Jump_L=i;
            break;
        }
    }

    for(uint8 i=Jump_L+1;i<PIX_IMG_X;i++)
    {
        if(copy_pix_img[T_y][i-1]==Black && copy_pix_img[T_y][i]==T_color)
        {
            Jump_R=i;
            SeedFound=1;
            break;
        }
    }

    if(Jump_L)

    if(SeedFound && Jump_L*Jump_R!=0)
        return((uint8)((Jump_R+Jump_L)*0.5));
    
    return 31;

}





void FloodSeedFill(uint8 x,uint8 y,uint8 Old_Color,uint8 New_Color)
{
    if((x>0 && x<PIX_IMG_X) && (y>0 && y<PIX_IMG_Y))  //���鷶Χ
    {
        if(copy_pix_img[y][x]==Old_Color)
        {
            copy_pix_img[y][x]=New_Color;
            SumInCD++;
            for(uint8 i=0;i<4;i++)  //4��ͨ��
            {
                FloodSeedFill(x+Direction_4[i].offset_x,y+Direction_4[i].offset_y,Old_Color,New_Color); //�ӵ��ݹ�
            }
        }
    }
}



//�жϰ����ߣ����⣩
uint8 If_Garage(void)
{
    uint8 StripeNum=0;  //������

    for(uint8 i=0;i<IMG_X-1;i++)
    {
        if(img[3][i]==White && img[3][i+1]==Black)
        {
            StripeNum++;
        }
    }

    if(StripeNum>=6)    //�����������趨ֵ
    {
        #ifdef IMG_DEBUG
        for(uint8 i=0;i<IMG_X;i++)  //�滭�����Ա����
        {
            img[3][i]=Gray;
        }
        #endif

        if(Feature_Verify_Color(10,0,20,10,White,80))
        {
            flag_Garage_L=1;
            flag_Garage_R=0;    //��Ϊû��ÿ֡��ʼ��������Ϊ�˱����������ì�ܣ��ʶ�������־λ���ø�
            return 1;
        }
        else if(Feature_Verify_Color(157,0,20,10,White,80))
        {
            flag_Garage_L=0;
            flag_Garage_R=1;
            return 1;
        }

        for(uint8 i=0;i<IMG_X;i++)
        {
            img[3][i]=Gray;
        }
    }

    return 0;
}




//״̬�ж�
uint8 Judge_MOD3(void)
{
    //-------״̬���� <head>--------//
    
    if(flag_Y_Road_IN)
    {
        flag_Y_Road_IN--;
    }

    if(YRoadInCount)
    {
        YRoadInCount--;
    }

    if(RoundInCount)
    {
        RoundInCount--;
    }

    if(RoundOutCount)
    {
        RoundOutCount--;
    }

    if(CrossInCount)
    {
        CrossInCount--;
    }
    
    if(AprilTagInCount)
    {
        AprilTagInCount--;
    }

    if(GarageInCount)
    {
        GarageInCount--;
    }

    //-------״̬���� <bottom>--------//




    //------T·��� <head>---------//
    if(Round_Status==0 && Feature_Verify_Color(0,44,187,5,Black,90))
    {
        flag_T_Road=1;
    }
    //------T·��� <bottom>---------//



    //------������ <head>---------//
    if(!GarageInCount && If_Garage())
    {
        GarageInCount=50;
        bb_time=100;
        if(flag_Garage_ARM<Garage_Rule-1)
        {
            flag_Garage_ARM++;
            flag_Garage_L=0;
            flag_Garage_R=0;
            sancha_wait_banma=0;
            //apriltag_wait_banma=0;
        }
        return 1;
    }
    //------������ <bottom>---------//




    //------������ <head>---------//
    if(sancha_wait_in<=0 && !sancha_wait_banma)
    {
        int32 YRoad_Diff;
        switch(Y_Road_Status)
        {
            case 0:
                if(!YRoadInCount && !flag_Y_Road_IN)
                {
                    uint8 SumMark_Lane_Head=0;  //ͼ����е�������Ǹ���  //��������˼��䡢T·
                    uint8 SumJumpMark_Lane_2_Black=0;   //��������ͨ���ǵ��ڿ�������ĸ���
                    uint8 SumJumpBlack_2_Mark_Lane=0;   //��������ͨ���ǵ��ڿ�������ĸ���

                    for(uint8 i=0;i<PIX_IMG_X;i++)
                    {
                        if(copy_pix_img[PIX_IMG_Y-1][i]==Mark_Lane)
                        {
                            SumMark_Lane_Head++;
                        }
                    }

                    if(SumMark_Lane_Head)
                    {
                        for(uint8 j=PIX_IMG_Y-4;j<PIX_IMG_Y;j++)
                        {
                            uint8 JumpP1=0; //�ҵ���һ��������־ �Թ涨������˳�� ÿ�г�ʼ��

                            for(uint8 i=1;i<PIX_IMG_X;i++)
                            {
                                if(copy_pix_img[j][i-1]==Mark_Lane && copy_pix_img[j][i]==Black)    //�����
                                {
                                    SumJumpMark_Lane_2_Black++;
                                    JumpP1=1;
                                }

                                if(JumpP1 && copy_pix_img[j][i-1]==Black && copy_pix_img[j][i]==Mark_Lane)    //�����
                                {
                                    SumJumpBlack_2_Mark_Lane++;
                                }
                            }
                        }
                    }

                    //temp=SumMark_Lane_Head;
                    temp1=SumJumpMark_Lane_2_Black;
                    temp2=SumJumpBlack_2_Mark_Lane;

                    if(SumJumpMark_Lane_2_Black>=4 && SumJumpBlack_2_Mark_Lane>=4)   //ɨ��4�У�ÿ��ÿ������1������� �����ѷſ�
                    {
                        Y_Road_Status=1;
                        //flag_Y_Road=1;
                        //openmvȷ������ת��
                        //YRoadInCount=30;
                        //flag_Y_Road_IN=40;
                    }

                    //If_Straight();  //�жϱ����Ƿ�ֱ
                    //if(flag_Straight_L || flag_Straight_R || GarageInCount)
                    if(GarageInCount)
                    {
                        Y_Road_Status=0;
                    }
                }
                break;
            
            case 1:
                SumInCD_YRoad[0]=SumInCD_YRoad[1];  //��ʷ����ǰ��
                SumInCD_YRoad[1]=sumincd.YRoad;

                YRoad_Diff=SumInCD_YRoad[1]-SumInCD_YRoad[0];

                if(YRoad_Diff<-20 || YRoad_Diff>60)     //TODO ����YRoad_Diff����
                {
                    Y_Road_Status=0; 
                }
                else if(SumInCD_YRoad[1]>=YRoad_SumInCD_Limit)
                {
                    Y_Road_Status=2;    //������
                    YRoadInCount=60;    //��������
                }


                break;

            case 2:
                break;
        }
    }

    if(ad_value_all<=YRoad_ad_limit)
    {
        flag_Y_Road_IN=40;
    }
    //-------������ <bottom>--------//




    //-------ʮ�ּ�� <head>--------//
    
    if(!flag_T_Road && (Round_Status<=5) && EdgeNum>=IMG_Y*0.6)
    {   
        if(Feature_Verify_Color(0,23,187,3,White,90))
        {
            flag_Cross=1;
            CrossInCount=10;
            return 1;
        }
    }
    //-------ʮ�ּ�� <bottom>--------//




    //------������� MOD3 <head>---------//

    switch(Round_Status)
    {
        case 0:
            if(flag_Y_Road || Feature_Verify_Color(83,9,20,40,Black,10))
            {
                break;
            }
        
            if(!RoundOutCount && Feature_Verify_Color(10,10,50,8,White,90) && !Feature_Verify_Color(127,10,50,3,White,90))
            {
                Round_Status=1;

                flag_Normal_Lose_L=1; //�����ƫ
            }
            else if(!RoundOutCount && Feature_Verify_Color(127,10,50,8,White,90) && !Feature_Verify_Color(10,10,50,3,White,90))
            {
                Round_Status=2;

                flag_Normal_Lose_R=1;
            }
        
            break;

        case 1:
            if(ad_value_all>Round_ad_limit && Feature_Verify_Color(0,10,20,10,Black,90))
            {


                Round_Status=3;
            }
            break;

        case 2:
            if(ad_value_all>Round_ad_limit && Feature_Verify_Color(167,10,20,10,Black,90))
            {


                Round_Status=4;
            }
            break;

        case 3:
            if(!RoundInCount && Feature_Verify_Color(0,10,50,10,White,90))
            {
                RoundInCount=25;
            }

            if(RoundInCount==1)
            {
                Round_Status=5;
            }

            flag_Normal_Lose_L=1;

            break;

        case 4:
            if(!RoundInCount && Feature_Verify_Color(137,10,50,10,White,90))
            {
                RoundInCount=25;
            }

            if(RoundInCount==1)
            {
                Round_Status=6;
            }

            flag_Normal_Lose_R=1;

            break;
        case 5:
            if(Feature_Verify_Mark(49,10,8,3,Mark_Lane,30))
            {
                Round_Status=7;
            }

            flag_Normal_Lose_L=1;   //�����ڴ������׻�
            break;

        case 6:
            if(Feature_Verify_Mark(10,10,8,3,Mark_Lane,30))
            {
                Round_Status=8;
            }
            
            flag_Normal_Lose_R=1;
            break;

        case 7:
            if(1)
            {
                Round_Status=9;

                flag_Normal_Lose_L=1;
            }
            break;

        case 8:
            if(1)
            {
                Round_Status=10;

                flag_Normal_Lose_R=1;
            }
            break;

        case 9:

            if(!RoundOutCount && Feature_Verify_Color(84,30,20,19,White,80))
            {
                Round_Status=11;

                RoundOutCount=50;
                flag_Normal_Lose_L=1;
            }
            break;

        case 10:

            if(!RoundOutCount && Feature_Verify_Color(84,30,20,19,White,80))
            {
                Round_Status=12;
                RoundOutCount=50;
                flag_Normal_Lose_R=1;
            }
            break;

        case 11:
            if(RoundOutCount==1)
            {
                huandao_wait_banma=1;
                Round_Status=0;
            }
            break;

        case 12:
            if(RoundOutCount==1)
            {
                huandao_wait_banma=1;
                Round_Status=0;
            }
            break;
    }

    if((flag_Y_Road || flag_Y_Road_IN) && Round_Status<3)    //��ֹ������ʶ��Ϊ����
    {
        Round_Status=0;
    }

    if(RoundOutCount && Round_Status<=8)
    {
        Round_Status=0;
    }

    if(huandao_wait_banma)
    {
        Round_Status=0;
    }
    //------������� <bottom>---------//





    //------AprilTag��� <head>---------//
    if(!AprilTagInCount && !Round_Status && !flag_Y_Road)   //�������ų�����Ԫ�� ����if(!Round_Status)
    {
        uint8 AprilTag_Mark_Base=0;

        for(uint8 i=0;i<PIX_IMG_X;i++)
        {
            if(copy_pix_img[1][i]==Mark_AprilTag)
            {
                AprilTag_Mark_Base++;
            }
        }

    if(!AprilTag_Mark_Base && sumincd.AprilTag>6 && sumincd.AprilTag<80 && !Feature_Verify_Color(10,10,20,10,White,80) && !Feature_Verify_Color(157,10,20,10,White,80))   //Ŀǰ��������ͨ�������ظ����жϣ���Ϊɨ�߸߶��ǹ̶���
        {
            flag_AprilTag=1;
            AprilTagInCount=50;
            aim_speed=-5;
            systick_delay_ms(1500);
            break_flag=1;
            systick_delay_ms(2000);
            if(apriltag_delay!=0)   apriltag_delay--;
            if(apriltag_delay==0&&(temp_buff[1]==0x01||temp_buff[1]==0x02))//����apriltag
            {
                find_apriltag();
                apriltag_delay=50;
            }
            aim_speed=SPEED_SET;
            break_flag=0;
            //find_apriltag();
            bb_time=20;
        }
    }

    //------AprilTag��� <bottom>---------//




    //-------���ඪ�� <head>--------//
    if(flag_LoseEdge_part_L!=0 && flag_LoseEdge_part_R==0) //���󶪱�
    {
        flag_Normal_Lose_L=1;   //��ͨ���󶪱�
        return 1;
    }
    else if(flag_LoseEdge_part_L==0 && flag_LoseEdge_part_R!=0) //���Ҷ���
    {
        flag_Normal_Lose_R=1;   //��ͨ���Ҷ���
        return 1;
    }

    //-------���ඪ�� <bottom>--------//


    return 0;
}



//���ݸ߶ȼ����������
inline uint8 Width_Cali(uint8 y)
{
    if(0)   //��ʱ��Ϊ�궨ֵ����
        return((edge[0].Rx-edge[0].Lx)-(uint8)(WIDTH_K*y)); //ʹ����ײ��ȼ���
    else
        return(WIDTH_BASE-(uint8)(WIDTH_K*y)); //ʹ�ñ궨ֵ����
}



//˫�ඪ��
void Damn_Lose_Edge_all(void)
{
    Connect_pp(1,40,0,70,48);
    Connect_pp(0,148,0,118,48);
    /*
    uint8 flag_Edge_Start_Exist=0;
    uint8 flag_Edge_End_Exist=0;
    uint8 EdgeStartPos=0;
    uint8 EdgeEndPos=0;

    for(uint8 i=0;i<EdgeNum;i++)
    {
        if(edge[i].Lx!=0 && edge[i].Rx!=0)  //�ж��Ƿ���ڱ���
        {
            flag_Edge_Start_Exist=1;
            EdgeStartPos=i;
            break;
        }
    }

    for(uint8 i=EdgeNum;i>0;i--)
    {
        if(edge[i].Lx!=0 && edge[i].Rx!=0)  //�ж��Ƿ���ڱ���
        {
            flag_Edge_End_Exist=0;
            EdgeEndPos=i;
            break;
        }
    }

    if(flag_Edge_Start_Exist && flag_Edge_End_Exist)
    {
        Connect(edge,1,EdgeStartPos,EdgeEndPos);
        Connect(edge,2,EdgeStartPos,EdgeEndPos);
    }
    */
}



//���߲�ȫ
uint8 If_Lose_Edge(void)
{

switch(Y_Road_Status)
{
    case 0:
        break;
    case 1:
        Connect_pp(1,40,0,70,48);   //��ֱ
        Connect_pp(0,148,0,118,48);

        break;
    
    case 2:
        Connect_pp(1,40,0,70,48);   //��ֱ
        Connect_pp(0,148,0,118,48);
        Y_Road_Status=0;

        bb_time=10;
        sancha_stop();
        sancha_wait_banma=1;

        break;
}


switch(Round_Status)
{
    case 0:
        break;

    case 1:
        flag_Normal_Lose_L=1;
        break;

    case 2:
        flag_Normal_Lose_R=1;
        break;

    case 3:
        bb_time=5;
        if(RoundInCount>10)
        {
            Connect_pp(0,120,0,10,48);
            flag_Normal_Lose_L=1;   //ATTENTION ����˴��޸��˵��ƫ��μ�aicar_error.c
        }

        break;

    case 4:
        bb_time=5;

        if(RoundInCount>10)
        {
            Connect_pp(1,68,0,178,48);
            flag_Normal_Lose_R=1;   //ATTENTION ����˴��޸��˵��ƫ��μ�aicar_error.c
        }

        break;

    case 5:
        //Connect_pp(0,120,0,10,48);
        flag_Normal_Lose_L=1;   //ATTENTION ����˴��޸��˵��ƫ��μ�aicar_error.c
        break;

    case 6:
        //Connect_pp(1,68,0,178,48);
        flag_Normal_Lose_R=1;   //ATTENTION ����˴��޸��˵��ƫ��μ�aicar_error.c
        break;

    case 7:
        Connect_pp(0,120,0,10,48);
        flag_Normal_Lose_L=1;   //ATTENTION ����˴��޸��˵��ƫ��μ�aicar_error.c
        break;

    case 8:
        Connect_pp(1,68,0,178,48);
        flag_Normal_Lose_R=1;   //ATTENTION ����˴��޸��˵��ƫ��μ�aicar_error.c
        break;

    case 9:
        Connect_pp(0,120,0,10,48);
        flag_Normal_Lose_L=1;   //ATTENTION ����˴��޸��˵��ƫ��μ�aicar_error.c
        break;

    case 10:
        Connect_pp(1,68,0,178,48);
        flag_Normal_Lose_R=1;   //ATTENTION ����˴��޸��˵��ƫ��μ�aicar_error.c
        break;

    case 11:
        flag_Normal_Lose_L=1;
        break;

    case 12:
        flag_Normal_Lose_R=1;
        break;
}

/*
    if(flag_Round_out_L && RoundOutCount>80)
    {
        Connect_pp(0,120,0,20,40);
        flag_Normal_Lose_L=1;
    }
    else if(flag_Round_out_R && RoundOutCount>80)
    {
        Connect_pp(1,68,0,168,40);
        flag_Normal_Lose_R=1;
    }



    if(flag_Round_in_L && RoundInCount>0)
    {
        Connect_pp(0,120,0,30,40);
        flag_Normal_Lose_L=1;
    }
    else if(flag_Round_in_R && RoundInCount>0)
    {
        Connect_pp(1,68,0,158,40);
        flag_Normal_Lose_R=1;
    }
*/

/*
    if(flag_Round_in_L && flag_Normal_Lose_L)   //�������ת����
    {
        flag_Normal_Lose_R=1;
        flag_Normal_Lose_L=0;
    }
    else if(flag_Round_in_R && flag_Normal_Lose_R)
    {
        flag_Normal_Lose_L=1;
        flag_Normal_Lose_R=0;
    }
*/

    if(flag_T_Road)
    {
        uint16 Height_L=0;
        uint16 Height_R=0;
        uint8 Height=0;
        for(uint8 i=0;i<IMG_X/2;i++)
        {   
            Height=0;
            for(uint8 j=0;j<IMG_Y-1;j++)
            {
                if(img[j][i]==Black || j==IMG_Y-2)
                {
                    Height=j;
                    break;
                }
            }
            Height_L+=Height;
        }

        for(uint8 i=IMG_X/2;i<IMG_X;i++)
        {
            Height=0;
            for(uint8 j=0;j<IMG_Y-1;j++)
            {
                if(img[j][i]==Black || j==IMG_Y-2)
                {
                    Height=j;
                    break;
                }
            }
            Height_R+=Height;
        }

        if(Height_L>Height_R)
        {
            Connect_pp(0,120,0,20,49);
            flag_Normal_Lose_L=1;
        }
        else if(Height_L<Height_R)
        {
            Connect_pp(1,68,0,168,49);
            flag_Normal_Lose_R=1;
        }
    }

    if(CrossInCount)  //ʮ��
    {
        Damn_Lose_Edge_all();   //˫�ඪ�ߺ���
        return 1;
    }
    else if(flag_Normal_Lose_L)  //���󶪱�
    {
        for(uint8 i=0;i<=EdgeNum;i++)
        {
            edge[i].Lx=edge[i].Rx-Width_Cali(i);    //ɾ�߲�ȫ  ��ֱ�Ӹ�д��߽�����
        }
        return 1;
    }
    else if(flag_Normal_Lose_R)  //���Ҷ���
    {
        for(uint8 i=0;i<=EdgeNum;i++)
        {
            edge[i].Rx=edge[i].Lx+Width_Cali(i);
        }
        return 1;
    }

    return 0;
}




//�����е�&����Ϊ����(�����������)
void Set_Mid(void)
{
    for(uint8 i=0;i<=EdgeNum;i++)   //�����е�
    {

        mid[i]=(edge[i].Lx+edge[i].Rx)/2;

        if(i>=5)    //�������ȶ��󣬶Գ����ݲ���������߽����ж�
        {
            if(mid[i]-mid[i-1]>=20 || mid[i]-mid[i-1]<=-20)
            {
                EdgeNum=i-1;  //�����е����
            }
        }
        
    }

    /*
    for(uint8 i=1;i<(EdgeNum/MID_RESOLUTION)-1;i++)   //������ϳ�ֱ��
    {
        Mid_Connect(mid,(i+1)*MID_RESOLUTION,i*MID_RESOLUTION);
    }
    */

}



//�򵥼�����
void Simple_Err_Check(void) 
{

}




//�滭����
void Print_Mid(void)
{
    for(uint8 i=Y_LIMIT; i<=EdgeNum+Y_LIMIT; i++)
    {
        img[i][edge[i-Y_LIMIT].Lx]=Gray;
        img[i][edge[i-Y_LIMIT].Rx]=Gray;

        if(mid[i]<IMG_X && mid[i]>0)    //��Ҳ��֪��Ϊʲô��ʱҪд�����ֵ�����
            img[i][mid[i-Y_LIMIT]]=Gray;
    }

}



//�ӵ���ɨ��������
void Search(void)
{

    init();
    Y_Change();
    zoomin(0.5,0);
    Copy_pix_img();

    //-------------------ɨ�߲��� <head>--------------//
    Hor_Search_Base(MidStart,0);   //��һ����ȷ��ɨ���׼����
    MidStart=(edge[0].Lx+edge[0].Rx)/2;
    Ver_Search(MidStart);   //��ֱɨ�� ����ɨ�����߶� EdgeNum

    for(uint8 i=0;i<=EdgeNum;i++)   //ˮƽɨ��ѭ��
    {
        Hor_Search(MidStart,i);
                                //TODO ȱһ���Զ��߸������ж����� flag_LoseEdge_all_X��λ
    }
    //If_Straight();  //ȷ��ֱ��
    //-------------------ɨ�߲��� <bottom>-------------//
    

    //-------------------��ͨ�����ò��� <head>---------------//
    if(SetSeed_Lane(MidStart,EdgeNum))
    {
        FloodSeedFill(MidStart/3,EdgeNum/4,White,Mark_Lane);
        sumincd.Lane=SumInCD;
        SumInCD=0;

        if(Y_Road_Status==1)
        {
            FloodSeedFill(PIX_IMG_X/2,PIX_IMG_Y-1,Black,Mark_YRoad);
            sumincd.YRoad=SumInCD;
            SumInCD=0;
        }

        FloodSeedFill(SetSeed_AprilTag(5,Mark_Lane),5,Black,Mark_AprilTag);
        sumincd.AprilTag=SumInCD;
        SumInCD=0;
    }

    //-------------------��ͨ�����ò��� <bottom>-------------//


    //-------------------�жϲ��� <head>---------------//
    switch(Search_Strategy)
    {
        case 3:
            Judge_MOD3();
            break;

        default:
            Judge_MOD3();
            break;
    }
    //-------------------�жϲ��� <bottom>-------------//


    //-------------------���߲��� <head>---------------//
    If_Lose_Edge(); 

    Set_Mid();
    //-------------------���߲��� <bottom>-------------//


    //-------------------�滭���� <head>---------------//
    Print_Mid();
    //-------------------�滭���� <bottom>-------------//

    //-------------------�������� <head>---------------//
    if(flag_Garage_L)
    {
        aicar_left_garage_in();
    }
    else if(flag_Garage_R)
    {
        aicar_right_garage_in();
    }
    //-------------------�������� <bottom>-------------//
}
