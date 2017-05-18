/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       VCAN_TSL1401.h
 * @brief      ����CCD������
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2014-01-04
 */

#ifndef _TSL1401_H_
#define _TSL1401_H_ 1



#define TSL1401_SIZE    128                         //����CCDÿ�βɼ��Ĵ���

#define TSL1401_INT_TIME(time)  pit_init_ms(PIT0,time)     //����CCD ��ʱ�жϵ��ж�ʱ�����ú���


typedef enum
{
    TSL1401_L,      //��
    TSL1401_M,      //��
    TSL1401_R,      //��

    TSL1401_MAX,
} TSL1401_e;


typedef enum
{
    tsl1401_noint,      //û��ʼ��
    tsl1401_start,      //��ʼ�ɼ�
    //tsl1401_ing,        //�ɼ���
    tsl1401_finish,     //�ɼ�����
} tsl1401_status_e;


extern void tsl1401_init        (uint32 time);                              //tsl1401,����CCD��ʼ����timeΪ�ع�ʱ�䣬��λ�� TSL1401_INT_TIME ������һ�£���ʼ����������tsl1401_set_addrs �����ô洢��ַ�����������ô洢ͼ���ַ��
extern void tsl1401_set_addrs   (TSL1401_e num, uint8 *addr, ... );         //tsl1401,����CCD���òɼ����ݵĵ�ַ
extern void tsl1401_get_img     (void);                                     //tsl1401,����CCD�ɼ�ͼ��

extern void tsl1401_set_time    (uint32 time);                              //���� tsl1401,����CCD �ع�ʱ��
extern uint32 tsl1401_get_time     (void);                                  //��ȡ tsl1401,����CCD �ع�ʱ��

extern void tsl1401_led_en      (TSL1401_e num);                            //���������
extern void tsl1401_led_dis     (TSL1401_e num);                            //�رղ����


extern void tsl1401_time_isr    ();                                         //��ʱ�ع��жϷ�����

#endif  // _TSL1401_H_