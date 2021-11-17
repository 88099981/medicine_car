/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               ����ai�Ӿ�һ�� ��д                 00**
**00                  ���ߣ�������                     00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            �������ã� motor                      00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/
#include "aicar_motor.h"
#include "zf_pwm.h"
#include "zf_gpio.h"
#include "aicar_init.h"

float kp_l=0.0, ki_l=0.0, kd_l=0.0;
float kp_r=0.0, ki_r=0.0, kd_r=0.0;

vuint8 S_i=0, S_j=0;
int16 speed_1[5]={0}, speed_2[5]={0};
int16 speed_sum1=0, speed_sum2=0;
int32 realspeed_l, realspeed_r;

float speed_now_filter1=0.0, speed_now_filter2=0.0;

float speed_error1 = 0;
float speed_error_last1 = 0;
float speed_error_last_last1 = 0;
float speed_out1 = 0;
float speed_error2 = 0;
float speed_error_last2 = 0;
float speed_error_last_last2 = 0;
float speed_out2 = 0;


void aicar_motor_init()
{
    gpio_init(DIR_1, GPO, 0, GPIO_PIN_CONFIG); 		//��Ƭ���˿�D0 ��ʼ��DIR_1			GPIO
    gpio_init(DIR_2, GPO, 0, GPIO_PIN_CONFIG); 		//��Ƭ���˿�D1 ��ʼ��DIR_2			GPIO
    pwm_init(PWM_1, 17000, 0);      				//��Ƭ���˿�D2 ��ʼ��PWM_1����10K ռ�ձ�0
    pwm_init(PWM_2, 17000, 0);     					//��Ƭ���˿�D3 ��ʼ��PWM_2����10K ռ�ձ�0
}

void aicar_motor_control(int32 motor1, int32 motor2)//1��2��,���Ʒ�����hcarһ��
{ 
    int32 motor_temp=0;
    motor_temp=motor2;
    motor2=-motor1;
    motor1=-motor_temp;
    if (motor1 >= 0)
    {
        gpio_set(DIR_1, 0);
        pwm_duty(PWM_1, motor1);
    }
    else
    {
        gpio_set(DIR_1, 1);
        pwm_duty(PWM_1, PWM_DUTY_MAX+motor1);
    }

    if (motor2 >= 0)
    {
        gpio_set(DIR_2, 0);
        pwm_duty(PWM_2, motor2);
    }
    else
    {
        gpio_set(DIR_2, 1);
        pwm_duty(PWM_2, PWM_DUTY_MAX+motor2);
    }

}

void aicar_motor_pid(int16 aim_left, int16 aim_right)
{
    realspeed_l=encoder1;
    realspeed_r=encoder2;

    speed_1[S_i] = realspeed_l;
    speed_2[S_i] = realspeed_r;

    //����
    if(S_j<5)
    {
        S_j++;
    }
    S_i++;
    if(S_i > 4)
    {
        S_i = 0;
    }
    //�ۺ͹���
    speed_sum1 = 0;
    speed_sum2 = 0;

    //�ۺ�
    for(uint8_t j=0;j<5;j++)
    {
        speed_sum1 += speed_1[j];
    }
    for(uint8_t j=0;j<5;j++)
    {
        speed_sum2 += speed_2[j];
    }

    //ƽ��
    speed_now_filter1 = speed_sum1/S_j;
    speed_now_filter2 = speed_sum2/S_j;


    speed_error1 = (float)(aim_left - speed_now_filter1);
    speed_out1 += kp_l * (speed_error1 - speed_error_last1) + ki_l * speed_error1 +
                kd_l*(speed_error1 - 2*speed_error_last1 + speed_error_last_last1);

    speed_error_last_last1 = speed_error_last1;
    speed_error_last1 = speed_error1;

    speed_error2 = (float)(aim_right - speed_now_filter2);
    speed_out2 += kp_r * (speed_error2 - speed_error_last2) + ki_r * speed_error2 +
                kd_r*(speed_error2 - 2*speed_error_last2 + speed_error_last_last2);

    speed_error_last_last2 = speed_error_last2;
    speed_error_last2 = speed_error2;



    if(speed_out1>49000)
    {
        speed_out1=49000;
    }
    else if(speed_out1<-49000)
    {
        speed_out1=-49000;
    }
    if(speed_out2>49000)
    {
        speed_out2=49000;
    }
    else if(speed_out2<-49000)
    {
        speed_out2=-49000;
    }
    aicar_motor_control((int32)speed_out1, (int32)speed_out2);
}
