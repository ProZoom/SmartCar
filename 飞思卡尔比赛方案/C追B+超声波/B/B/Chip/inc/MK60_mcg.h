/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_mcg.h
 * @brief      MCG PLL����
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-06-29
 */


#ifndef __MK60_MCG_H__
#define __MK60_MCG_H__

#include "common.h"

/********************************************************************/
#if defined(MK60DZ10)
typedef enum
{
    PLLUSR      ,  //�Զ������÷�Ƶϵ��ģʽ��ֱ�Ӽ��� ȫ�ֱ��� mcg_div ��ֵ
    PLL48,
    PLL50,
    PLL96,
    PLL100,
    PLL110,
    PLL120,
    PLL125,
    PLL130,
    PLL140,
    PLL150,
    PLL160,
    PLL170,
    PLL180,
    PLL200,
    PLL225,
    PLL250,

    PLL_MAX,
} PLL_e;

#elif defined(MK60F15)
typedef enum
{
    PLLUSR      ,  //�Զ������÷�Ƶϵ��ģʽ��ֱ�Ӽ��� ȫ�ֱ��� mcg_div ��ֵ
    PLL50       ,
    PLL100      ,
    PLL110      ,
    PLL120      ,
    PLL130      ,
    PLL140      ,
    PLL150      ,
    PLL160      ,
    PLL170      ,
    PLL180      ,
    PLL190      ,
    PLL200      ,
    PLL210      ,
    PLL220      ,
    PLL225      ,
    PLL230      ,
    PLL235      ,
    PLL244      ,
    PLL250      ,
    PLL275      ,             //K60FX512LQV15 ,ʵ����� 275M
    PLL300      ,

    //�������Ǻ�����ʱ������MHzΪ��λ����֧��С����
    //������Ҫ����Ƶ�ʵ����ѣ������Զ����Ƶϵ���ķ������������޸Ĵ���

    PLL_MAX,
} PLL_e;
#endif



typedef struct
{
    uint16  clk;         //
    uint8   prdiv;       //�ⲿ�����Ƶ����ѡ��
    uint8   vdiv;        //�ⲿ����Ƶ����ѡ��
} mcg_cfg_t;

//ʱ�ӷ�Ƶ����
typedef struct
{
    uint8 core_div;    //�ں�ʱ�ӷ�Ƶ����
    uint8 bus_div;     //����ʱ�ӷ�Ƶ����
    uint8 flex_div;    //flexʱ�ӷ�Ƶ����
    uint8 flash_div;   //flashʱ�ӷ�Ƶ����
} mcg_div_t;


uint8 pll_init(PLL_e pll);

__RAMFUNC    void set_sys_dividers(uint32 outdiv1, uint32 outdiv2, uint32 outdiv3, uint32 outdiv4);

/********************************************************************/
#endif /* __MK60_MCG_H__ */
