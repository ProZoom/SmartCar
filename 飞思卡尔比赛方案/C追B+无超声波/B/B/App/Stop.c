/********************南京理工大学泰州科技学院******************
                 * 文件名       ：Stop.c
                 * 描述         ：干簧管停车
***************************************************************/
#include "include.h"
void Stop_init()
{
  port_init(PTE24, ALT1 | IRQ_RISING | PULLDOWN | PF  );//初始化 PTE24管脚，复用功能为GPIO ，上升沿触发中断，上拉电阻
}
uint8 Stop_flag=0;
extern int16 System_RunTime_Count;
/*NRF发送数据缓存区(全局)*/
extern uint8 buff[DATA_PACKET]; 
extern uint8 relen;
void Stop()
{  
      Stop_flag++;
      //printf("Stop_flag=%d\n",Stop_flag);
      if(Stop_flag == 15)
          {
            disable_irq (PIT1_IRQn);
          }
}