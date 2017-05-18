/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_SysTick.c
 * @brief      SysTick ����������������ʱ
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-10-08
 */


#include "common.h"
#include "MK60_SysTick.h"
/*
Cortex-M4 ���ں��а���һ�� SysTick ʱ�ӡ�SysTick?Ϊһ��?24?λ�ݼ���������
SysTick �趨��ֵ��ʹ�ܺ�ÿ���� 1 ��ϵͳʱ�����ڣ�����ֵ�ͼ� 1��
������ 0 ʱ��SysTick �������Զ���װ��ֵ������������
ͬʱ�ڲ��� COUNTFLAG ��־����λ�������ж�(����ж�ʹ�������)��
*/


/*!
 *  @brief      SysTick��ʱ����
 *  @param      time          LPTMR��ʱʱ��(0~65535)
 *  @since      v5.0
 *  Sample usage:       systick_delay(32);     // systick ��ʱ32 ��ϵͳ����
 */
void systick_delay(uint32 time)
{
    if(time == 0)
    {
        return;
    }

    ASSERT(time <= (SysTick_RVR_RELOAD_MASK >> SysTick_RVR_RELOAD_SHIFT));  //24λ

    SYST_CSR = 0x00;                        //�ȹ��� systick ,���־λ

    SYST_RVR = time;                        //������ʱʱ��

    SYST_CVR = 0x00;                        //��ռ�����

    SYST_CSR = ( 0
                 | SysTick_CSR_ENABLE_MASK       //ʹ�� systick
                 //| SysTick_CSR_TICKINT_MASK    //ʹ���ж� (ע���˱�ʾ�ر��ж�)
                 | SysTick_CSR_CLKSOURCE_MASK    //ʱ��Դѡ�� ( core clk)
               );

    while( !(SYST_CSR & SysTick_CSR_COUNTFLAG_MASK));   //�ȴ�ʱ�䵽
}

/*!
 *  @brief      SysTick��ʱ����
 *  @param      ms          ��ʱʱ��
 *  @since      v5.0
 *  Sample usage:       systick_delay_ms(32);     // systick ��ʱ32 ms
 */
void systick_delay_ms(uint32 ms)
{
    while(ms--)
    {
        systick_delay(SYSTICK_CLK_KHZ);
    }
}

/*!
 *  @brief      SysTick��ʱ����
 *  @param      time          ��ʱʱ��(0~65535)
 *  @since      v5.0
 *  Sample usage:       systick_timing(32);     // systick ��ʱ 32 ��ϵͳ����
 */
void systick_timing(uint32 time)
{

    ASSERT(time <= (SysTick_RVR_RELOAD_MASK >> SysTick_RVR_RELOAD_SHIFT));  //24λ

    SYST_RVR = time;                        //������ʱʱ��

    //�������ȼ�
    NVIC_SetPriority (SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1); /* set Priority for Cortex-M4 System Interrupts */

    SYST_CVR = 0x00;                        //��ռ�����

    SYST_CSR = ( 0
                 | SysTick_CSR_ENABLE_MASK       //ʹ�� systick
                 | SysTick_CSR_TICKINT_MASK      //ʹ���ж� (ע���˱�ʾ�ر��ж�)
                 | SysTick_CSR_CLKSOURCE_MASK    //ʱ��Դѡ�� ( core clk)
               );
}




