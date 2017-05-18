/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_SysTick.h
 * @brief      SysTick ����������������ʱ
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-10-08
 */

//SysTick ����ʱ��̫�̣��������� ��ʱ����������� ��ʱ����

#define     SYSTICK_CLK_KHZ                 core_clk_khz


/*          ������ʱ         */
extern void    systick_delay(uint32 time);
extern void    systick_delay_ms(uint32 ms);

#define     systick_delay_us(us)          systick_delay(us * SYSTICK_CLK_KHZ/1000);   //��ʱ us
#define     systick_delay_ns(ns)          systick_delay(ns * SYSTICK_CLK_KHZ/1000000);//��ʱ ns

/*       ���ڶ�ʱ           */
extern void systick_timing(uint32 time);
#define systick_timing_ms(ms)               systick_timing(ms * SYSTICK_CLK_KHZ);        //��ʱ us
#define systick_timing_us(us)               systick_timing(us * SYSTICK_CLK_KHZ/1000);   //��ʱ us
#define systick_timing_ns(ns)               systick_timing(ns * SYSTICK_CLK_KHZ/1000000);//��ʱ ns


