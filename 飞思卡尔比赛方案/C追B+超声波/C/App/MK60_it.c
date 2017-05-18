/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_it.c
 * @brief      山外K60 平台中断服务函数
 * @author     山外科技
 * @version    v5.0
 * @date       2013-06-26
 */

#include    "MK60_it.h"
#include "VCAN_computer.h"
/*****5ms中断*****/
void PIT0_IRQHandler(void)
{
    Filter();
    buffer=Offset_Caculate();
    Position();         //方向控制
    // Motor_Control(Speed);
    //Distance_Control(distance);
    PIT_Flag_Clear(PIT0);              //清定时器中断标志位
}
/*****外部引脚触发中断*****/
void PORTD_IRQHandler(void)
{
    uint8  n;    //引脚号
    uint32 flag;

    flag = PORTD_ISFR;
    PORTD_ISFR  = ~0;                                   //清中断标志位

    n = 10;
    if(flag & (1 << n))                                 //PTD10触发中断
    {
        nrf_handler();
    }
}
/*****PTE24中断,用于停车*****/
void PORTE_IRQHandler(void)
{
    uint8  n = 0;    //引脚号
    n = 24;
    if(PORTE_ISFR & (1 << n))           //PTE24 触发中断
    {
        
        PORTE_ISFR  = (1 << n);        //写1清中断标志位
        Stop();
    }
}
/*****20ms中断*****/
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
   //丢车程序
   if(distance<100)  Motor_Pos(duty);
   //加速
   if(Distance_variation-80>0)  Motor_Pos(duty+12);
       else if(Distance_variation-60>0)  Motor_Pos(duty+10);
               else if(Distance_variation-40>0)     Motor_Pos(duty+8);
                         else if(Distance_variation-20>0)     Motor_Pos(duty+4);
                              else     Motor_Pos(duty);
   //减速
   if(Distance_variation-70<0)     Motor_Pos(duty-15);
      else if(Distance_variation-50<0)   Motor_Pos(duty-12);
               else if(Distance_variation-40<0)   Motor_Pos(duty-10);
                         else if(Distance_variation-20<0)  Motor_Pos(duty-5);
                                else   Motor_Pos(duty);
   PIT_Flag_Clear(PIT1);              //清定时器中断标志位
}