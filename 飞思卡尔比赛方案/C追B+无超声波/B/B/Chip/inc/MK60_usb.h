
/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_usb.c
 * @brief      USB �����(Ŀǰ �� �� ���� ���� )
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-09-26
 * @note       ������ ��ֲ ��˼������˾ �ṩ�� K60 �ο�����
 */


#ifndef _MK60_USB_H_
#define _MK60_USB_H_


#include "USB.h"


// USB  ģ��ʱ�ӱ��� ���� Ϊ 48M

// USB ģ��ʱ��Դ ѡ��
#define USB_CLOCK_PLL       //PLL����Ϊʱ��Դ
//#define USB_CLOCK_FLL     //FLL  ��Ϊʱ��Դ
//#define USB_CLOCK_CLKIN   //USB_CLKIN (�� PTE26) ��Ϊʱ��Դ

//USB clk = ʱ��ԴƵ�� * (1 + USB_CLK_FRAC) / (USB_CLK_DIV + 1)
//USB_CLK_FRAC = 0 �� 1
//USB_CLK_DIV 0 ~ 7
#if defined(MK60DZ10)
//PLL  = 96M
#define USB_CLK_DIV         1       // ʱ��Դ��Ƶ���� �� 
#define USB_CLK_FRAC        0

#elif defined(MK60F15)
//PLL  = 120M
#define USB_CLK_DIV         4       // ʱ��Դ��Ƶ���� �� 
#define USB_CLK_FRAC        1


#endif




void    usb_init(void);                         // USB ģ���ʼ��

void    usb_com_init(void);                     // USB ���⴮�� ��ʼ��
void    usb_enum_wait(void);                    // USB �ȴ�ö��
uint8   usb_com_rx(uint8 *rx_buf);              // USB ���⴮�� ����
void    usb_com_tx(uint8 *tx_buf, uint8 len);   // USB ���⴮�� ����


#endif  //_MK60_USB_H_
