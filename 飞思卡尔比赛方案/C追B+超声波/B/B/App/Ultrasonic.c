/********************南京理工大学泰州科技学院******************
                 * 文件名       ：Ultrasonic.c
                 * 描述         ：超声波模块
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
        gpio_set(TRIG,1);               //产生触发脉冲
        pit_delay_us(PIT1,15);
        gpio_set(TRIG,0);
        
        while(gpio_get(ECHG) == 0);             //等待电平变高，低电平一直等待
        pit_time_start  (PIT2);                 //开始计时
        while(gpio_get(ECHG) == 1)              //等待电平变低，高电平一直等待
        {
            flag++;
            if(flag >FLAGWAIT)
            {
                break;
            }
        };             
        
        time = pit_time_get_us(PIT2);    //停止计时，获取计时时间
        //printf("\n\n时间为：%dus",time); 
        if(flag <FLAGWAIT )
        {
            distance = time * 340 /2/1000;
        } 

    }
}