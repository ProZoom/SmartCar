/********************�Ͼ�����ѧ̩�ݿƼ�ѧԺ******************
                 * �ļ���       ��Buzzer.c
                 * ����         ��������ģ��
***************************************************************/
#include "include.h"

void Buzzer_init()
{
    gpio_init(BUZZER_IO,GPO,1);
}

void Buzzer(uint32 MS)
{
   //gpio_turn (BUZZER_IO)��
   gpio_set(BUZZER_IO,0);
   DELAY_MS(MS);
   gpio_set(BUZZER_IO,1);
   
}
