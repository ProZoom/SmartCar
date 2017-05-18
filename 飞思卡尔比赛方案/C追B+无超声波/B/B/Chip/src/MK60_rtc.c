/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_rtc.c
 * @brief      rtcģ�麯��
 * @author     ɽ��Ƽ�
 * @version    v5.1
 * @date       2014-04-25
 */

#include "common.h"
#include "MK60_rtc.h"

/*!
 *  @brief      RTC��ʼ��
 *  @since      v5.0
 *  Sample usage:       rtc_init();    //RTC��ʼ��
 */
void rtc_init(void)
{
    volatile uint32 delay;

    SIM_SCGC6 |= SIM_SCGC6_RTC_MASK;    //���� RTC ʱ��

    RTC_CR  = RTC_CR_SWR_MASK;          //��λ RTC �Ĵ������� SWR�� RTC_WAR �� RTC_RAR ��
    RTC_CR  &= ~RTC_CR_SWR_MASK;        //��ո�λ��־λ
    RTC_CR  = (0
               | RTC_CR_OSCE_MASK       //32.768 kHz ���� ʹ��
               //| RTC_CR_SC2P_MASK     //���� 2p ����
               //| RTC_CR_SC4P_MASK     //���� 4p ����
               //| RTC_CR_SC8P_MASK     //���� 8p ����
               | RTC_CR_SC16P_MASK      //���� 16p ����
               | RTC_CR_CLKO_MASK       //RTC_CLKOUT ��� 32k ʹ�� (0��ʾ�����1��ʾ��ֹ)
              );

    delay = 0x600000;
    while(delay--);                     //�ȴ� 32K �����ȶ�������ʱ����Ҫ�������ֲᣩ

    //����ʱ�䲹��
    RTC_TCR = (0
               | RTC_TCR_CIR(0)         //�������(���Դ�1��(0X0)��256(0xFF)�ķ�Χ��),8bit
               | RTC_TCR_TCR(0)         //����ֵ�ķ�Χ�� 32*1024Hz -127 �����ڵ� 32*1024Hz +128 ���ڣ��� TCR ��ΧΪ (int8)-127 ~ (int8)128
              );

    RTC_SR &= RTC_SR_TCE_MASK;          //����RTC �����������ں������üĴ���  TSR �� TPR

    //ʱ�����������
    RTC_TSR = 0;                        //��ǰʱ��
    RTC_TAR = 0;                        //����ʱ��

    //�ж�����
    RTC_IER = (0
               //| RTC_IER_TAIE_MASK    //�����ж�ʹ�ܣ�0��ʾ��ֹ��1��ʾʹ�ܣ�
               //| RTC_IER_TOIE_MASK    //����ж�ʹ�ܣ�0��ʾ��ֹ��1��ʾʹ�ܣ�
               //| RTC_IER_TIIE_MASK    //��Чʱ���ж�ʹ�ܣ�0��ʾ��ֹ��1��ʾʹ�ܣ�
              );

    RTC_SR |= RTC_SR_TCE_MASK;          //ʹ��RTC ������
}

/*!
 *  @brief      ���õ�ǰʱ��
 *  @since      v5.0
 */
void rtc_set_time(uint32 seconds)
{
    RTC_SR &= ~RTC_SR_TCE_MASK;         //����RTC �����������ں������üĴ���  TSR �� TPR
    RTC_TSR = seconds;                  //��ǰʱ��
    RTC_SR |= RTC_SR_TCE_MASK;          //ʹ��RTC ������

}

/*!
 *  @brief      ��ȡ��ǰʱ��
 *  @since      v5.0
 */
uint32 rtc_get_time(void)
{
    return RTC_TSR;
}


/*!
 *  @brief      ����RTC����ʱ�䣬ʹ�������ж�
 *  @param      alarm    ����ʱ��
 *  @return     �������ӽ��(0��ʾʧ�ܣ�1��ʾ�ɹ�)
 *  @since      v5.0
 *  Sample usage:       if( rtc_set_alarm(sec) == 0 )       //��������ʱ�� Ϊ sec
                        {
                            printf("\n��������ʧ�ܣ��������ù�ȥ��ʱ��Ϊ����ʱ��!");
                        }
 */
uint8 rtc_set_alarm(uint32 alarm)
{
    if(alarm < RTC_TSR)                 //����ʱ�䲻��Ϊ��ȥʱ��
    {
        return 0;
    }

    RTC_SR &= ~RTC_SR_TCE_MASK;         //����RTC �����������ں������üĴ���  TSR �� TPR

    RTC_TAR = alarm;                    //����ʱ��
    RTC_SR |= RTC_SR_TCE_MASK;          //ʹ��RTC ������

    RTC_IER |= RTC_IER_TAIE_MASK;       //ʹ�������ж�

    return 1;
}

/*!
 *  @brief      �ر������ж�
 *  @since      v5.0
 */
void rtc_close_alarm()
{
    RTC_IER &= ~RTC_IER_TAIE_MASK;      //��ֹ�����ж�
}

/*!
 *  @brief      RTC�����жϷ�����
 *  @since      v5.0
 *  @warning    �˺�����Ҫ�û������Լ�������ɣ�����������ṩһ��ģ��
 *  Sample usage:       set_vector_handler(RTC_VECTORn , rtc_test_handler);    //�� rtc_test_handler ������ӵ��ж�����������Ҫ�����ֶ�����
 */
void rtc_test_handler(void)
{
    uint32 sr = RTC_SR;

    if(sr & RTC_SR_TIF_MASK) //��Ч����
    {
        //��TIF��TOFλ�����ü���ʱдTSR�Ĵ����ͻ���մ�λ
        RTC_SR = (0
                  //| RTC_SR_TCE_MASK    //����ʹ��(0Ϊ��ֹ��1Ϊʹ��)
                  );
        RTC_TSR = 0;  //������Ĵ�����д���������TIFλ

        //�������û������Լ��Ĵ���
        //printf("\nRTC��Ч����");
    }

    if(sr & RTC_SR_TOF_MASK)  //���
    {

        //��TIF��TOFλ�����ü���ʱдTSR�Ĵ����ͻ���մ�λ
        RTC_SR = (0
                  //| RTC_SR_TCE_MASK    //����ʹ��(0Ϊ��ֹ��1Ϊʹ��)
                  );
        RTC_TSR = 0;  //������Ĵ�����д���������TIFλ

        //�������û������Լ��Ĵ���
        //printf("\nRTC���");
    }

    if(sr & RTC_SR_TAF_MASK)  //����
    {
        //д TAR ����� SR_TAF
        RTC_TAR = 0;

        //�������û������Լ��Ĵ���
        //printf("\nRTC����");
    }




}
