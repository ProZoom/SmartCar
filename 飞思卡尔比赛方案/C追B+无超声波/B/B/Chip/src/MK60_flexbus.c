/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_flexbus.c
 * @brief      flexbus������
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-09-02
 */

#include "common.h"
#include "MK60_flexbus.h"


/*!
 *  @brief      flexbus��ʼ��Ϊ8080Э��
 *  @since      v5.0
 */
void flexbus_8080_init()
{
    //flexbus ����ֱ��֧�� 8080Э�飬����֧�� 6800Э��
    //���ǿ������� cmd �� data ���ݵĵ�ַ��ͬ���Ӷ�ͨ����ַ�����������������

    SIM_SOPT2 |= SIM_SOPT2_FBSL(3);             //FlexBus��ȫˮƽ:����ָ������ݷ���
    SIM_SCGC7 |= SIM_SCGC7_FLEXBUS_MASK;        //ʹ��ʱ��

    //�������ݹܽŸ���
    port_init(PTD6  , ALT5 | HDS);              //PTD6 ����Ϊ fb_ad[0] ,����Ϊ �������������
    port_init(PTD5  , ALT5 | HDS);              //PTD5 ����Ϊ fb_ad[1] ,����Ϊ �������������
    port_init(PTD4  , ALT5 | HDS);              //PTD4 ����Ϊ fb_ad[2] ,����Ϊ �������������
    port_init(PTD3  , ALT5 | HDS);              //PTD3 ����Ϊ fb_ad[3] ,����Ϊ �������������
    port_init(PTD2  , ALT5 | HDS);              //PTD2 ����Ϊ fb_ad[4] ,����Ϊ �������������
    port_init(PTC10 , ALT5 | HDS);              //PTC10����Ϊ fb_ad[5] ,����Ϊ �������������
    port_init(PTC9  , ALT5 | HDS);              //PTC9 ����Ϊ fb_ad[6] ,����Ϊ �������������
    port_init(PTC8  , ALT5 | HDS);              //PTC8 ����Ϊ fb_ad[7] ,����Ϊ �������������
    port_init(PTC7  , ALT5 | HDS);              //PTC7 ����Ϊ fb_ad[8] ,����Ϊ �������������
    port_init(PTC6  , ALT5 | HDS);              //PTC6 ����Ϊ fb_ad[9] ,����Ϊ �������������
    port_init(PTC5  , ALT5 | HDS);              //PTC5 ����Ϊ fb_ad[10],����Ϊ �������������
    port_init(PTC4  , ALT5 | HDS);              //PTC4 ����Ϊ fb_ad[11],����Ϊ �������������
    port_init(PTC2  , ALT5 | HDS);              //PTC2 ����Ϊ fb_ad[12],����Ϊ �������������
    port_init(PTC1  , ALT5 | HDS);              //PTC1 ����Ϊ fb_ad[13],����Ϊ �������������
    port_init(PTC0  , ALT5 | HDS);              //PTC0 ����Ϊ fb_ad[14],����Ϊ �������������
    port_init(PTB18 , ALT5 | HDS);              //PTB18����Ϊ fb_ad[15],����Ϊ �������������

    //���ÿ��ƹܽŸ���
    //8080����,��Intel����,��Ҫ�ĸ��߿����ߣ�RDдʹ��, WR��ʹ��, RS����/ָ��ѡ��, CSƬѡ
    port_init(PTB19 , ALT5 | HDS);              //PTB19����Ϊ fb_oe_b  ,����Ϊ ������������� , fb_oe_b ʱ����� 8080 ���ߵ� RDдʹ��
    port_init(PTD1  , ALT5 | HDS);              //PTD1 ����Ϊ fb_cs0_b ,����Ϊ ������������� , fb_cs0_bʱ����� 8080 ���ߵ� CSƬѡ
    port_init(PTC11 , ALT5 | HDS);              //PTC11����Ϊ fb_r/w   ,����Ϊ ������������� , fb_r/w  ʱ����� 8080 ���ߵ� WR��ʹ��

    //Ŀǰ��ȱ 8080 ���ߵ� RS����/ָ��ѡ����
    //flexbus�����ó������ַ�������Ŀ飨��16λ��ַ����ͬ�����Ӷ����õ�ַ�������� 8080 ���ߵ� RS����/ָ��ѡ����
    //��16λ�ĵ�ַ��FB_BA ��������Ϊ 0x6000 �������ѡ��� ��ַ���� fb_ad[16] ~ fb_ad[28]
    //FB_BAM����Ϊ 0x0800 ����ѡ�� fb_ad[27] ��Ϊ RS ��(0x0800 0000 == 1<<27 )
    // RS == 1 ʱ �������ݣ�RS == 0 ʱ��������
    //��� 0x6000 0000 Ϊ �����ַ �� 0x6800 0000 Ϊ���ݶ˿�
    port_init(PTC12 , ALT5 | HDS);              //PTC12����Ϊ fb_ad[27],����Ϊ ������������� , fb_ad[27]��Ϊ8080 ���ߵ� RS����/ָ��ѡ����

    FB_CSAR(0) = FB_CSAR_BA(FB_BA);             // ����ַ Base address
    FB_CSMR(0) =  ( 0
                    | FB_CSMR_BAM(FB_BAM)       // BAM = 0x0800 ,���ص�ַ���� Ϊ 0x,800 FFFF ,�� Ƭѡ��Ч�ĵ�ַΪ ����ַ ~ ������ַ + ���ص�ַ���룩 ,0x0800 0000 ��Ӧ�� FB_AD27
                    | FB_CSMR_V_MASK            // ʹ��Ƭѡ�ź� FB_CS0
                  );
    FB_CSCR(0) =    FB_CSCR_BLS_MASK            //�Ҷ���
                    | FB_CSCR_PS(2)             //16Byte����
                    | FB_CSCR_AA_MASK           // �Զ�Ӧ��
                    ;

}