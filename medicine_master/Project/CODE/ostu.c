#include "ostu.h"
#include "cut.h"

#define Step_yu    252				//4��   �Ȳ�  �Ҷȼ�
#define Step   4

uint8 ostu_thres=0;

uint8 get_ostu_thres()
{
	uint16 i, j;
	
	//��ʹ�õ�ͼ��ķ�Χ 
	uint8 height_start=10;
	uint8 height = IMG_H;
	uint8 width_start=0;
	uint8 width = IMG_W;
	
	uint16 histogram[256]={0};						//ֱ��ͼ 
	uint16 histogram_cdf[256]={0};					//ֱ��ͼ���ۼƷֲ�ͼ���IMG_H*IMG_W 
	uint32 histogram_u_cdf[256]={0};				//���IMG_H*IMG_W*255
	
    uint8 find_gray_start=0;						//�ҵ��Ҷ�ֵ��ʼ��ı�־λ 
	//uint8 gray_start;								//ͼ�����ʼ�Ҷ�
	//uint8 gray_end;									//ͼ�����ֹ�Ҷ� 
	uint8 gray_ave;									//ͼ���ƽ���Ҷ�ֵ
	 
	uint8 bisection_left,bisection_right;			//���ַ�����ʼ����� 
    uint8 bisection_max_dis=Step*2;					//���ַ��������˵������
    uint16 n0,n1;									//��ֵ�������ߵ����ص��� 
    uint64 class_var_left,class_var_right;			//�������ߵ���䷽�� 
	uint64 class_var_i;								//i������䷽��
	 
	//�����Ҷ�ֱ��ͼ
	for(i=height_start;i<height;i++)
	{
		for(j = width_start;j<width;j++)
		{
			histogram[img[i][j]&Step_yu]++;			//Step_yu����252ʱ���൱������0xFC,����ĩβ��λ 
		}
	}
	
	histogram_cdf[0]=histogram[0];
	histogram_u_cdf[0]=0;
    for(i=Step;i<256;i=i+Step)
    {
    	histogram_cdf[i]=histogram_cdf[i-Step]+histogram[i];
		histogram_u_cdf[i]=histogram_u_cdf[i-Step]+histogram[i]*i;
		if(!find_gray_start&&histogram[i])
		{
			//gray_start=i;
		}
		else if(find_gray_start&&!histogram[i])
		{
			//gray_end=i-Step;
		}
	}
	gray_ave=(uint8)(histogram_u_cdf[Step_yu]/(uint32)histogram_cdf[Step_yu])&Step_yu;
	
	bisection_left=(uint8)(histogram_u_cdf[gray_ave]/(uint32)histogram_cdf[gray_ave])&Step_yu;
	bisection_right=(uint8)((histogram_u_cdf[Step_yu]-histogram_u_cdf[gray_ave])/
					(uint32)(histogram_cdf[Step_yu]-histogram_cdf[gray_ave]))&Step_yu;
	
	n0=histogram_cdf[bisection_left];
	n1=histogram_cdf[Step_yu]-n0;			//histogram_cdf[255]����ȡ�õ�ͼ������ص�Ķ��� 
	class_var_left=((uint64)histogram_u_cdf[Step_yu]*n0-(uint64)histogram_u_cdf[bisection_left]*histogram_cdf[Step_yu])*
					((uint64)histogram_u_cdf[Step_yu]*n0-(uint64)histogram_u_cdf[bisection_left]*histogram_cdf[Step_yu])/(uint64)n0
					+(((uint64)histogram_u_cdf[Step_yu]-(uint64)histogram_u_cdf[bisection_left])*(uint64)histogram_cdf[Step_yu]-(uint64)histogram_u_cdf[Step_yu]*n1)*
					(((uint64)histogram_u_cdf[Step_yu]-(uint64)histogram_u_cdf[bisection_left])*(uint64)histogram_cdf[Step_yu]-(uint64)histogram_u_cdf[Step_yu]*n1)/(uint64)n1;
	
	n0=histogram_cdf[bisection_right];
	n1=histogram_cdf[Step_yu]-n0;			//histogram_cdf[255]����ȡ�õ�ͼ������ص�Ķ��� 
	class_var_right=((uint64)histogram_u_cdf[Step_yu]*n0-(uint64)histogram_u_cdf[bisection_right]*histogram_cdf[Step_yu])*
					((uint64)histogram_u_cdf[Step_yu]*n0-(uint64)histogram_u_cdf[bisection_right]*histogram_cdf[Step_yu])/(uint64)n0
					+(((uint64)histogram_u_cdf[Step_yu]-(uint64)histogram_u_cdf[bisection_right])*(uint64)histogram_cdf[Step_yu]-(uint64)histogram_u_cdf[Step_yu]*n1)*
					(((uint64)histogram_u_cdf[Step_yu]-(uint64)histogram_u_cdf[bisection_right])*(uint64)histogram_cdf[Step_yu]-(uint64)histogram_u_cdf[Step_yu]*n1)/(uint64)n1;
	
	//���ַ� 
	for(i=((bisection_left+bisection_right)>>1)&Step_yu;(bisection_right-bisection_left)>=bisection_max_dis;i=((bisection_left+bisection_right)>>1)&Step_yu)
	{
		n0=histogram_cdf[i];
		n1=histogram_cdf[Step_yu]-n0;			//histogram_cdf[255]����ȡ�õ�ͼ������ص�Ķ���
		
		class_var_i=((uint64)histogram_u_cdf[Step_yu]*n0-(uint64)histogram_u_cdf[i]*histogram_cdf[Step_yu])*
					((uint64)histogram_u_cdf[Step_yu]*n0-(uint64)histogram_u_cdf[i]*histogram_cdf[Step_yu])/(uint64)n0
					+(((uint64)histogram_u_cdf[Step_yu]-(uint64)histogram_u_cdf[i])*(uint64)histogram_cdf[Step_yu]-(uint64)histogram_u_cdf[Step_yu]*n1)*
					(((uint64)histogram_u_cdf[Step_yu]-(uint64)histogram_u_cdf[i])*(uint64)histogram_cdf[Step_yu]-(uint64)histogram_u_cdf[Step_yu]*n1)/(uint64)n1;
		
		if(class_var_i>=class_var_left&&class_var_right>=class_var_left)		//�滻����ߵ�ֵ 
		{
			bisection_left=i;
			class_var_left=class_var_i;
		}
        else if(class_var_i>=class_var_right&&class_var_left>=class_var_right)	//�滻���ұߵ�ֵ 
        {
			bisection_right=i;
			class_var_right=class_var_i;
        }
        ////�ų����ŵ�����³��ֵ��м��ֵ��С��ʱ��  ������ѭ����
        else if(class_var_i<=class_var_left&&class_var_i<=class_var_right)
        {
          	//��������ȡ����
            if(class_var_left>=class_var_right)
			{
				bisection_right=i;               //�滻�ұ�
				class_var_right=class_var_i;
			}
            else
			{
				bisection_left=i;                //�滻���
				class_var_left=class_var_i;
			}
        }
	}
	
	n0=histogram_cdf[i];
	n1=histogram_cdf[Step_yu]-n0;			//histogram_cdf[255]����ȡ�õ�ͼ������ص�Ķ���
	
	class_var_i=((uint64)histogram_u_cdf[Step_yu]*n0-(uint64)histogram_u_cdf[i]*histogram_cdf[Step_yu])*
				((uint64)histogram_u_cdf[Step_yu]*n0-(uint64)histogram_u_cdf[i]*histogram_cdf[Step_yu])/(uint64)n0
				+(((uint64)histogram_u_cdf[Step_yu]-(uint64)histogram_u_cdf[i])*(uint64)histogram_cdf[Step_yu]-(uint64)histogram_u_cdf[Step_yu]*n1)*
				(((uint64)histogram_u_cdf[Step_yu]-(uint64)histogram_u_cdf[i])*(uint64)histogram_cdf[Step_yu]-(uint64)histogram_u_cdf[Step_yu]*n1)/(uint64)n1;
	if(class_var_i<class_var_left)
	{
		i=bisection_left;
	}
	else if(class_var_i<class_var_right)
	{
		i=bisection_right;
	}
        
	return i;
}

void binary_img()
{
    //uint8 in_ostu_thres=85;
	uint8 in_ostu_thres=get_ostu_thres();
	uint8 i,j;
	for(i=0;i<IMG_H;i++)
	{
		for(j=0;j<IMG_W;j++)
		{
			if(img[i][j]>in_ostu_thres)
				img[i][j]=White;
			else
				img[i][j]=Black;
		}
	}
}
