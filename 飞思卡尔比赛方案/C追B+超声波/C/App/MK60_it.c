/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_it.c
 * @brief      ɽ��K60 ƽ̨�жϷ�����
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-06-26
 */

#include    "MK60_it.h"
#include "VCAN_computer.h"
/*****5ms�ж�*****/
void PIT0_IRQHandler(void)
{
    Filter();
    buffer=Offset_Caculate();
    Position();         //�������
    // Motor_Control(Speed);
    //Distance_Control(distance);
    PIT_Flag_Clear(PIT0);              //�嶨ʱ���жϱ�־λ
}
/*****�ⲿ���Ŵ����ж�*****/
void PORTD_IRQHandler(void)
{
    uint8  n;    //���ź�
    uint32 flag;

    flag = PORTD_ISFR;
    PORTD_ISFR  = ~0;                                   //���жϱ�־λ

    n = 10;
    if(flag & (1 << n))                                 //PTD10�����ж�
    {
        nrf_handler();
    }
}
/*****PTE24�ж�,����ͣ��*****/
void PORTE_IRQHandler(void)
{
    uint8  n = 0;    //���ź�
    n = 24;
    if(PORTE_ISFR & (1 << n))           //PTE24 �����ж�
    {
        
        PORTE_ISFR  = (1 << n);        //д1���жϱ�־λ
        Stop();
    }
}
/*****20ms�ж�*****/
extern int16 System_RunTime_Count;
extern uint8 flag_20ms;
extern uint32 Distance_Set_Goal;
int32 Distance_variation;
extern uint32 duty;
void PIT1_IRQHandler(void)
{
   System_RunTime_Count++;
   flag_20ms=1;
   Distance_variation=distance-Distance_Set_Goal;
   //��������
   if(distance<100)  Motor_Pos(duty);
   //����
   if(Distance_variation-80>0)  Motor_Pos(duty+12);
       else if(Distance_variation-60>0)  Motor_Pos(duty+10);
               else if(Distance_variation-40>0)     Motor_Pos(duty+8);
                         else if(Distance_variation-20>0)     Motor_Pos(duty+4);
                              else     Motor_Pos(duty);
   //����
   if(Distance_variation-70<0)     Motor_Pos(duty-15);
      else if(Distance_variation-50<0)   Motor_Pos(duty-12);
               else if(Distance_variation-40<0)   Motor_Pos(duty-10);
                         else if(Distance_variation-20<0)  Motor_Pos(duty-5);
                                else   Motor_Pos(duty);
   PIT_Flag_Clear(PIT1);              //�嶨ʱ���жϱ�־λ
}