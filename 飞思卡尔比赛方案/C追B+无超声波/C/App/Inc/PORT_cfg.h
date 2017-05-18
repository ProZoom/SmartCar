/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       port_cfg.h
 * @brief      ɽ��K60 ���ùܽ�����
 * @author     ɽ��Ƽ�
 * @version    v5.1
 * @date       2013-06-26
 */
#ifndef _PORT_CFG_H_
#define _PORT_CFG_H_

#include "MK60_port.h"

/**********************************  UART   ***************************************/

//      ģ��ͨ��    �˿�          ��ѡ��Χ                          ����
#define UART0_RX_PIN    PTD6        //PTA1��PTA15��PTB16��PTD6          PTA1��Ҫ�ã���Jtag��ͻ��
#define UART0_TX_PIN    PTD7        //PTA2��PTA14��PTB17��PTD7          PTA2��Ҫ�ã���Jtag��ͻ��

#define UART1_RX_PIN    PTC3        //PTC3��PTE1
#define UART1_TX_PIN    PTC4        //PTC4��PTE0

#define UART2_RX_PIN    PTD2        //PTD2
#define UART2_TX_PIN    PTD3        //PTD3

#define UART3_RX_PIN    PTC16       //PTB10��PTC16��PTE5
#define UART3_TX_PIN    PTC17       //PTB11��PTC17��PTE4

#define UART4_RX_PIN    PTC14       //PTC14��PTE25
#define UART4_TX_PIN    PTC15       //PTC15��PTE24

#define UART5_RX_PIN    PTE9        //PTD8��PTE9
#define UART5_TX_PIN    PTE8        //PTD9��PTE8

/**********************************  FTM    ***************************************/

//      ģ��ͨ��    �˿�          ��ѡ��Χ              ����
#define FTM0_CH0_PIN    PTC1        //PTC1��PTA3            PTA3��Ҫ�ã���Jtag��SWD��ͻ��
#define FTM0_CH1_PIN    PTC2        //PTC2��PTA4
#define FTM0_CH2_PIN    PTC3        //PTC3��PTA5
#define FTM0_CH3_PIN    PTC4        //PTC4��PTA6
#define FTM0_CH4_PIN    PTA7        //PTD4��PTA7
#define FTM0_CH5_PIN    PTD5        //PTD5��PTA0            PTA0��Ҫ�ã���Jtag��SWD��ͻ��
#define FTM0_CH6_PIN    PTD6        //PTD6��PTA1            PTA1��Ҫ�ã���Jtag��ͻ��
#define FTM0_CH7_PIN    PTD7        //PTD7��PTA2            PTA2��Ҫ�ã���Jtag��ͻ��


//      ģ��ͨ��    �˿�          ��ѡ��Χ              ����
#define FTM1_CH0_PIN    PTA12       //PTA8��PTA12��PTB0
#define FTM1_CH1_PIN    PTA9       //PTA9��PTA13��PTB1

//      ģ��ͨ��    �˿�          ��ѡ��Χ              ����
#define FTM2_CH0_PIN    PTA10       //PTA10��PTB18
#define FTM2_CH1_PIN    PTA11       //PTA11��PTB19


#ifdef  MK60F15                 //ALT6       ALT3        ALT4        FX ����FTM3ģ��

#define FTM3_CH0_PIN    PTE5        //PTE5                   PTD0
#define FTM3_CH1_PIN    PTE6        //PTE6                   PTD1
#define FTM3_CH2_PIN    PTE7        //PTE7                   PTD2
#define FTM3_CH3_PIN    PTE8        //PTE8                   PTD3
#define FTM3_CH4_PIN    PTE9        //PTE9       PTC8
#define FTM3_CH5_PIN    PTE10       //PTE10      PTC9
#define FTM3_CH6_PIN    PTE11       //PTE11      PTC10
#define FTM3_CH7_PIN    PTE12       //PTE12      PTC11

#endif

//��������ģ��ͨ��  �˿�          ��ѡ��Χ              ����
#define FTM1_QDPHA_PIN  PTA12       //PTA8��PTA12��PTB0
#define FTM1_QDPHB_PIN  PTA13       //PTA9��PTA13��PTB1

#define FTM2_QDPHA_PIN  PTA10       //PTA10��PTB18
#define FTM2_QDPHB_PIN  PTA11       //PTA11��PTB19


/**********************************  I2C   ***************************************/

//      ģ��ͨ��    �˿�          ��ѡ��Χ              ����
#define I2C0_SCL_PIN    PTD8        // PTB0��PTB2��PTD8
#define I2C0_SDA_PIN    PTD9        // PTB1��PTB3��PTD9

#define I2C1_SCL_PIN    PTC10       // PTE1��PTC10
#define I2C1_SDA_PIN    PTC11       // PTE0��PTC11


/**********************************  SPI   ***************************************/
//PCS�ӿڣ����õ�ʱ����Ҫע�ͣ��Ͳ�����г�ʼ����Ӧ�Ĺܽ�
//      ģ��ͨ��    �˿�          ��ѡ��Χ                  ����

#define SPI0_SCK_PIN    PTA15       // PTA15��PTC5��PTD1        ȫ������ ALT2
#define SPI0_SOUT_PIN   PTA16       // PTA16��PTC6��PTD2        ȫ������ ALT2
#define SPI0_SIN_PIN    PTA17       // PTA17��PTC7��PTD3        ȫ������ ALT2

#define SPI0_PCS0_PIN   PTA14       // PTA14��PTC4��PTD0��      ȫ������ ALT2
#define SPI0_PCS1_PIN   PTC3        // PTC3��PTD4               ȫ������ ALT2
#define SPI0_PCS2_PIN   PTC2        // PTC2��PTD5               ȫ������ ALT2
#define SPI0_PCS3_PIN   PTC1        // PTC1��PTD6               ȫ������ ALT2
#define SPI0_PCS4_PIN   PTC0        // PTC0��                   ȫ������ ALT2
#define SPI0_PCS5_PIN   PTB23       // PTB23                    ALT3


#define SPI1_SCK_PIN    PTB11       // PTE2��PTB11��            ȫ������ ALT2
#define SPI1_SOUT_PIN   PTB16       // PTE1��PTB16��            ȫ������ ALT2
#define SPI1_SIN_PIN    PTB17       // PTE3��PTB17��            ȫ������ ALT2

#define SPI1_PCS0_PIN   PTB10       // PTE4��PTB10��            ȫ������ ALT2
#define SPI1_PCS1_PIN   PTE0        // PTE0��PTB9��             ȫ������ ALT2
#define SPI1_PCS2_PIN   PTE5        // PTE5��                   ȫ������ ALT2
#define SPI1_PCS3_PIN   PTE6        // PTE6��                   ȫ������ ALT2


#define SPI2_SCK_PIN    PTD12       // PTB21��PTD12             ȫ������ ALT2
#define SPI2_SOUT_PIN   PTD13       // PTB22��PTD13             ȫ������ ALT2
#define SPI2_SIN_PIN    PTD14       // PTB23��PTD14             ȫ������ ALT2
#define SPI2_PCS0_PIN   PTD11       // PTB20��PTD11             ȫ������ ALT2
#define SPI2_PCS1_PIN   PTD15       // PTD15                    ȫ������ ALT2


/**********************************  CAN   ***************************************/
#define CAN0_TX_PIN     PTA12       //PTA12��PTB18              ȫ������ ALT2
#define CAN0_RX_PIN     PTA13       //PTA13��PTB19              ȫ������ ALT2

#define CAN1_TX_PIN     PTE24       //PTE24��PTC17              ȫ������ ALT2
#define CAN1_RX_PIN     PTE25       //PTE25��PTC16              ȫ������ ALT2


#endif  //_PORT_CFG_H_


