/********************�Ͼ�����ѧ̩�ݿƼ�ѧԺ******************
                 * �ļ���       ��Stop.c
                 * ����         ���ɻɹ�ͣ��
***************************************************************/
#include "include.h"
void Stop_init()
{
  port_init(PTE24, ALT1 | IRQ_RISING | PULLDOWN  );//��ʼ�� PTE24�ܽţ����ù���ΪGPIO �������ش����жϣ���������
}
uint8 Stop_flag=0;
uint8 Key_jitter;
extern int16 System_RunTime_Count;
uint32 stop_duty;
/*NRF�������ݻ�����(ȫ��)*/
extern uint8 buff[DATA_PACKET]; 
extern uint8 relen;
void Stop()
{  
      Stop_flag++;
      if(Stop_flag == Key_jitter)
      {
        disable_irq (PIT1_IRQn);
        Motor_Pos(0); 
        Motor_Neg(stop_duty);
        DELAY_MS(500);//��ɲ���ƶ�����ֹͣ�����ж���ײ
        buff[0]=0;//֪ͨǰ��ͣ��
      }
}