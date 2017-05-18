/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_adc.c
 * @brief      ADC����
 * @author     ɽ��Ƽ�
 * @version    v5.1
 * @date       2014-01-16
 */

#include "common.h"
#include "MK60_adc.h"

ADC_MemMapPtr ADCN[2] = {ADC0_BASE_PTR, ADC1_BASE_PTR}; //��������ָ�����鱣�� ADCN �ĵ�ַ

static void     adc_start   (ADCn_Ch_e, ADC_nbit);    //��ʼadcת��

/*!
 *  @brief      ADC��ʼ��
 *  @param      ADCn_Ch_e    ADCͨ��
 *  @since      v5.0
 *  @note       �˳�ʼ����֧���������������ÿ��ͨ����֧��ADC ���������
                ����˵���� ADCn_Ch_e ��ע��˵��
 *  Sample usage:       adc_init (ADC0_SE10 );    //��ʼ�� ADC0_SE10 ��ʹ�� PTA7 �ܽ�
 */
void adc_init(ADCn_Ch_e adcn_ch)
{

    uint8 adcn = adcn_ch >> 5 ;
    //uint8 ch = adcn_ch & 0x1F;

    switch(adcn)
    {
    case ADC0:       /*   ADC0  */
        SIM_SCGC6 |= (SIM_SCGC6_ADC0_MASK );        //����ADC0ʱ��
        SIM_SOPT7 &= ~(SIM_SOPT7_ADC0ALTTRGEN_MASK  | SIM_SOPT7_ADC0PRETRGSEL_MASK);
        SIM_SOPT7 |= SIM_SOPT7_ADC0TRGSEL(0);
        break;
    case ADC1:       /*   ADC1    */
        SIM_SCGC3 |= (SIM_SCGC3_ADC1_MASK );
        SIM_SOPT7 &= ~(SIM_SOPT7_ADC1ALTTRGEN_MASK  | SIM_SOPT7_ADC1PRETRGSEL_MASK) ;
        SIM_SOPT7 |= SIM_SOPT7_ADC1TRGSEL(0);
        break;
    default:
        ASSERT(0);
    }

    switch(adcn_ch)
    {
    case ADC0_SE8:       // PTB0
        port_init(PTB0, ALT0);
        break;
    case ADC0_SE9:       // PTB1
        port_init(PTB1, ALT0);
        break;
    case ADC0_SE10:      // PTA7
        port_init(PTA7, ALT0);
        break;
    case ADC0_SE11:      // PTA8
        port_init(PTA8, ALT0);
        break;
    case ADC0_SE12:      // PTB2
        port_init(PTB2, ALT0);
        break;
    case ADC0_SE13:      // PTB3
        port_init(PTB3, ALT0);
        break;
    case ADC0_SE14:      // PTC0
        port_init(PTC0, ALT0);
        break;
    case ADC0_SE15:      // PTC1
        port_init(PTC1, ALT0);
        break;
    case ADC0_SE17:      // PTE24
        port_init(PTE24, ALT0);
        break;
    case ADC0_SE18:      // PTE25
        port_init(PTE25, ALT0);
        break;

    case ADC0_DP0:
    case ADC0_DP1:
    case ADC0_DP3:
    case ADC0_DM0:       // ADC0_DM0
    case ADC0_DM1:       // ADC0_DM1
    case ADC0_SE16:      // ADC0_SE16
    case Temp0_Sensor:   // Temperature Sensor,�ڲ��¶Ȳ���������ADC����
    case VREFH0:         // �ο��ߵ�ѹ,����ADC���� ,�����Ϊ 2^n-1
    case VREFL0:         // �ο��͵�ѹ,����ADC���� ,�����Ϊ 0
        break;          //�ⲿ�ֹܽŲ������ø���

        // ---------------------------------ADC1-------------------------
    case ADC1_DP0:
    case ADC1_DP1:
    case ADC1_DP3:
        break;
    case ADC1_SE4a:      // PTE0
        port_init(PTE0, ALT0);
        break;
    case ADC1_SE5a:      // PTE1
        port_init(PTE1, ALT0);
        break;
    case ADC1_SE6a:      // PTE2
        port_init(PTE2, ALT0);
        break;
    case ADC1_SE7a:      // PTE3
        port_init(PTE3, ALT0);
        break;

    case ADC1_SE8:       // PTB0
        port_init(PTB0, ALT0);
        break;
    case ADC1_SE9:       // PTB1
        port_init(PTB1, ALT0);
        break;
    case ADC1_SE10:      // PTB4
        port_init(PTB4, ALT0);
        break;
    case ADC1_SE11:      // PTB5
        port_init(PTB5, ALT0);
        break;
    case ADC1_SE12:      // PTB6
        port_init(PTB6, ALT0);
        break;
    case ADC1_SE13:      // PTB7
        port_init(PTB7, ALT0);
        break;
    case ADC1_SE14:      // PTB10
        port_init(PTB10, ALT0);
        break;
    case ADC1_SE15:      // PTB11
        port_init(PTB11, ALT0);
        break;
    case ADC1_SE17:      // PTA17
        port_init(PTA17, ALT0);
        break;

    case ADC1_SE16:      // ADC1_SE16
    case VREF_OUTPUT:    // VREF Output
    case ADC1_DM0:       // ADC1_DM0
    case ADC1_DM1:       // ADC1_DM1
    case Temp1_Sensor:
    case VREFH1:         // �ο��ߵ�ѹ,����ADC���� ,�����Ϊ 2^n-1
    case VREFL1:         // �ο��͵�ѹ,����ADC���� ,�����Ϊ 0
        break;

    default:
        ASSERT(0);      //���ԣ����ݵĹܽŲ�֧�� ADC ��������������뻻 �����ܽ�
        break;
    }
}

/*!
 *  @brief      ��ȡADC����ֵ(��֧��Bͨ��)
 *  @param      ADCn_Ch_e    ADCͨ��
 *  @param      ADC_nbit     ADC���ȣ� ADC_8bit,ADC_12bit, ADC_10bit, ADC_16bit ��
 *  @return     ����ֵ
 *  @since      v5.0
 *  Sample usage:       uint16 var = adc_once(ADC0_SE10, ADC_8bit);
 */
uint16 adc_once(ADCn_Ch_e adcn_ch, ADC_nbit bit) //�ɼ�ĳ·ģ������ADֵ
{
    ADCn_e adcn = (ADCn_e)(adcn_ch >> 5) ;

    uint16 result = 0;

    adc_start(adcn_ch, bit);      //����ADCת��

    while (( ADC_SC1_REG(ADCN[adcn], 0 ) & ADC_SC1_COCO_MASK ) != ADC_SC1_COCO_MASK);   //ֻ֧�� Aͨ��
    result = ADC_R_REG(ADCN[adcn], 0);
    ADC_SC1_REG(ADCN[adcn], 0) &= ~ADC_SC1_COCO_MASK;
    return result;
}

/*!
 *  @brief      ����ADC�������(��֧��Bͨ��)
 *  @param      ADCn_Ch_e    ADCͨ��
 *  @param      ADC_nbit     ADC���ȣ� ADC_8bit,ADC_12bit, ADC_10bit, ADC_16bit ��
 *  @since      v5.0
 *  @note       �˺����ڲ����ã������󼴿ɵȴ����ݲɼ����
 *  Sample usage:       adc_start(ADC0_SE10, ADC_8bit);
 */
void adc_start(ADCn_Ch_e adcn_ch, ADC_nbit bit)
{
    ADCn_e adcn = (ADCn_e)(adcn_ch >> 5) ;
    uint8 ch = (uint8)(adcn_ch & 0x1F);

    //��ʼ��ADCĬ������
    ADC_CFG1_REG(ADCN[adcn]) = (0
                                //| ADC_CFG1_ADLPC_MASK         //ADC�������ã�0Ϊ�������ģ�1Ϊ�͹���
                                | ADC_CFG1_ADIV(2)              //ʱ�ӷ�Ƶѡ��,��Ƶϵ��Ϊ 2^n,2bit
                                | ADC_CFG1_ADLSMP_MASK          //����ʱ�����ã�0Ϊ�̲���ʱ�䣬1 Ϊ������ʱ��
                                | ADC_CFG1_MODE(bit)
                                | ADC_CFG1_ADICLK(0)            //0Ϊ����ʱ��,1Ϊ����ʱ��/2,2Ϊ����ʱ�ӣ�ALTCLK����3Ϊ �첽ʱ�ӣ�ADACK����
                               );


    ADC_CFG2_REG(ADCN[adcn])  = (0
                                 //| ADC_CFG2_MUXSEL_MASK       //ADC����ѡ��,0Ϊaͨ����1Ϊbͨ����
                                 //| ADC_CFG2_ADACKEN_MASK      //�첽ʱ�����ʹ��,0Ϊ��ֹ��1Ϊʹ�ܡ�
                                 | ADC_CFG2_ADHSC_MASK          //��������,0Ϊ����ת�����У�1Ϊ����ת������
                                 | ADC_CFG2_ADLSTS(0)           //������ʱ��ѡ��ADCKΪ4+n������ѭ��������ѭ����0Ϊ20��1Ϊ12��2Ϊ6��3Ϊ2
                                );

    //д�� SC1A ����ת��
    ADC_SC1_REG(ADCN[adcn], 0 ) = (0
                                   | ADC_SC1_AIEN_MASK          // ת������ж�,0Ϊ��ֹ��1Ϊʹ��
                                   //| ADC_SC1_DIFF_MASK        // ���ģʽʹ��,0Ϊ���ˣ�1Ϊ���
                                   | ADC_SC1_ADCH( ch )
                                  );

}

/*!
 *  @brief      ֹͣADC�������
 *  @param      ADCn_e       ADCģ��ţ� ADC0�� ADC1��
 *  @since      v5.0
 *  Sample usage:       adc_stop(ADC0);
 */
void adc_stop(ADCn_e adcn)
{
    ADC_SC1_REG(ADCN[adcn], 0) = (0
                                  | ADC_SC1_AIEN_MASK                       // ת������ж�,0Ϊ��ֹ��1Ϊʹ��
                                  //| ADC_SC1_DIFF_MASK                     // ���ģʽʹ��,0Ϊ���ˣ�1Ϊ���
                                  | ADC_SC1_ADCH(Module0_Dis)               //����ͨ��ѡ��,�˴�ѡ���ֹͨ��
                                 );
}

