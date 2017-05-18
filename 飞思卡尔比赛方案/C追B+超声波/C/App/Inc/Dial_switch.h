#ifndef _DIAL_SWITCH_H_
#define _DIAL_SWITCH_H_
#include "include.h"

//#define READ_Coder_1 gpio_get(PTA4)   使用这个单片机不工作
#define READ_Coder_2 gpio_get(PTA5)
#define READ_Coder_3 gpio_get(PTA6)     
//#define READ_Coder_4 gpio_get(PTA7)     使用这个单片机不工作


//初始化拨码开关
void Dial_switch_Init(void);
//读取拨码开关值
uint8_t Read_Coder_Key(void);
//拨码开关设置目标速度
void Coder_ctrl_speed(void);
#endif