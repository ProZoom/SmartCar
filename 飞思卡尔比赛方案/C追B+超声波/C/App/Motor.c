/********************南京理工大学泰州科技学院******************
                 * 文件名       ：Motor.c
                 * 描述         ：电机封装
                 *功能          ：将双电机封装成一个电机
***************************************************************/
#include "include.h"
void Motor_Pos(uint32 duty)  //正转  duty(0,100)
{
    ftm_pwm_duty(FTM0, FTM_CH1,duty);
    ftm_pwm_duty(FTM0, FTM_CH3,duty);
}

void Motor_Neg(uint32 duty)  //反转
{
    ftm_pwm_duty(FTM0, FTM_CH0,duty);
    ftm_pwm_duty(FTM0, FTM_CH2,duty);
}
