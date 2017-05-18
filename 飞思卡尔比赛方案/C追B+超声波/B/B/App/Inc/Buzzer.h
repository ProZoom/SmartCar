#ifndef _BUZZER_H_
#define _BUZZER_H_
#include "include.h"
/*! 宏定义buzzer编号 */
#define BUZZER_IO  PTE25
//初始化蜂鸣器
void Buzzer_init();
//蜂鸣器间隔 ms 响一次
void Buzzer(uint32 MS);

#endif
