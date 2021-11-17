/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               ����ai�Ӿ�һ�� ��д                 00**
**00                  ���ߣ�������                     00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            �������ã� servo                        00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/
#include "aicar_init.h"
#include "aicar_servo.h"
#include "zf_pwm.h"

void aicar_servo_init(void)
{
    pwm_init(S_MOTOR1_PIN,50,3750);
}
void aicar_servo_control(uint32 duty)//ǰ�ֶ����2900-5200����ֵ3850
{
    if(duty<=3000)
    {
        duty=3000;
    }
    else if(duty>=5100)
    {
        duty=5100;
    }
    pwm_duty(S_MOTOR1_PIN,duty);
}
void aicar_holder_init(void)//��̨��1150-6350����ֵ3850
{
    pwm_init(S_MOTOR2_PIN,50,3850);
}
void aicar_holder_control(uint32 duty)//��̨��1150-6350����ֵ3850
{
    if(duty<=1150)
    {
        duty=1150;
    }
    else if(duty>=6350)
    {
        duty=6350;
    }
    pwm_duty(S_MOTOR2_PIN,duty);
}
void aicar_laser_init(void)
{
    pwm_init(S_MOTOR3_PIN,125,0);//������Ƶ��Ϊ125hz��ռ�ձ�Ϊ50%
}