/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 * @file       main.c
 * @brief      山外K60 平台主程序
 * @author     山外科技
 * @version    v5.0
 * @date       2013-08-28
 */
/********************南京理工大学泰州科技学院******************
                 * 文件名       ：main.c
                 * 描述         ：主函数模块
                 *车型：B
***************************************************************/
#include "common.h"
#include "include.h"

/*****全局变量区*****/
int16 *buffer;
uint16 Speed=0;
float Speed_Set_Goal=2000;

extern float OutData[4];
uint32 Distance_Set_Goal=200;
int16 System_RunTime_Count=0;
/*NRF发送数据缓存区(全局)*/
uint8 buff[DATA_PACKET]; 
uint8 relen;
/*超声波测距(全局)*/
uint16 distance=0;
uint8 flag_20ms;
uint32 duty;
uint32 time;
uint32 flag;
#if 1
  void  main(void)
{
    /*****硬件初始化*****/
    HARDWARE_init();     
    /*****拨码开关选择策略*****/ 
    /*while(1)
    {}*/
    /*****赛道采样*****/
    unsigned int k=200000;
    while(k--)
    {
      Normalization();
    }
    /*****定时器中断初始化*****/
    pit_init_ms(PIT0, 5);                              //初始化PIT0，定时时间为： 5ms
    pit_init_ms(PIT1,20);                              //初始化PIT1，定时时间为： 20ms
    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler); //设置PIT0的中断服务函数为 PIT0_IRQHandler
    set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler); //设置PIT1的中断服务函数为 PIT1_IRQHandler
    set_vector_handler(PORTD_VECTORn ,PORTD_IRQHandler);//设置 PORTD 的中断服务函数为 PORTD_VECTORn 
    set_vector_handler(PORTE_VECTORn ,PORTE_IRQHandler);//设置PORTE的中断服务函数为 PORTE_IRQHandler
    enable_irq (PIT0_IRQn);//开方向控制中断函数，5ms,用于舵机方向控制
    enable_irq (PIT1_IRQn); //开速度采集中断函数，20ms,用于闭环控制,计时
    enable_irq(PORTD_IRQn);//开通信中断函数用于通信
    enable_irq (PORTE_IRQn);//使能PORTE中断,用于停车
   /*****主循环，用于人机交互界面，停车*****/
    /*寄存器值=（duty-1）*60*/
    //ftm_pwm_duty(FTM0, FTM_CH0,15);
    //ftm_pwm_duty(FTM0, FTM_CH1, 0);

   while(1)
    {   
       //OutPut_Data(); 
      Show();
      relen = nrf_rx(buff,1);
        //buff[0]=buff[0]*1;
        if(relen != 0)
        {
            //printf("\n接收到数据:%d",buff);             //打印接收到的数据
            ftm_pwm_duty(FTM0, FTM_CH0,buff[0]);
        }
       else
        {   
            ftm_pwm_duty(FTM0, FTM_CH0,buff[0]);
        }
    }
}

#endif