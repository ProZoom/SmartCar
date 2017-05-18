/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_spi.c
 * @brief      SPI��������
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-07-16
 */
#ifndef __MK60_SPI_H__
#define __MK60_SPI_H__

/**
 *  @brief ���ӻ�ģʽ
 */
typedef enum
{
    MASTER,    //����ģʽ
    SLAVE      //�ӻ�ģʽ
} SPI_CFG;

/**
 *  @brief SPIģ���
 */
typedef enum
{
    SPI0,
    SPI1,
    SPI2
} SPIn_e;

/**
 *  @brief SPIģ��Ƭѡ��
 */
typedef enum
{
    SPI_PCS0 = 1 << 0,
    SPI_PCS1 = 1 << 1,
    SPI_PCS2 = 1 << 2,
    SPI_PCS3 = 1 << 3,
    SPI_PCS4 = 1 << 4,
    SPI_PCS5 = 1 << 5,
} SPI_PCSn_e;


extern uint32 spi_init       (SPIn_e, SPI_PCSn_e , SPI_CFG,uint32 baud);                                        //SPI��ʼ����ѡ��Ƭѡ�źţ�����ģʽ��������
uint32 spi_set_baud (SPIn_e,                      uint32 baud);

//�������շ��ͺ���
extern void spi_mosi       (SPIn_e, SPI_PCSn_e pcs,                              uint8 *modata, uint8 *midata,               uint32 len);    //SPI���ͽ��պ���,����databuff���ݣ����ѽ��յ������ݴ����databuff��(ע�⣬�Ḳ��ԭ����databuff)
extern void spi_mosi_cmd   (SPIn_e, SPI_PCSn_e pcs, uint8 *mocmd , uint8 *micmd , uint8 *modata, uint8 *midata, uint32 cmdlen , uint32 len); //SPI���ͽ��պ���,��spi_mosi��ȣ������ȷ���cmd �������Ĳ��裬���ֿ������ַ���



#endif  // __MK60_SPI_H__