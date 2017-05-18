/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       VCAN_NRF24L0.c
 * @brief      NRF24L0��������ʵ��
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-07-9
 */
#ifndef _VCAN_NRF24L0_H_
#define _VCAN_NRF24L0_H_     1


//������Ӳ������
#define NRF_SPI         SPI2
#define NRF_CS          SPI_PCS0

#define NRF_CE_PTXn     PTD15
#define NRF_IRQ_PTXn    PTD10

//�������û����õ�ѡ��

#define DATA_PACKET             32      //һ�δ�������֧�ֵ��ֽ�����1~32��
#define RX_FIFO_PACKET_NUM      80      //���� FIFO �� �� ��Ŀ ( �ܿռ� ����Ҫ���� һ��ͼ��Ĵ�С������ û�������� )
#define ADR_WIDTH               5       //�����ַ���ȣ�3~5��
#define IS_CRC16                1       //1��ʾʹ�� CRC16��0��ʾ ʹ��CRC8 (0~1)


//���õ��������


/**
 *  @brief NRFģ�鷢��״̬
 */
typedef enum
{
    NRF_TXING,              //������
    NRF_TX_ERROR,           //���ʹ���
    NRF_TX_OK,              //�������
} nrf_tx_state_e;


//��������
extern  uint8   nrf_init(void);                     //��ʼ��NRF24L01+

extern  uint8   nrf_link_check(void);               //���NRF24L01+�뵥Ƭ���Ƿ�ͨ������

extern  uint32  nrf_rx(uint8 *rxbuf, uint32 len);   //��������
extern  uint8   nrf_tx(uint8 *txbuf, uint32 len);   //��������

extern  nrf_tx_state_e nrf_tx_state ();             //��鷢��״̬(�������ݺ��ѯ�Ƿ��ͳɹ�)


extern  void    nrf_handler(void);                  //NRF24L01+ �� �жϷ�����

//����ĺ��� ���� ������Ϣ������� �ĺ���ʹ�ã�һ���û��ò���
extern  uint8  nrf_rx_fifo_check(uint32 offset,uint16 * val);    //��ȡ ����FIFO ������


#endif      //_VCAN_NRF24L0_H_
