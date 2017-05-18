/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_uart.h
 * @brief      uart����ͷ�ļ�
 * @author     ɽ��Ƽ�
 * @version    v5.2
 * @date       2014-10-09
 */

#ifndef __MK60_UART_H__
#define __MK60_UART_H__


#define ChanelNum 17
#define DataNum ChanelNum*2

typedef enum
{
    UART0,
    UART1,
    UART2,
    UART3,
    UART4,
    UART5,

    UART_MAX,
} UARTn_e;

extern UART_MemMapPtr UARTN[UART_MAX];
/********************************************************************/


//��ʼ��
extern void   uart_init      (UARTn_e, uint32 baud);             //��ʼ��uartxģ��

//������ش���
extern void   uart_getchar   (UARTn_e, char *ch);                //�ȴ�����1���ֽ�

extern char   uart_querychar (UARTn_e, char *ch);                //��ѯ����1���ַ�
extern uint32 uart_querystr  (UARTn_e, char *str, uint32 max_len); //��ѯ�����ַ���
extern uint32 uart_querybuff (UARTn_e, char *buff,uint32 max_len); //��ѯ����buff

extern char   uart_query     (UARTn_e);                          //��ѯ�Ƿ���յ�һ���ֽ�


//������ش���
extern void uart_putchar   (UARTn_e, char ch);                 //����1���ֽ�
extern void uart_putbuff   (UARTn_e , uint8 *buff, uint32 len);//����len���ֽ�buff
extern void uart_putstr    (UARTn_e , const uint8 *str);       //�����ַ���

extern void SendToScope(uint8 ch,uint16 data);              //�����



//�ж���ش���
extern void uart_rx_irq_en (UARTn_e);                          //�����ڽ����ж�
extern void uart_tx_irq_en (UARTn_e);                          //�����ڷ����ж�
extern void uart_txc_irq_en (UARTn_e);                         //�����ڷ�������ж�

extern void uart_rx_irq_dis(UARTn_e);                          //�ش��ڽ����ж�
extern void uart_tx_irq_dis(UARTn_e);                          //�ش��ڷ����ж�
extern void uart_txc_irq_dis(UARTn_e);                         //�ش��ڷ�������ж�

extern void uart3_test_handler(void);                        //�жϷ������������ο������û�����ʵ�֣�

/********************************************************************/

#endif /* __MK60_UART_H__ */
