/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_dac.c
 * @brief      DAC����
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-08-28
 */

#include "common.h"
#include "MK60_dac.h"

DAC_MemMapPtr DACN[2] = {DAC0_BASE_PTR, DAC1_BASE_PTR}; //��������ָ�����鱣�� DACN �ĵ�ַ


/*!
 *  @brief      DAC��ʼ��
 *  @param      DACn_e      DACģ���
 *  @since      v5.0
 *  Sample usage:       dac_init (DAC1 );    //��ʼ�� DAC1
 */
void dac_init(DACn_e dacn)
{
    /* ʹ��ʱ�� */
    SIM_SCGC2 |= (SIM_SCGC2_DAC0_MASK << dacn) ;    //ʹ��DACģ��

    /*  ����DAC�Ĵ���  */

    //����DAC_C0�Ĵ���
    DAC_C0_REG(DACN[dacn])  = ( 0
                                |  DAC_C0_DACTRGSEL_MASK                //ѡ���������
                                |  DAC_C0_DACRFS_MASK                   //ѡ��ο�VDD��ѹ(3.3V)
                                |  DAC_C0_DACEN_MASK                    //ʹ��DACģ��
                              );

    //����DAC_C1�Ĵ���
    DAC_C1_REG(DACN[dacn]) = ( 0
                             ) ;
    //����DAC_C2�Ĵ���
    DAC_C2_REG(DACN[dacn]) = ( 0
                               | DAC_C2_DACBFRP(0)             //���û�������ָ��ָ��0
                             );

    DAC_DATH_REG(DACN[dacn], 0) = 0;   //Ĭ�������͵�ѹ
    DAC_DATL_REG(DACN[dacn], 0) = 0;
}

/*!
 *  @brief      DAC���
 *  @param      DACn_e      DACģ���
 *  @param      val         ���ģ��������Ӧ����������12bit��
 *  @since      v5.0
 *  Sample usage:       dac_out (DAC1 ,0x100);    //��ʼ�� DAC1 ��� 0x100 ��������Ӧ��ģ����
 */
void dac_out(DACn_e dacn, uint16 val)
{
    ASSERT(val < 0x1000);               //val Ϊ 12bit

    DAC_DATH_REG(DACN[dacn], 0) = (val >> 8);   //�����ѹ
    DAC_DATL_REG(DACN[dacn], 0) = (val & 0xFF);
}
