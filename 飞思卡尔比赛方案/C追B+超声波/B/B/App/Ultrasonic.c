/********************�Ͼ�����ѧ̩�ݿƼ�ѧԺ******************
                 * �ļ���       ��Ultrasonic.c
                 * ����         ��������ģ��
***************************************************************/
#include "common.h"
#include "include.h"
#include "Ultrasonic.h"
uint16 distance=0;
void Ultrasonic(void)
{
    uint32 time;
    uint32 flag;
    gpio_init(TRIG,GPO,0);
    gpio_init(ECHG,GPI,0);
    
    while(1)
    {
        flag = 0;
        gpio_set(TRIG,1);               //������������
        pit_delay_us(PIT1,15);
        gpio_set(TRIG,0);
        
        while(gpio_get(ECHG) == 0);             //�ȴ���ƽ��ߣ��͵�ƽһֱ�ȴ�
        pit_time_start  (PIT2);                 //��ʼ��ʱ
        while(gpio_get(ECHG) == 1)              //�ȴ���ƽ��ͣ��ߵ�ƽһֱ�ȴ�
        {
            flag++;
            if(flag >FLAGWAIT)
            {
                break;
            }
        };             
        
        time = pit_time_get_us(PIT2);    //ֹͣ��ʱ����ȡ��ʱʱ��
        //printf("\n\nʱ��Ϊ��%dus",time); 
        if(flag <FLAGWAIT )
        {
            distance = time * 340 /2/1000;
        } 

    }
}