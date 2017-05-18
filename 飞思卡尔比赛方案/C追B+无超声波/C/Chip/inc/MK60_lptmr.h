/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_lptmr.h
 * @brief      lptmr����������������ʱ�������������ʱ���ܴ�ʵ��
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-06-26
 */

#ifndef __MK60_LPTMR_H__
#define __MK60_LPTMR_H__

/**
 *  @brief LPTMR�����������ܽ�ѡ��
 */
typedef enum
{
    //ֻ��1��2�ܽţ���û��0��3�ܽ�
    LPT0_ALT1 = 1,      // PTA19
    LPT0_ALT2 = 2       // PTC5
} LPT0_ALTn;

/**
 *  @brief LPTMR�������������ʽ
 */
typedef enum LPT_CFG
{
    LPT_Rising  = 0,    //�����ش���
    LPT_Falling = 1     //�½��ش���
} LPT_CFG;

#define LPTMR_Flag_Clear()  (LPTMR0_CSR |= LPTMR_CSR_TCF_MASK)         //���LPT�Ƚϱ�־λ

/*          ������ʱ         */
extern void     lptmr_delay_ms(uint16 ms);      //��ʱ(ms)
extern void     lptmr_delay_us(uint16 us);      //��ʱ(us)

/*       ���ڶ�ʱ           */
extern void     lptmr_timing_ms(uint16 ms);     //��ʱ(ms)
extern void     lptmr_timing_us(uint16 ms);     //��ʱ(us)

/*        ���ڼ�ʱ           */
extern void     lptmr_time_start_ms(void);      //��ʼ��ʱ(ms)
extern uint32   lptmr_time_get_ms(void);        //��ȡ��ʱʱ��

extern void     lptmr_time_start_us(void);      //��ʼ��ʱ(ns)
extern uint32   lptmr_time_get_us(void);        //��ȡ��ʱʱ��

extern void     lptmr_time_close();             //�رռ�ʱ��


/*       �����������        */
extern void     lptmr_pulse_init  (LPT0_ALTn, uint16 count, LPT_CFG);   //�����������ʼ������
extern uint16   lptmr_pulse_get   (void);                               //��ȡ����ֵ
extern void     lptmr_pulse_clean (void);                               //��ռ���ֵ


/*      �жϷ�����ģ��    */
extern void lptmr_test_handler(void);                                       //�жϷ������������ο������û�����ʵ�֣�


#endif /* __MK60_LPTMR_H__ */
