/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_port.c
 * @brief      port�������ú͸��ֹ�������
 * @author     ɽ��Ƽ�
 * @version    v5.1
 * @date       2014-04-25
 */

#include "common.h"
#include "MK60_port.h"

PORT_MemMapPtr PORTX[PTX_MAX] = {PORTA_BASE_PTR, PORTB_BASE_PTR, PORTC_BASE_PTR, PORTD_BASE_PTR, PORTE_BASE_PTR};

/*!
 *  @brief      PORT��ʼ��
 *  @param      PTxn    �˿�
 *  @param      cfg     �˿��������ã��紥��ѡ�����������ѡ��
 *  @since      v5.0
 *  @note       ��port_init_NoALT��ͬ���ǣ��˺�����Ҫ���� MUX ���ù��ܣ�
                ���� MUX = ALT0
 *  Sample usage:       port_init (PTA8, IRQ_RISING | PF | ALT1 | PULLUP );    //��ʼ�� PTA8 �ܽţ������ش����жϣ�����Դ�˲��������ù���ΪGPIO ����������
 */
void  port_init(PTXn_e ptxn, uint32 cfg )
{
    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK << PTX(ptxn));                           //����PORTx�˿�

    PORT_ISFR_REG(PORTX_BASE(ptxn)) = (1<<PTn(ptxn));                           // ��ձ�־λ

    PORT_PCR_REG(PORTX_BASE(ptxn), PTn(ptxn)) = cfg;                            // ���ù��� , ȷ������ģʽ ,������������������
}

/*!
 *  @brief      PORT��ʼ��
 *  @param      PTxn    �˿�
 *  @param      cfg     �˿��������ã��紥��ѡ�����������ѡ��
 *  @since      v5.0
 *  @note       ��port_init��ͬ���ǣ��˺�������Ҫ���� MUX ���ù��ܣ���ʹ������Ҳ����Ч����
                MUX ���� Ϊԭ�ȼĴ������õ�ֵ
 *  Sample usage:       port_init_NoALT (PTA8, IRQ_RISING | PF | PULLUP );    //��ʼ�� PTA8 �ܽţ������ش����жϣ�����Դ�˲���������ԭ�ȸ��ù��ܣ���������
 */
void  port_init_NoALT(PTXn_e ptxn, uint32 cfg)
{
    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK << PTX(ptxn));                           //����PORTx�˿�

    PORT_ISFR_REG(PORTX_BASE(ptxn)) = (1<<PTn(ptxn));                           // ��ձ�־λ

    //���cfg���MUX�����ؼĴ������MUX
    cfg &= ~PORT_PCR_MUX_MASK;                      //����MUX �ֶΣ�������Ҫ����ALT������ԭ����ALT��
    cfg |=  (PORT_PCR_REG(PORTX_BASE(ptxn), PTn(ptxn)) & PORT_PCR_MUX_MASK);    //��ȡ�Ĵ��������õ� MUX

    PORT_PCR_REG(PORTX_BASE(ptxn), PTn(ptxn)) = cfg;            // ���ù��� , ȷ������ģʽ ,������������������
}

/*!
 *  @brief      PORTA�Ĳο��жϷ�����
 *  @since      v5.0
 *  @warning    �˺�����Ҫ�û������Լ�������ɣ�����������ṩһ��ģ��
 *  Sample usage:       set_vector_handler(PORTA_VECTORn , porta_handler);    //�� porta_handler ������ӵ��ж�����������Ҫ�����ֶ�����
 */
void porta_handler(void)
{
    uint8  n = 0;    //���ź�

    //PTA6
    n = 6;
    if(PORTA_ISFR & (1 << n))           //PTA6�����ж�
    {
        PORTA_ISFR  = (1 << n);        //д1���жϱ�־λ

        /*  ����Ϊ�û�����  */


        /*  ����Ϊ�û�����  */
    }

    //���� PTA7 ��Ҫִ�е��û�������Ϊ func() �������ֱ�ӵ������º궨�壺
    //PORT_FUNC(A,7,func);
    //���������������� PTA6 ��Ч����һ���ģ�ֻ�����Ǽ����û�������
}