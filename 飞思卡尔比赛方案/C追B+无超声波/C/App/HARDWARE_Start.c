/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2014
 *     All rights reserved.
 * @file       HARDWARE_Start.c
 * @brief      基于山外固件库的硬件启动函数
 * @author     吴海斌
 * @version    v1.0
 * @date       2014-12-25
 */
#include "common.h"
#include "include.h"

void HARDWARE_init()
{
    //DisableInterrupts;
   /*****液晶初始化*****/
    LCD_Init();
    LCD_CLS();
  /*****NRF2401初始化*****/
   while(!nrf_init())      
   {
     while(!nrf_link_check())
     {
          LCD_P8x16Str(0,0,"Warning:");
          LCD_P8x16Str(20,3,"NRF Failure");
      }
   }
   /*****电机PWM初始化*****/
   ftm_pwm_init(FTM0, FTM_CH0,10*1000,0);
   ftm_pwm_init(FTM0, FTM_CH1,10*1000,0);
   ftm_pwm_init(FTM0, FTM_CH2,10*1000,0); 
   ftm_pwm_init(FTM0, FTM_CH3,10*1000,0);
   
   ftm_pwm_duty(FTM0, FTM_CH0,0);   
   ftm_pwm_duty(FTM0, FTM_CH1,0);
   ftm_pwm_duty(FTM0, FTM_CH2,0);
   ftm_pwm_duty(FTM0, FTM_CH3,0);
   
   /*****舵机PWM初始化*****/
   ftm_pwm_init(FTM1, FTM_CH1,100,14);
   ftm_pwm_duty(FTM1, FTM_CH1,14);
   FTM_CnV_REG(FTMN[1], FTM_CH1) =MIDVALUE;    //左转增加 右转减少  中值大约在5000  上下浮动不能超过正负1700
   
   /*****编码器测速初始化*****/
   lptmr_pulse_init(LPT0_ALT1, 0xffff, LPT_Rising); 
   /*****串口初始化*****/
   uart_init(UART5,9600); 
   /*****停止位初始化*****/
   Stop_init();
   /*****拨码开关初始化*****/
   Dial_switch_Init();
   /*****传感器AD初始化*****/
   adc_init(ADC0_SE16); 
   adc_init(ADC1_SE16);
   adc_init(ADC1_SE12);  
   adc_init(ADC1_SE9);    
   adc_init(ADC1_SE11);  
   adc_init(ADC0_SE12); 
   /*****超声波初始化*****/
    gpio_init(TRIG,GPO,0);
    gpio_init(ECHG,GPI,0);
   /*****PID控制器初始化*****/
    Position_PID_Init(); 
}
