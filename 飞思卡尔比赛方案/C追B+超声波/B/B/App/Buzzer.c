/********************南京理工大学泰州科技学院******************
                 * 文件名       ：Buzzer.c
                 * 描述         ：蜂鸣器模块
***************************************************************/
#include "include.h"

void Buzzer_init()
{
    gpio_init(BUZZER_IO,GPO,1);
}

void Buzzer(uint32 MS)
{
   //gpio_turn (BUZZER_IO)；
   gpio_set(BUZZER_IO,0);
   DELAY_MS(MS);
   gpio_set(BUZZER_IO,1);
   
}
