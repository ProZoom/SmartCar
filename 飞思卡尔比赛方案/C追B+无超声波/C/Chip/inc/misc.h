/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       misc.h
 * @brief      ɽ��K60 ƽ̨������Ҫ�õ��ĺ�����ʵ��
 * @author     ɽ��Ƽ�
 * @version    v5.1
 * @date       2014-04-25
 */

#ifndef __MISC_H__
#define __MISC_H__

#include "common.h"


void write_vtor (int);                                              //�����ж�������ĵ�ַ
void set_vector_handler(VECTORn_t , void pfunc_handler(void));      //�����жϺ������ж���������


//���ݾɴ���
#define enable_irq(irq)                 NVIC_EnableIRQ(irq)         //ʹ��IRQ
#define disable_irq(irq)                NVIC_DisableIRQ(irq)        //��ֹIRQ
#define set_irq_priority(irq,pri0)      NVIC_SetPriority(irq,pri0)  //�������ȼ�

#if 1    //���ַ�����һ����CMSIS�Դ���һ����ֱ�ӻ������
#define EnableInterrupts                __enable_irq()              //ʹ��ȫ���ж�
#define DisableInterrupts               __disable_irq()             //��ֹȫ���ж�
#else
#define EnableInterrupts                asm(" CPSIE i");            //ʹ��ȫ���ж�
#define DisableInterrupts               asm(" CPSID i");            //��ֹȫ���ж�
#endif

void vcan_cpy( uint8 *dst, uint8 *src, uint32 count);

#endif  /* __MISC_H__ */

