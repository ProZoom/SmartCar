#ifndef _BUZZER_H_
#define _BUZZER_H_
#include "include.h"
/*! �궨��buzzer��� */
#define BUZZER_IO  PTE25
//��ʼ��������
void Buzzer_init();
//��������� ms ��һ��
void Buzzer(uint32 MS);

#endif
