/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_lptmr.c
 * @brief      lptmr����������������ʱ�������������ʱ���ܴ�ʵ�֣�
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-06-26
 */


#include "common.h"
#include "MK60_lptmr.h"

/*!
 *  @brief      LPTMR���������ʼ��
 *  @param      LPT0_ALTn   LPTMR��������ܽ�
 *  @param      count       LPTMR����Ƚ�ֵ
 *  @param      LPT_CFG     LPTMR���������ʽ�������ؼ������½��ؼ���
 *  @since      v5.0
 *  Sample usage:       lptmr_pulse_init(LPT0_ALT1,0xFFFF,LPT_Rising);     // LPTMR �������������0xFFFF�󴥷��ж�������Ҫ���жϲ�ִ���жϷ��������������ز�׽
 */
void lptmr_pulse_init(LPT0_ALTn altn, uint16 count, LPT_CFG cfg)
{

#if  defined(MK60F15)
    OSC0_CR |= OSC_CR_ERCLKEN_MASK;                              //ʹ�� OSCERCLK
#endif

    // ����ģ��ʱ��
    SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK;                        //ʹ��LPTģ��ʱ��

    //��������ܽ�
    if(altn == LPT0_ALT1)
    {
        port_init(PTA19, ALT6 );            //��PTA19��ʹ�� ALT6
    }
    else if(altn == LPT0_ALT2)
    {
        port_init(PTC5, ALT4 );             //��PTC5��ʹ�� ALT4
    }
    else                                    //�����ܷ����¼�
    {
        ASSERT((altn == LPT0_ALT1 ) || (altn == LPT0_ALT2 ));   //���ùܽ�����
    }

    // ��״̬�Ĵ���
    LPTMR0_CSR = 0x00;                                          //�ȹ���LPT��������������ʱ�ӷ�Ƶ,��ռ���ֵ��

#if defined(MK60DZ10)
    //ѡ��ʱ��Դ
    LPTMR0_PSR  =   ( 0
                      | LPTMR_PSR_PCS(1)                  //ѡ��ʱ��Դ�� 0 Ϊ MCGIRCLK ��1Ϊ LPO��1KHz�� ��2Ϊ ERCLK32K ��3Ϊ OSCERCLK
                      | LPTMR_PSR_PBYP_MASK               //��· Ԥ��Ƶ/�����˲��� ,������ Ԥ��Ƶ/�����˲���(ע���˱�ʾʹ��Ԥ��Ƶ/�����˲���)
                      //| LPTMR_PSR_PRESCALE(1)           //Ԥ��Ƶֵ = 2^(n+1) ,n = 0~ 0xF
                    );
#elif defined(MK60F15)
    //ѡ��ʱ��Դ
    LPTMR0_PSR  =   ( 0
                      | LPTMR_PSR_PCS(3)          //ѡ��ʱ��Դ�� 0 Ϊ MCGIRCLK ��1Ϊ LPO��1KHz�� ��2Ϊ ERCLK32K ��3Ϊ OSCERCLK
                      //| LPTMR_PSR_PBYP_MASK     //��· Ԥ��Ƶ/�����˲��� ,������ Ԥ��Ƶ/�����˲���(ע���˱�ʾʹ��Ԥ��Ƶ/�����˲���)
                      | LPTMR_PSR_PRESCALE(4)     //Ԥ��Ƶֵ = 2^(n+1) ,n = 0~ 0xF
                    );
#endif

    // �����ۼӼ���ֵ
    LPTMR0_CMR  =   LPTMR_CMR_COMPARE(count);                   //���ñȽ�ֵ

    // �ܽ����á�ʹ���ж�
    LPTMR0_CSR  =  (0
                    | LPTMR_CSR_TPS(altn)       // ѡ������ܽ� ѡ��
                    | LPTMR_CSR_TMS_MASK        // ѡ��������� (ע���˱�ʾʱ�����ģʽ)
                    | ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //���������������ʽѡ��0Ϊ�ߵ�ƽ��Ч�������ؼ�1
                    | LPTMR_CSR_TEN_MASK        //ʹ��LPT(ע���˱�ʾ����)
                    | LPTMR_CSR_TIE_MASK        //�ж�ʹ��
                    //| LPTMR_CSR_TFC_MASK      //0:����ֵ���ڱȽ�ֵ�͸�λ��1�������λ��ע�ͱ�ʾ0��
                   );
}

/*!
 *  @brief      ��ȡLPTMR�������ֵ
 *  @return     �������ֵ
 *  @since      v5.0
 *  Sample usage:       uint16 data = lptmr_pulse_get();  //��ȡ�������ֵ
 */
uint16 lptmr_pulse_get(void)
{
    uint16 data;
    if(LPTMR0_CSR & LPTMR_CSR_TCF_MASK)     //�Ѿ������
    {

        data = ~0;                          //���� 0xffffffff ��ʾ����
    }
    else
    {
#if defined(MK60F15)
        LPTMR0_CNR = 0;
#endif
        data = LPTMR0_CNR;
    }
    return data;
}


/*!
 *  @brief      ���LPTMR�������
 *  @since      v5.0
 *  Sample usage:       lptmr_counter_clean();          //���LPTMR�������
 */
void lptmr_pulse_clean(void)
{
    LPTMR0_CSR  &= ~LPTMR_CSR_TEN_MASK;     //����LPT��ʱ��ͻ��Զ����������ֵ
    LPTMR0_CSR  |= LPTMR_CSR_TEN_MASK;
}

/*!
 *  @brief      LPTMR��ʱ������ms��
 *  @param      ms          LPTMR��ʱʱ��(0~65535)
 *  @since      v5.0
 *  Sample usage:       lptmr_delay_ms(32);     // LPTMR ��ʱ32ms
 */
void lptmr_delay_ms(uint16 ms)
{
    if(ms == 0)
    {
        return;
    }

    SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK;    //ʹ��LPTģ��ʱ��

    LPTMR0_CSR = 0x00;                      //�ȹ���LPT���Զ����������ֵ

    LPTMR0_CMR = ms;                        //���ñȽ�ֵ������ʱʱ��

    //ѡ��ʱ��Դ
    LPTMR0_PSR  =   ( 0
                      | LPTMR_PSR_PCS(1)                  //ѡ��ʱ��Դ�� 0 Ϊ MCGIRCLK ��1Ϊ LPO��1KHz�� ��2Ϊ ERCLK32K ��3Ϊ OSCERCLK
                      | LPTMR_PSR_PBYP_MASK               //��· Ԥ��Ƶ/�����˲��� ,������ Ԥ��Ƶ/�����˲���(ע���˱�ʾʹ��Ԥ��Ƶ/�����˲���)
                      //| LPTMR_PSR_PRESCALE(1)           //Ԥ��Ƶֵ = 2^(n+1) ,n = 0~ 0xF
                    );

    //ʹ�� LPT
    LPTMR0_CSR  =  (0
                    //| LPTMR_CSR_TPS(1)        // ѡ������ܽ� ѡ��
                    //| LPTMR_CSR_TMS_MASK      // ѡ��������� (ע���˱�ʾʱ�����ģʽ)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //���������������ʽѡ��0Ϊ�ߵ�ƽ��Ч�������ؼ�1
                    | LPTMR_CSR_TEN_MASK        //ʹ��LPT(ע���˱�ʾ����)
                    //| LPTMR_CSR_TIE_MASK      //�ж�ʹ��
                    //| LPTMR_CSR_TFC_MASK      //0:����ֵ���ڱȽ�ֵ�͸�λ��1�������λ��ע�ͱ�ʾ0��
                   );

    while (!(LPTMR0_CSR & LPTMR_CSR_TCF_MASK)); //�ȴ��Ƚ�ֵ�����ֵ��ȣ���ʱ�䵽��

    LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;          //����Ƚϱ�־λ

    return;
}

/*!
 *  @brief      LPTMR��ʱ������ms��
 *  @param      ms          LPTMR��ʱʱ��(0~65535)
 *  @since      v5.0
 *  Sample usage:
                    lptmr_timing_ms(32);                                // LPTMR ��ʱ 32ms
                    set_vector_handler(LPTimer_VECTORn,lptmr_hander);   // �����жϷ��������ж���������
                    enable_irq(LPTimer_IRQn);                           // ʹ��LPTMR�ж�
 */
void lptmr_timing_ms(uint16 ms)
{
    if(ms == 0)
    {
        return;
    }

    SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK;    //ʹ��LPTģ��ʱ��

    LPTMR0_CSR = 0x00;                      //�ȹ���LPT���Զ����������ֵ

    LPTMR0_CMR = ms;                        //���ñȽ�ֵ������ʱʱ��

    //ѡ��ʱ��Դ
    LPTMR0_PSR  =   ( 0
                      | LPTMR_PSR_PCS(1)                  //ѡ��ʱ��Դ�� 0 Ϊ MCGIRCLK ��1Ϊ LPO��1KHz�� ��2Ϊ ERCLK32K ��3Ϊ OSCERCLK
                      | LPTMR_PSR_PBYP_MASK               //��· Ԥ��Ƶ/�����˲��� ,������ Ԥ��Ƶ/�����˲���(ע���˱�ʾʹ��Ԥ��Ƶ/�����˲���)
                      //| LPTMR_PSR_PRESCALE(1)           //Ԥ��Ƶֵ = 2^(n+1) ,n = 0~ 0xF
                    );

    //ʹ�� LPT
    LPTMR0_CSR  =  (0
                    //| LPTMR_CSR_TPS(1)        // ѡ������ܽ� ѡ��
                    //| LPTMR_CSR_TMS_MASK      // ѡ��������� (ע���˱�ʾʱ�����ģʽ)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //���������������ʽѡ��0Ϊ�ߵ�ƽ��Ч�������ؼ�1
                    | LPTMR_CSR_TEN_MASK        //ʹ��LPT(ע���˱�ʾ����)
                    | LPTMR_CSR_TIE_MASK        //�ж�ʹ��
                    //| LPTMR_CSR_TFC_MASK      //0:����ֵ���ڱȽ�ֵ�͸�λ��1�������λ��ע�ͱ�ʾ0��
                   );

    return;
}

/*!
 *  @brief      LPTMR��ʱ������us��
 *  @param      ms          LPTMR��ʱʱ��(0~41942)
 *  @since      v5.0
 *  Sample usage:       lptmr_delay_us(32);     // LPTMR ��ʱ32us
 */
void lptmr_delay_us(uint16 us)
{
    ASSERT( ( (us * EXTAL_IN_MHz + 16) /  32 )  <= 0xFFFF );    //���ԣ�ȷ���Ĵ��������
    //us * ����Ƶ��  /32 <= 0xFFFF
    //���� 32 ��������ļ��������� ��Ƶ
    //���� 16��Ϊ�� ��������

    if(us == 0)
    {
        return;
    }
#if defined(MK60DZ10)
    OSC_CR |= OSC_CR_ERCLKEN_MASK;                              //ʹ�� OSCERCLK
#elif defined(MK60F15)
    OSC0_CR |= OSC_CR_ERCLKEN_MASK;                              //ʹ�� OSCERCLK
#endif

    SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK;                        //ʹ��LPTģ��ʱ��

    LPTMR0_CSR = 0x00;                                          //�ȹ���LPT���Զ����������ֵ

    LPTMR0_CMR = (us * EXTAL_IN_MHz + 16) / 32;                 //���ñȽ�ֵ������ʱʱ��

    //ѡ��ʱ��Դ
    LPTMR0_PSR  =   ( 0
                      | LPTMR_PSR_PCS(3)          //ѡ��ʱ��Դ�� 0 Ϊ MCGIRCLK ��1Ϊ LPO��1KHz�� ��2Ϊ ERCLK32K ��3Ϊ OSCERCLK
                      //| LPTMR_PSR_PBYP_MASK     //��· Ԥ��Ƶ/�����˲��� ,������ Ԥ��Ƶ/�����˲���(ע���˱�ʾʹ��Ԥ��Ƶ/�����˲���)
                      | LPTMR_PSR_PRESCALE(4)     //Ԥ��Ƶֵ = 2^(n+1) ,n = 0~ 0xF
                    );

    //ʹ�� LPT
    LPTMR0_CSR  =  (0
                    //| LPTMR_CSR_TPS(1)        // ѡ������ܽ� ѡ��
                    //| LPTMR_CSR_TMS_MASK      // ѡ��������� (ע���˱�ʾʱ�����ģʽ)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //���������������ʽѡ��0Ϊ�ߵ�ƽ��Ч�������ؼ�1
                    | LPTMR_CSR_TEN_MASK        //ʹ��LPT(ע���˱�ʾ����)
                    //| LPTMR_CSR_TIE_MASK      //�ж�ʹ��
                    //| LPTMR_CSR_TFC_MASK      //0:����ֵ���ڱȽ�ֵ�͸�λ��1�������λ��ע�ͱ�ʾ0��
                   );

    while (!(LPTMR0_CSR & LPTMR_CSR_TCF_MASK)); //�ȴ��Ƚ�ֵ�����ֵ��ȣ���ʱ�䵽��

    LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;          //����Ƚϱ�־λ

    return;
}



/*!
 *  @brief      LPTMR��ʱ������us��
 *  @param      ms          LPTMR��ʱʱ��(0~41942)
 *  @since      v5.0
 *  Sample usage:
                    lptmr_timing_us(32);     // LPTMR ��ʱ32us
                    set_vector_handler(LPTimer_VECTORn,lptmr_hander);   // �����жϷ��������ж���������
                    enable_irq(LPTimer_IRQn);                           // ʹ��LPTMR�ж�
 */
void lptmr_timing_us(uint16 us)
{
    ASSERT( ( (us * EXTAL_IN_MHz + 16) /  32 )  <= 0xFFFF );    //���ԣ�ȷ���Ĵ��������
    //us * ����Ƶ��  /32 <= 0xFFFF
    //���� 32 ��������ļ��������� ��Ƶ
    //���� 16��Ϊ�� ��������

    if(us == 0)
    {
        return;
    }

#if defined(MK60DZ10)
    OSC_CR |= OSC_CR_ERCLKEN_MASK;                              //ʹ�� OSCERCLK
#elif defined(MK60F15)
    OSC0_CR |= OSC_CR_ERCLKEN_MASK;                              //ʹ�� OSCERCLK
#endif

    SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK;                        //ʹ��LPTģ��ʱ��

    LPTMR0_CSR = 0x00;                                          //�ȹ���LPT���Զ����������ֵ

    LPTMR0_CMR = (us * EXTAL_IN_MHz + 16) / 32;                 //���ñȽ�ֵ������ʱʱ��

    //ѡ��ʱ��Դ
    LPTMR0_PSR  =   ( 0
                      | LPTMR_PSR_PCS(3)          //ѡ��ʱ��Դ�� 0 Ϊ MCGIRCLK ��1Ϊ LPO��1KHz�� ��2Ϊ ERCLK32K ��3Ϊ OSCERCLK
                      //| LPTMR_PSR_PBYP_MASK     //��· Ԥ��Ƶ/�����˲��� ,������ Ԥ��Ƶ/�����˲���(ע���˱�ʾʹ��Ԥ��Ƶ/�����˲���)
                      | LPTMR_PSR_PRESCALE(4)     //Ԥ��Ƶֵ = 2^(n+1) ,n = 0~ 0xF
                    );

    //ʹ�� LPT
    LPTMR0_CSR  =  (0
                    //| LPTMR_CSR_TPS(1)        // ѡ������ܽ� ѡ��
                    //| LPTMR_CSR_TMS_MASK      // ѡ��������� (ע���˱�ʾʱ�����ģʽ)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //���������������ʽѡ��0Ϊ�ߵ�ƽ��Ч�������ؼ�1
                    | LPTMR_CSR_TEN_MASK        //ʹ��LPT(ע���˱�ʾ����)
                    | LPTMR_CSR_TIE_MASK      //�ж�ʹ��
                    //| LPTMR_CSR_TFC_MASK      //0:����ֵ���ڱȽ�ֵ�͸�λ��1�������λ��ע�ͱ�ʾ0��
                   );

    return;
}



/*!
 *  @brief      LPTMR��ʱ������ms,��ʱʱ�����Ϊ65534ms��
 *  @since      v5.0
 *  Sample usage:
                    void my_delay(uint32 time)
                    {
                        volatile uint32 i = time;
                        while(i--);

                    }


                    lptmr_time_start_ms();

                    my_delay(600000);
                    i = lptmr_time_get_ms();
                    if(i == ~0)
                    {
                        printf("\n��ʱʱ�䳬ʱ");
                    }
                    else
                    {
                        printf("\n��ʱʱ��Ϊ��%dms",i);
                    }
 */
void lptmr_time_start_ms(void)
{
    SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK;    //ʹ��LPTģ��ʱ��

    LPTMR0_CSR = 0x00;                      //�ȹ���LPT���Զ����������ֵ

    LPTMR0_CMR = ~0;                        //���ñȽ�ֵ������ʱʱ��

    //ѡ��ʱ��Դ
    LPTMR0_PSR  =   ( 0
                      | LPTMR_PSR_PCS(1)                  //ѡ��ʱ��Դ�� 0 Ϊ MCGIRCLK ��1Ϊ LPO��1KHz�� ��2Ϊ ERCLK32K ��3Ϊ OSCERCLK
                      | LPTMR_PSR_PBYP_MASK               //��· Ԥ��Ƶ/�����˲��� ,������ Ԥ��Ƶ/�����˲���(ע���˱�ʾʹ��Ԥ��Ƶ/�����˲���)
                      //| LPTMR_PSR_PRESCALE(1)           //Ԥ��Ƶֵ = 2^(n+1) ,n = 0~ 0xF
                    );

    //ʹ�� LPT
    LPTMR0_CSR  =  (0
                    //| LPTMR_CSR_TPS(1)        // ѡ������ܽ� ѡ��
                    //| LPTMR_CSR_TMS_MASK      // ѡ��������� (ע���˱�ʾʱ�����ģʽ)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //���������������ʽѡ��0Ϊ�ߵ�ƽ��Ч�������ؼ�1
                    | LPTMR_CSR_TEN_MASK        //ʹ��LPT(ע���˱�ʾ����)
                    //| LPTMR_CSR_TIE_MASK      //�ж�ʹ��
                    //| LPTMR_CSR_TFC_MASK      //0:����ֵ���ڱȽ�ֵ�͸�λ��1�������λ��ע�ͱ�ʾ0��
                   );
    return;
}




/*!
 *  @brief      ��ȡLPTMR��ʱʱ�䣨ms��
*   @return     ��ʱʱ�䣨����ֵΪ ~0 ��ʾ��ʱ��ʱ������ֵ�� 0~ 65534ms ������ ��
 *  @since      v5.0
 *  Sample usage:   �ο� lptmr_time_start_ms �ĵ�������
 */
uint32 lptmr_time_get_ms(void)
{
    uint32 data;

    if(LPTMR0_CSR & LPTMR_CSR_TCF_MASK)     //�Ѿ������
    {

        data = ~0;                          //���� 0xffffffff ��ʾ����
    }
    else
    {
#if defined(MK60F15)
        LPTMR0_CNR = 0;
#endif
        data = LPTMR0_CNR;                  //���ؼ�ʱʱ��(��ֵ���Ϊ 0xffff)
    }

    return data;
}

/*!
 *  @brief      �ر� LPTMR��ʱ
 *  @since      v5.0
 */
void lptmr_time_close()
{
    LPTMR0_CSR = 0x00;                                          //�ȹ���LPT���Զ����������ֵ�����������
}

/*!
 *  @brief      LPTMR��ʱ������us,��ʱʱ�����Ϊ41942us��
 *  @since      v5.0
 *  Sample usage:
                    void my_delay(uint32 time)
                    {
                        volatile uint32 i = time;
                        while(i--);

                    }

                    uint32 i;
                    lptmr_time_start_us();

                    my_delay(5894);

                    i = lptmr_time_get_us();
                    if(i == ~0)
                    {
                        printf("\n��ʱʱ�䳬ʱ");
                    }
                    else
                    {
                        printf("\n��ʱʱ��Ϊ��%dus",i);
                    }
 */
void lptmr_time_start_us(void)
{

#if defined(MK60DZ10)
    OSC_CR |= OSC_CR_ERCLKEN_MASK;                              //ʹ�� OSCERCLK
#elif defined(MK60F15)
    OSC0_CR |= OSC_CR_ERCLKEN_MASK;                              //ʹ�� OSCERCLK
#endif

    SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK;                        //ʹ��LPTģ��ʱ��

    LPTMR0_CSR = 0x00;                                          //�ȹ���LPT���Զ����������ֵ

    LPTMR0_CMR = ~0;                                            //���ñȽ�ֵΪ���ֵ

    //ѡ��ʱ��Դ
    LPTMR0_PSR  =   ( 0
                      | LPTMR_PSR_PCS(3)          //ѡ��ʱ��Դ�� 0 Ϊ MCGIRCLK ��1Ϊ LPO��1KHz�� ��2Ϊ ERCLK32K ��3Ϊ OSCERCLK
                      //| LPTMR_PSR_PBYP_MASK     //��· Ԥ��Ƶ/�����˲��� ,������ Ԥ��Ƶ/�����˲���(ע���˱�ʾʹ��Ԥ��Ƶ/�����˲���)
                      | LPTMR_PSR_PRESCALE(4)     //Ԥ��Ƶֵ = 2^(n+1) ,n = 0~ 0xF
                    );

    //ʹ�� LPT
    LPTMR0_CSR  =  (0
                    //| LPTMR_CSR_TPS(1)        // ѡ������ܽ� ѡ��
                    //| LPTMR_CSR_TMS_MASK      // ѡ��������� (ע���˱�ʾʱ�����ģʽ)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //���������������ʽѡ��0Ϊ�ߵ�ƽ��Ч�������ؼ�1
                    | LPTMR_CSR_TEN_MASK        //ʹ��LPT(ע���˱�ʾ����)
                    //| LPTMR_CSR_TIE_MASK      //�ж�ʹ��
                    //| LPTMR_CSR_TFC_MASK      //0:����ֵ���ڱȽ�ֵ�͸�λ��1�������λ��ע�ͱ�ʾ0��
                   );

    return;
}

/*!
 *  @brief      ��ȡLPTMR��ʱʱ�䣨us��
*   @return     ��ʱʱ�䣨����ֵΪ ~0 ��ʾ��ʱ��ʱ������ֵ�� 0~ 41942us ������ ��
 *  @since      v5.0
 *  Sample usage:   �ο� lptmr_time_start_us �ĵ�������
 */
uint32 lptmr_time_get_us(void)
{
    uint32 data;

    if(LPTMR0_CSR & LPTMR_CSR_TCF_MASK)     //�Ѿ������
    {

        data = ~0;                          //���� 0xffffffff ��ʾ����
    }
    else
    {
#if defined(MK60F15)
        LPTMR0_CNR = 0;
#endif
        data = (LPTMR0_CNR * 32) / EXTAL_IN_MHz; //���е�λ����
    }

    return data;
}


/*!
 *  @brief      LPTMR�жϷ�����
 *  @since      v5.0
 *  @warning    �˺�����Ҫ�û������Լ�������ɣ�����������ṩһ��ģ��
 *  Sample usage:       set_vector_handler(LPTimer_VECTORn , lptmr_handler);    //�� lptmr_handler ������ӵ��ж�����������Ҫ�����ֶ�����
 */
void lptmr_test_handler(void)
{
    LPTMR_Flag_Clear();

    //�������û����ʵ�ִ���


}

