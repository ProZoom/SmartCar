/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_adc.h
 * @brief      ADC����
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-08-28
 */


#ifndef __MK60_ADC_H__
#define __MK60_ADC_H__ 1

typedef enum
{
    // ---------------------------------ADC0-------------------------
    ADC0_DP0,
    ADC0_DP1,
    PGA0_DP,        //��֧��ADC����Ҫ���� ADC PGA register ���÷Ŵ�����
    ADC0_DP3,
    //���������֧��Bͨ��
    ADC0_SE4b,      // PTC2     ��֧�����ADC
    ADC0_SE5b,      // PTD1     ��֧�����ADC
    ADC0_SE6b,      // PTD5     ��֧�����ADC
    ADC0_SE7b,      // PTD6     ��֧�����ADC

    ADC0_SE8,       // PTB0
    ADC0_SE9,       // PTB1
    ADC0_SE10,      // PTA7
    ADC0_SE11,      // PTA8
    ADC0_SE12,      // PTB2
    ADC0_SE13,      // PTB3
    ADC0_SE14,      // PTC0
    ADC0_SE15,      // PTC1
    ADC0_SE16,      // ADC0_SE16
    ADC0_SE17,      // PTE24
    ADC0_SE18,      // PTE25
    ADC0_DM0,       // ADC0_DM0
    ADC0_DM1,       // ADC0_DM1
    RES0,           // ����
    RES1,           // ����
    DAC0_OUT,       // DAC0��� ��֧��ADC
    RES2,           // ����
    RES3,           // ����
    Temp0_Sensor,   // Temperature Sensor,�ڲ��¶Ȳ���������ADC����
    Bandgap0,       // �¶Ȳ����ṹ��϶��׼Դ   ��֧��ADC
    RES4,           // ����
    VREFH0,         // �ο��ߵ�ѹ,����ADC���� ,�����Ϊ 2^n-1
    VREFL0,         // �ο��͵�ѹ,����ADC���� ,�����Ϊ 0
    Module0_Dis,    // ��֧�� ADC

    // ---------------------------------ADC1-------------------------
    ADC1_DP0,
    ADC1_DP1,
    PGA1_DP,        // ��֧�� ADC
    ADC1_DP3,
    ADC1_SE4a,      // PTE0
    ADC1_SE5a,      // PTE1
    ADC1_SE6a,      // PTE2
    ADC1_SE7a,      // PTE3

    ADC1_SE4b = ADC1_SE4a,  // PTC8     ��֧�����ADC ,���� ADC1_SE4b �����ADC������ᵱ�� ADC1_SE4a ����
    ADC1_SE5b = ADC1_SE5a,  // PTC9     ��֧�����ADC
    ADC1_SE6b = ADC1_SE6a,  // PTC10    ��֧�����ADC
    ADC1_SE7b = ADC1_SE7a,  // PTC11    ��֧�����ADC

    ADC1_SE8,       // PTB0
    ADC1_SE9,       // PTB1
    ADC1_SE10,      // PTB4
    ADC1_SE11,      // PTB5
    ADC1_SE12,      // PTB6
    ADC1_SE13,      // PTB7
    ADC1_SE14,      // PTB10
    ADC1_SE15,      // PTB11
    ADC1_SE16,      // ADC1_SE16
    ADC1_SE17,      // PTA17
    VREF_OUTPUT,    // VREF Output
    ADC1_DM0,       // ADC1_DM0
    ADC1_DM1,       // ADC1_DM1
    RES5,           //����
    RES6,
    DAC1_OUT,
    RES7,           //����
    RES8,
    Temp1_Sensor,
    Bandgap1,       // �¶Ȳ����ṹ��϶��׼Դ   ��֧��ADC
    RES9,
    VREFH1,         // �ο��ߵ�ѹ,����ADC���� ,�����Ϊ 2^n-1
    VREFL1,         // �ο��͵�ѹ,����ADC���� ,�����Ϊ 0
    Module1_Dis,    // ��֧�� ADC

} ADCn_Ch_e;

typedef enum  //ADCģ��
{
    ADC0,
    ADC1
} ADCn_e;

//����λ��
typedef enum ADC_nbit
{
    ADC_8bit   = 0x00,
    ADC_10bit  = 0x02,
    ADC_12bit  = 0x01,
    ADC_16bit  = 0x03
} ADC_nbit;


//�ⲿ�����ӿ�����
extern void     adc_init    (ADCn_Ch_e);                //ADC��ʼ��
extern uint16   adc_once    (ADCn_Ch_e, ADC_nbit);      //�ɼ�һ��һ·ģ������ADֵ

extern void     adc_stop    (ADCn_e);                   //ֹͣADCת��


#endif /* __MK60_ADC16_H__ */
