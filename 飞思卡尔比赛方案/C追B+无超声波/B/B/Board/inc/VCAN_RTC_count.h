/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       VCAN_RTC_count.h
 * @brief      rtc���ݼ��㺯��ʵ��(��1970-01-01 Ϊ��׼����2099-12-31)
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-08-27
 */


#ifndef _VCAN_RTC_COUNT_H_
#define _VCAN_RTC_COUNT_H_

typedef struct
{
    uint16  year;   //��
    uint8   mon;    //��
    uint8   day;    //��

    uint8   hour;   //ʱ
    uint8   min;    //��
    uint8   sec;    //��

    uint8   invalid;//ʱ����Ч��飨0��ʾʱ����Ч��1��ʾʱ����Ч��
} time_s;


extern uint8   Is_LeapYear(uint32 year);               //�ж��Ƿ�Ϊ���꣨���򷵻�1�����򷵻�0��
extern uint32  year2day(uint32 year);                  //��ָ����ݵ�����
extern uint32  time2sec(time_s time);                  //������ʱ�����ʽʱ��תΪ�� 1970 -01-01Ϊ��������
extern void    sec2time(uint32 sec, time_s *time);     //�� 1970-01-01Ϊ�������� תΪ ������ʱ�����ʽʱ��

#endif
