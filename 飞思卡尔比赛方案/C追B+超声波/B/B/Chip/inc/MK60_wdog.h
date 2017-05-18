/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_wdog.h
 * @brief      ���Ź���������
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-07-02
 */

#ifndef __MK60_WDOG_H__
#define __MK60_WDOG_H__

/********************************************************************/

//wdog ���룬���� LDO ʱ�ӣ�����Ƶ��

extern void wdog_init_ms(uint32 ms);   //��ʼ�����Ź�������ι��ʱ�� ms
extern void wdog_feed(void);           //ι��


extern void wdog_disable(void);        //���ÿ��Ź�
extern void wdog_enable(void);         //���ÿ��Ź�

/********************************************************************/
#endif /* __MK60_WDOG_H__ */
