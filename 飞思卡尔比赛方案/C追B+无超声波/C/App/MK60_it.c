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
void PIT1_IRQHandler(void)
{
   System_RunTime_Count++;
   PIT_Flag_Clear(PIT1);              //�嶨ʱ���жϱ�־λ
}