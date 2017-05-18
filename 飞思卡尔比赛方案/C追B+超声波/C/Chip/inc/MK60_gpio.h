/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_gpio.h
 * @brief      gpio����ͷ�ļ�
 * @author     ɽ��Ƽ�
 * @version    v5.1
 * @date       2014-04-25
 */
#ifndef __MK60_GPIO_H__
#define __MK60_GPIO_H__

#include "MK60_gpio_cfg.h"

/*
 * ����ܽŷ���
 */
typedef enum GPIO_CFG
{
    //�����ֵ���ܸģ�����
    GPI         = 0,                                //����ܽ����뷽��      GPIOx_PDDRn�0��ʾ���룬1��ʾ���
    GPO         = 1,                                //����ܽ��������
} GPIO_CFG;

#define HIGH  1u
#define LOW   0u

extern  GPIO_MemMapPtr      GPIOX[PTX_MAX];
#define GPIOX_BASE(PTxn)    GPIOX[PTX(PTxn)]       //GPIOģ��ĵ�ַ


/****************************�ⲿʹ��****************************/

extern void    gpio_init  (PTXn_e, GPIO_CFG, uint8 data);    //��ʼ��gpio
extern void    gpio_ddr   (PTXn_e, GPIO_CFG);                //�����������ݷ���
extern void    gpio_set   (PTXn_e,           uint8 data);    //��������״̬
extern void    gpio_turn  (PTXn_e);                          //��ת����״̬
extern uint8   gpio_get   (PTXn_e);                          //��ȡ����״̬

//���� 4�� ���� �� PTxn ֻ���� �궨�壬������ ����
#define GPIO_SET(PTxn,data)       (PTXn_T(PTxn,OUT)= (data))    //���������ƽ
#define GPIO_TURN(PTxn)           (PTXn_T(PTxn,T)= 1)           //��ת�����ƽ
#define GPIO_GET(PTxn)            (PTXn_T(PTxn,IN))             //��ȡ��������״̬
#define GPIO_DDR(PTxn,ddr)        (PTXn_T(PTxn,DDR) = ddr)      //�������״̬


//����  ���� �� PTxn ������  �궨�壬Ҳ������ ����


//nλ����
#define GPIO_SET_NBIT(NBIT,PTxn,data)   GPIO_PDOR_REG(GPIOX_BASE(PTxn)) =   (                                                   \
                                                                                (                                               \
                                                                                    GPIO_PDOR_REG(GPIOX_BASE(PTxn))             \
                                                                                    &                                           \
                                                                                    ((uint32)( ~(((1<<NBIT)-1)<<PTn(PTxn))))    \
                                                                                )                                               \
                                                                                |   ( ((data)&( (1<<(NBIT))-1))<<PTn(PTxn) )    \
                                                                            )


#define GPIO_DDR_NBIT(NBIT,PTxn,ddr)   GPIO_PDDR_REG(GPIOX_BASE(PTxn))  =   (                                                   \
                                                                                (                                               \
                                                                                    GPIO_PDDR_REG(GPIOX_BASE(PTxn))             \
                                                                                    &                                           \
                                                                                    ((uint32)( ~(((1<<(NBIT))-1)<<PTn(PTxn))))  \
                                                                                )                                               \
                                                                                |   ( ( (ddr) &  ( (1<<(NBIT))-1))<<PTn(PTxn) ) \
                                                                            )

#define GPIO_T_NBIT(NBIT,PTxn,data)   GPIO_PTOR_REG(GPIOX_BASE(PTxn))  =   (                                                    \
                                                                                (                                               \
                                                                                    GPIO_PTOR_REG(GPIOX_BASE(PTxn))             \
                                                                                    &                                           \
                                                                                    ((uint32)( ~(((1<<NBIT)-1)<<PTn(PTxn))))    \
                                                                                )                                               \
                                                                                |   ( ((data)&( (1<<(NBIT))-1))<<PTn(PTxn) )    \
                                                                            )


#define GPIO_GET_NBIT(NBIT,PTxn)    (( GPIO_PDIR_REG(GPIOX_BASE(PTxn))>>PTn(PTxn) ) & ((1<<NBIT)-1))


#endif      //__MK60_GPIO_H__
