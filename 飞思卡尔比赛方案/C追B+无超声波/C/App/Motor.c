/********************�Ͼ�����ѧ̩�ݿƼ�ѧԺ******************
                 * �ļ���       ��Motor.c
                 * ����         �������װ
                 *����          ����˫�����װ��һ�����
***************************************************************/
#include "include.h"
void Motor_Pos(uint32 duty)  //��ת  duty(0,100)
{
    ftm_pwm_duty(FTM0, FTM_CH1,duty);
    ftm_pwm_duty(FTM0, FTM_CH3,duty);
}

void Motor_Neg(uint32 duty)  //��ת
{
    ftm_pwm_duty(FTM0, FTM_CH0,duty);
    ftm_pwm_duty(FTM0, FTM_CH2,duty);
}
