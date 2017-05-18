/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_dac.h
 * @brief      DAC����
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-08-28
 */

#ifndef     _MK60_DAC_H_
#define     _MK60_DAC_H_ 1u

typedef enum DACn       //DACģ��
{
    DAC0,
    DAC1
} DACn_e;

extern void dac_init(DACn_e);               //DACһ��ת����ʼ��
extern void dac_out(DACn_e, uint16 val);    //DACһ��ת������

#endif  //_MK60_DAC_H_

