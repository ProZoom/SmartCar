/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2014
 *     All rights reserved.
 * @file       HARDWARE_Start.c
 * @brief      ����ɽ��̼����Ӳ����������
 * @author     �⺣��
 * @version    v1.0
 * @date       2014-12-25
 */
#include "common.h"
#include "include.h"

void HARDWARE_init()
{
    //DisableInterrupts;
   /*****Һ����ʼ��*****/
    LCD_Init();
    LCD_CLS();
  /*****NRF2401��ʼ��*****/
   while(!nrf_init())      
   {
     while(!nrf_link_check())
     {
          LCD_P8x16Str(0,0,"Warning:");
          LCD_P8x16Str(20,3,"NRF Failure");
      }
   }
   /*****���PWM��ʼ��*****/
   ftm_pwm_init(FTM0, FTM_CH0,10*1000,0);
   ftm_pwm_init(FTM0, FTM_CH1,10*1000,0);
   ftm_pwm_init(FTM0, FTM_CH2,10*1000,0); 
   ftm_pwm_init(FTM0, FTM_CH3,10*1000,0);
   
   ftm_pwm_duty(FTM0, FTM_CH0,0);   
   ftm_pwm_duty(FTM0, FTM_CH1,0);
   ftm_pwm_duty(FTM0, FTM_CH2,0);
   ftm_pwm_duty(FTM0, FTM_CH3,0);
   
   /*****���PWM��ʼ��*****/
   ftm_pwm_init(FTM1, FTM_CH1,100,14);
   ftm_pwm_duty(FTM1, FTM_CH1,14);
   FTM_CnV_REG(FTMN[1], FTM_CH1) =MIDVALUE;    //��ת���� ��ת����  ��ֵ��Լ��5000  ���¸������ܳ�������1700
   
   /*****���������ٳ�ʼ��*****/
   lptmr_pulse_init(LPT0_ALT1, 0xffff, LPT_Rising); 
   /*****���ڳ�ʼ��*****/
   uart_init(UART5,9600); 
   /*****ֹͣλ��ʼ��*****/
   Stop_init();
   /*****���뿪�س�ʼ��*****/
   Dial_switch_Init();
   /*****������AD��ʼ��*****/
   adc_init(ADC0_SE16); 
   adc_init(ADC1_SE16);
   adc_init(ADC1_SE12);  
   adc_init(ADC1_SE9);    
   adc_init(ADC1_SE11);  
   adc_init(ADC0_SE12); 
   /*****��������ʼ��*****/
    gpio_init(TRIG,GPO,0);
    gpio_init(ECHG,GPI,0);
   /*****PID��������ʼ��*****/
    Position_PID_Init(); 
}
