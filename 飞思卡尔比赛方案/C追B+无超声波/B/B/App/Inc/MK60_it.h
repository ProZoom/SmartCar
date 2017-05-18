/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_it.h
 * @brief      ɽ��K60 ƽ̨�жϷ����ض���ͷ�ļ�
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-06-26
 */


#ifndef __MK60_IT_H__
#define __MK60_IT_H__

/*                          ���¶����ж�������
 *  ��ȡ��Ĭ�ϵ��ж�����Ԫ�غ궨��        #undef  VECTOR_xxx
 *  �����¶��嵽�Լ���д���жϺ���      #define VECTOR_xxx    xxx_IRQHandler
 *  ���磺
 *       #undef  VECTOR_003                         ��ȡ��ӳ�䵽�ж�����������жϺ�����ַ�궨��
 *       #define VECTOR_003    HardFault_Handler    ���¶���Ӳ���Ϸ��жϷ�����
 */
#include    "include.h"
#include    "common.h"
#define AMP1     ADC0_SE8        //PTB0
#define AMP2     ADC0_SE9        //PTB1
#define AMP3     ADC0_SE12       //PTB2
#define AMP4     ADC0_SE13       //PTB3
#define AMP5     ADC1_SE10       //PTB4
#define AMP6     ADC1_SE11       //PTB5
extern int16 *buffer;
extern uint16 Speed;
extern uint16 distance;
extern void Distance_Control(uint16 distance);

extern void PIT0_IRQHandler(void);
extern void PIT1_IRQHandler(void);
extern void PORTD_IRQHandler(void);
extern void PORTE_IRQHandler(void);
#endif  //__MK60_IT_H__
