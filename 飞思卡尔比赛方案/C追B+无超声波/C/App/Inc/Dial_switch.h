#ifndef _DIAL_SWITCH_H_
#define _DIAL_SWITCH_H_
#include "include.h"

//#define READ_Coder_1 gpio_get(PTA4)   ʹ�������Ƭ��������
#define READ_Coder_2 gpio_get(PTA5)
#define READ_Coder_3 gpio_get(PTA6)     
//#define READ_Coder_4 gpio_get(PTA7)     ʹ�������Ƭ��������


//��ʼ�����뿪��
void Dial_switch_Init(void);
//��ȡ���뿪��ֵ
uint8_t Read_Coder_Key(void);
//���뿪������Ŀ���ٶ�
void Coder_ctrl_speed(void);
#endif