/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               东秦ai视觉一队 编写                 00**
**00                  作者：刘坤龙                     00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            函数作用： error                       00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/
#include "aicar_error.h"
#include "aicar_init.h"
#include "aicar_adc.h"
#include "fd_GetMid.h"

int16 camera_error=0;
float kp_cam=0.0;
float kd_cam=0.0;
float a_cam=0.0;
void aicar_camera_error()
{
    camera_error=0;
    if(EdgeNum<=ERROR_EDGE)
    {
        for(uint8 i=1;i<=EdgeNum;i++)//遍历
        {
            camera_error+=mid[i]-94;
        }
        camera_error=-camera_error/EdgeNum;
    }
    else
    {
        for(uint8 i=1;i<=ERROR_EDGE;i++)//遍历
        {
            camera_error+=mid[i]-94;
        }
        camera_error=-camera_error/ERROR_EDGE;
    }
//    for(uint8 i=1;i<=Mid_Num;i++)//全场遍历
//    {
//        camera_error+=Mid_X[i]-94;
//    }
//    camera_error=-camera_error/Mid_Num;
    
//    kp_cam=camera_error*camera_error*a_cam;//二次
    servo_angle=(int16)(kp_cam*camera_error + kd_cam*(camera_error-lasttime_ad));
    lasttime_cam=camera_error;
    
    servo_duty=3850+servo_angle;
    if(servo_duty<=3450)
    {
        servo_duty=3450;
    }
    else if(servo_duty>=4250)
    {
        servo_duty=4250;
    }
}

void aicar_adc_error()
{
    ad_left=ad_value2*0.3+ad_value3*0.7;
    ad_right=ad_value4*0.7+ad_value5*0.3;
    
    ad_error=(ad_left-ad_right)*100/(ad_left+ad_right);
    servo_angle=(int16)(kp_ad*ad_error + kd_ad*(ad_error-lasttime_ad));
    lasttime_ad=ad_error;
    
    servo_duty=3850+servo_angle;
    if(servo_duty<=3450)
    {
        servo_duty=3450;
    }
    else if(servo_duty>=4250)
    {
        servo_duty=4250;
    }
}


void aicar_mix_error()
{
    uint32 servo_angle_cam=0, servo_angle_ad=0;
    
    camera_error=0;
    if(EdgeNum<=ERROR_EDGE)
    {
        for(uint8 i=1;i<=EdgeNum;i++)//遍历
        {
            camera_error+=mid[i]-94;
        }
        camera_error=-camera_error/EdgeNum;
    }
    else
    {
        for(uint8 i=0;i<=ERROR_EDGE;i++)//遍历
        {
            camera_error+=mid[i]-94;
        }
        camera_error=-camera_error/ERROR_EDGE;
    }
//    for(uint8 i=1;i<=Mid_Num;i++)//全场遍历
//    {
//        camera_error+=Mid_X[i]-94;
//    }
//    camera_error=-camera_error/Mid_Num;
    
//    kp_cam=camera_error*camera_error*a_cam;//二次
    
    ad_left=ad_value2*0.3+ad_value3*0.7;
    ad_right=ad_value4*0.7+ad_value5*0.3;
    ad_error=(ad_left-ad_right)*100/(ad_left+ad_right);

    if( Round_Status==3)
    {
        ad_left=ad_value1*0.7;  //图像扭不过电感，所以给电感加些偏差
        ad_right=ad_value6*0.3;

        ad_error=(ad_left-ad_right)*200/(ad_left+ad_right); 
    }
    else if( Round_Status==4)
    {
        ad_left=ad_value1*0.3;  //图像扭不过电感，所以给电感加些偏差
        ad_right=ad_value6*0.7;

        ad_error=(ad_left-ad_right)*200/(ad_left+ad_right); 
    }
    if( Round_Status==5)
    {
        ad_left=ad_value3*0.5;  //图像扭不过电感，所以给电感加些偏差
        ad_right=ad_value4*0.5;

        ad_error=(ad_left-ad_right)*200/(ad_left+ad_right); 
    }
    else if( Round_Status==6)
    {
        ad_left=ad_value3*0.5;  //图像扭不过电感，所以给电感加些偏差
        ad_right=ad_value4*0.5;

        ad_error=(ad_left-ad_right)*200/(ad_left+ad_right); 
    }

    if(Round_Status==9)
    {
        ad_left=ad_value3*0.7;  //图像扭不过电感，所以给电感加些偏差
        ad_right=ad_value4*0.3;

        ad_error=(ad_left-ad_right)*200/(ad_left+ad_right); 
        camera_error=0;
    }
    else if(Round_Status==10)
    {
        ad_left=ad_value3*0.3;  //图像扭不过电感，所以给电感加些偏差
        ad_right=ad_value4*0.7;

        ad_error=(ad_left-ad_right)*200/(ad_left+ad_right);
        camera_error=0;
    }
    if(flag_T_Road)
    {
        camera_error*=1.3;
        ad_error*=0.7; 
    }
    /*
    if(Round_Status==11 && RoundOutCount>30)
    {
        ad_left=ad_value1*0.7;  //图像扭不过电感，所以给电感加些偏差
        ad_right=ad_value6*0.3;

        ad_error=(ad_left-ad_right)*200/(ad_left+ad_right); 
        
        if(camera_error<0)
        {
            camera_error=0;
        }
        

        if(ad_error<0)
        {
            ad_error=0;
        }
    }
    else if(Round_Status==12 && RoundOutCount>30)
    {
        ad_left=ad_value1*0.3;  //图像扭不过电感，所以给电感加些偏差
        ad_right=ad_value6*0.7;

        ad_error=(ad_left-ad_right)*200/(ad_left+ad_right); 
        
        if(camera_error>0)
        {
            camera_error=0;
        }
        

        if(ad_right>0)
        {
            ad_error=0;
        }
    }
    */
    /*
    if(Round_Status==11)
    {
        if(camera_error<0)
        {
            camera_error=0;
        }
    }
    else if(Round_Status==12)
    {
        if(camera_error>0)
        {
            camera_error=0;
        }
    }
    */

/*
    if(flag_Round_in_L)
    {
        if(RoundInCount<40)
        {
            bb_time=5;
            ad_left=ad_value1*0.6;
            ad_right=ad_value6*0.4;
        }
        //ad_left=ad_value1*0.5;
        //ad_right=ad_value6*0.5;
        //ad_left=ad_value1*0.8+ad_value3*0.1;
        //ad_right=ad_value6*0.25+ad_value4*0.1;
        ad_error=(ad_left-ad_right)*200/(ad_left+ad_right); 

        //ad_error*=0.8;
        //camera_error*=1.2;
        if(ad_error<0)//正左负右
        {
            //ad_error=Cp_sqrt(-ad_error);
            //ad_error=-ad_error;               
        }
        if(camera_error<0)//左正右负
            camera_error=0;             
                       
        //camera_error=0;
    }
    else if(flag_Round_in_R)
    {
        if(RoundInCount<40)
        {
            bb_time=5;
            ad_left=ad_value1*0.4;
            ad_right=ad_value6*0.6;
        }
        //ad_left=ad_value1*0.5;
        //ad_right=ad_value6*0.5;
        //ad_left=ad_value1*0.25;
        //ad_right=ad_value6*0.8;
        ad_error=(ad_left-ad_right)*200/(ad_left+ad_right);   

        //ad_error*=0.8;
        //camera_error*=1.2;
        if(ad_error>0)
            ad_error=Cp_sqrt(ad_error);//左正右负
        if(camera_error>0)
            camera_error=0;//左正右负       
    }
*/

    servo_angle_cam=(int16)(kp_cam*camera_error + kd_cam*(camera_error-lasttime_ad));
    lasttime_cam=camera_error;      
    
    servo_angle_ad=(int16)(kp_ad*ad_error + kd_ad*(ad_error-lasttime_ad));
    lasttime_ad=ad_error;
    
    servo_angle=(uint32)(servo_angle_ad+servo_angle_cam)/2;
    servo_duty=3850+servo_angle;
    if(servo_duty<=3450)
    {
        servo_duty=3450;
    }
    else if(servo_duty>=4250)
    {
        servo_duty=4250;
    }
}


void car_pid(int16 error)
{
    servo_angle=(int16)(kp_cam*error + kd_cam*(error-lasttime_ad));
    lasttime_cam = error;
    
    left_motor=90+servo_angle;
    right_motor=90-servo_angle ;
}