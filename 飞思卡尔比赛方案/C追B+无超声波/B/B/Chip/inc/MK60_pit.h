/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_pit.h
 * @brief      pit��ʱ��������
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-08-23
 */


#ifndef _MK60_PIT_H_
#define _MK60_PIT_H_

//����PITģ���
typedef enum
{
    PIT0,
    PIT1,
    PIT2,
    PIT3,

    PIT_MAX,
} PITn_e;

#define     PIT_Flag_Clear(PITn_e)          PIT_TFLG(PITn_e)|=PIT_TFLG_TIF_MASK        //���жϱ�־��д1 ��ձ�־λ��

//ms�Ǻ���=0.001�� us��΢��=0.000001�� ns������=0.000000001��

void    pit_close  (PITn_e);                                                            //�ر� PIT ��ʱ

/*****************************���´�������PIT�ж�*************************************/

void        pit_init(PITn_e, uint32 cnt);                                              //��ʼ��PITn�������ö�ʱʱ��(��λΪbusʱ������)
#define     pit_init_ms(PITn_e,ms)          pit_init(PITn_e,ms * bus_clk_khz);         //��ʼ��PITn�������ö�ʱʱ��(��λΪ ms)
#define     pit_init_us(PITn_e,us)          pit_init(PITn_e,us * bus_clk_khz/1000);    //��ʼ��PITn�������ö�ʱʱ��(��λΪ us)
#define     pit_init_ns(PITn_e,ns)          pit_init(PITn_e,ns * bus_clk_khz/1000000); //��ʼ��PITn�������ö�ʱʱ��(��λΪ ns)

/*****************************���ϴ�������PIT�ж�*************************************/
/*****************************���´�������PIT��ʱ*************************************/

//ע���ˣ���ʱ��������Ҫ��ʼ���ģ�ֱ�ӵ���

void        pit_delay(PITn_e, uint32 cnt);                                       //PIT��ʱ������Ҫ��ʼ���ģ�
#define     pit_delay_ms(PITn_e,ms)          pit_delay(PITn_e,ms * bus_clk_khz);        //PIT��ʱ ms
#define     pit_delay_us(PITn_e,us)          pit_delay(PITn_e,us * bus_clk_khz/1000);   //PIT��ʱ us
#define     pit_delay_ns(PITn_e,ns)          pit_delay(PITn_e,ns * bus_clk_khz/1000000);//PIT��ʱ ns

/*****************************���ϴ�������PIT��ʱ*************************************/
/*****************************���´�������PIT��ʱ*************************************/

void    pit_time_start  (PITn_e);                                                       //PIT��ʼ��ʱ
uint32  pit_time_get    (PITn_e);                                                       //��ȡ PITn��ʱʱ��(��ʱʱ��ر� ��ʱ��)����λΪ busʱ�ӣ�(��ֵΪ 0xFFFFFFFF�����ʾ���)

#define pit_time_get_ms(PITn_e)   (pit_time_get(PITn_e)/bus_clk_khz)                    //��ȡ��ʱʱ�䣨��λΪ ms��
#define pit_time_get_us(PITn_e)   (pit_time_get(PITn_e)/(bus_clk_khz/1000))             //��ȡ��ʱʱ�䣨��λΪ us��

/*****************************���ϴ�������PIT��ʱ*************************************/


#endif  //_MK60_PIT_H_
