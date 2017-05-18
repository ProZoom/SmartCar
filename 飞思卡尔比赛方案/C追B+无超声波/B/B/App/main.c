/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 * @file       main.c
 * @brief      ɽ��K60 ƽ̨������
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-08-28
 */
/********************�Ͼ�����ѧ̩�ݿƼ�ѧԺ******************
                 * �ļ���       ��main.c
                 * ����         ��������ģ��
                 *���ͣ�B
***************************************************************/
#include "common.h"
#include "include.h"

/*****ȫ�ֱ�����*****/
int16 *buffer;
uint16 Speed=0;
float Speed_Set_Goal=2000;

extern float OutData[4];
uint32 Distance_Set_Goal=200;
int16 System_RunTime_Count=0;
/*NRF�������ݻ�����(ȫ��)*/
uint8 buff[DATA_PACKET]; 
uint8 relen;
/*���������(ȫ��)*/
uint16 distance=0;
uint8 flag_20ms;
uint32 duty;
uint32 time;
uint32 flag;
#if 1
  void  main(void)
{
    /*****Ӳ����ʼ��*****/
    HARDWARE_init();     
    /*****���뿪��ѡ�����*****/ 
    /*while(1)
    {}*/
    /*****��������*****/
    unsigned int k=200000;
    while(k--)
    {
      Normalization();
    }
    /*****��ʱ���жϳ�ʼ��*****/
    pit_init_ms(PIT0, 5);                              //��ʼ��PIT0����ʱʱ��Ϊ�� 5ms
    pit_init_ms(PIT1,20);                              //��ʼ��PIT1����ʱʱ��Ϊ�� 20ms
    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler); //����PIT0���жϷ�����Ϊ PIT0_IRQHandler
    set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler); //����PIT1���жϷ�����Ϊ PIT1_IRQHandler
    set_vector_handler(PORTD_VECTORn ,PORTD_IRQHandler);//���� PORTD ���жϷ�����Ϊ PORTD_VECTORn 
    set_vector_handler(PORTE_VECTORn ,PORTE_IRQHandler);//����PORTE���жϷ�����Ϊ PORTE_IRQHandler
    enable_irq (PIT0_IRQn);//����������жϺ�����5ms,���ڶ���������
    enable_irq (PIT1_IRQn); //���ٶȲɼ��жϺ�����20ms,���ڱջ�����,��ʱ
    enable_irq(PORTD_IRQn);//��ͨ���жϺ�������ͨ��
    enable_irq (PORTE_IRQn);//ʹ��PORTE�ж�,����ͣ��
   /*****��ѭ���������˻��������棬ͣ��*****/
    /*�Ĵ���ֵ=��duty-1��*60*/
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
            //printf("\n���յ�����:%d",buff);             //��ӡ���յ�������
            ftm_pwm_duty(FTM0, FTM_CH0,buff[0]);
        }
       else
        {   
            ftm_pwm_duty(FTM0, FTM_CH0,buff[0]);
        }
    }
}

#endif