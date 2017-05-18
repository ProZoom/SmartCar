/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_rtc.h
 * @brief      rtcģ�麯��
 * @author     ɽ��Ƽ�
 * @version    v5.1
 * @date       2014-04-25
 */

#ifndef _MK60_RTC_H_
#define _MK60_RTC_H_


#define rtc_alarm_irq_en()      RTC_IER |= RTC_IER_TAIE_MASK      //ʹ�������ж�
#define rtc_alarm_irq_dis()     RTC_IER &= ~RTC_IER_TAIE_MASK     //��ֹ�����ж�


#define rtc_overflow_irq_en()   RTC_IER |= RTC_IER_TOIE_MASK      //ʹ������ж�
#define rtc_overflow_irq_dis()  RTC_IER &= ~RTC_IER_TOIE_MASK     //��ֹ����ж�

#define rtc_invalid_irq_en()    RTC_IER |= RTC_IER_TIIE_MASK      //ʹ����Ч�����ж�
#define rtc_invalid_irq_dis()   RTC_IER &= ~RTC_IER_TIIE_MASK     //��ֹ��Ч�����ж�



extern void     rtc_init(void);                         //��ʼ��

extern void     rtc_set_time(uint32 seconds);           //����ʱ��
extern uint32   rtc_get_time(void);                     //��ȡʱ��

extern uint8    rtc_set_alarm(uint32 alarm);            //��������
extern void     rtc_close_alarm(void);                  //�ر�����

extern void     rtc_test_handler(void);                 //�ɹ��ο��� RTC�жϷ�������


#endif