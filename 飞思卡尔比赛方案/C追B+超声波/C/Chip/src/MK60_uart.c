/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_uart.c
 * @brief      uart���ں���
 * @author     ɽ��Ƽ�
 * @version    v5.2
 * @date       2014-10-09
 */


#include "common.h"
#include "MK60_uart.h"


uint8 ScopeSendBuff[ChanelNum*2]={0};

UART_MemMapPtr UARTN[UART_MAX] = {UART0_BASE_PTR, UART1_BASE_PTR, UART2_BASE_PTR, UART3_BASE_PTR, UART4_BASE_PTR, UART5_BASE_PTR}; //�������ָ�����鱣�� UARTN �ĵ�ַ


/*!
 *  @brief      ��ʼ�����ڣ����ò�����
 *  @param      UARTn_e       ģ��ţ�UART0~UART5��
 *  @param      baud        �����ʣ���9600��19200��56000��115200��
 *  @since      v5.0
 *  @note       UART���õĹܽ��� App\Inc\PORT_cfg.h ��������ã�
                printf���õĹܽźͲ������� App\Inc\MK60_conf.h ���������
 *  Sample usage:       uart_init (UART3, 9600);        //��ʼ������3��������Ϊ9600
 */
void uart_init (UARTn_e uratn, uint32 baud)
{
    register uint16 sbr, brfa;
    uint8 temp;
    uint32 sysclk;     //ʱ��

    /* ���� UART���ܵ� ���ùܽ� */
    switch(uratn)
    {
    case UART0:
        SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;      //ʹ�� UART0 ʱ��

        if(UART0_RX_PIN == PTA1)
        {
            port_init( UART0_RX_PIN, ALT2);
        }
        else if((UART0_RX_PIN == PTA15) || (UART0_RX_PIN == PTB16) || (UART0_RX_PIN == PTD6)  )
        {
            port_init( UART0_RX_PIN, ALT3);
        }
        else
        {
            ASSERT(0);                           //���������������㣬ֱ�Ӷ���ʧ���ˣ����ùܽ�����
        }

        if(UART0_TX_PIN == PTA2)
        {
            port_init( UART0_TX_PIN, ALT2);
        }
        else if((UART0_TX_PIN == PTA14) || (UART0_TX_PIN == PTB17) || (UART0_TX_PIN == PTD7) )
        {
            port_init( UART0_TX_PIN, ALT3);
        }
        else
        {
            ASSERT(0);                           //���������������㣬ֱ�Ӷ���ʧ���ˣ����ùܽ�����
        }

        break;

    case UART1:
        SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;

        if((UART1_RX_PIN == PTC3) || (UART1_RX_PIN == PTE1))
        {
            port_init( UART1_RX_PIN, ALT3);
        }
        else
        {
            ASSERT(0);                           //���������������㣬ֱ�Ӷ���ʧ���ˣ����ùܽ�����
        }

        if((UART1_TX_PIN == PTC4) || (UART1_TX_PIN == PTE0))
        {
            port_init( UART1_TX_PIN, ALT3);
        }
        else
        {
            ASSERT(0);                           //���������������㣬ֱ�Ӷ���ʧ���ˣ����ùܽ�����
        }

        break;

    case UART2:
        SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;
        if(UART2_TX_PIN == PTD3)
        {
            port_init( UART2_TX_PIN, ALT3);
        }
        else
        {
            ASSERT(0);                           //���������������㣬ֱ�Ӷ���ʧ���ˣ����ùܽ�����
        }


        if(UART2_RX_PIN == PTD2)
        {
            port_init( UART2_RX_PIN, ALT3);
        }
        else
        {
            ASSERT(0);                           //���������������㣬ֱ�Ӷ���ʧ���ˣ����ùܽ�����
        }

        break;

    case UART3:
        SIM_SCGC4 |= SIM_SCGC4_UART3_MASK;

        if((UART3_RX_PIN == PTB10)|| (UART3_RX_PIN == PTC16) || (UART3_RX_PIN == PTE5) )
        {
            port_init( UART3_RX_PIN, ALT3);
        }
        else
        {
            ASSERT(0);                           //���������������㣬ֱ�Ӷ���ʧ���ˣ����ùܽ�����
        }

        if((UART3_TX_PIN == PTB11) || (UART3_TX_PIN == PTC17) || (UART3_TX_PIN == PTE4) )
        {
            port_init( UART3_TX_PIN, ALT3);             //��PTB11��ʹ��UART3_RXD
        }
        else
        {
            ASSERT(0);                           //���������������㣬ֱ�Ӷ���ʧ���ˣ����ùܽ�����
        }
        break;

    case UART4:
        SIM_SCGC1 |= SIM_SCGC1_UART4_MASK;

        if((UART4_RX_PIN == PTC14) || (UART4_RX_PIN == PTE25)  )
        {
            port_init( UART4_RX_PIN, ALT3);            //��PTC14��ʹ��UART4_RXD
        }
        else
        {
            ASSERT(0);                           //���������������㣬ֱ�Ӷ���ʧ���ˣ����ùܽ�����
        }

        if((UART4_TX_PIN == PTC15) || (UART4_TX_PIN == PTE24)  )
        {
            port_init( UART4_TX_PIN, ALT3);
        }
        else
        {
            ASSERT(0);                           //���������������㣬ֱ�Ӷ���ʧ���ˣ����ùܽ�����
        }
        break;

    case UART5:
        SIM_SCGC1 |= SIM_SCGC1_UART5_MASK;

        if((UART5_RX_PIN == PTD8) || (UART5_RX_PIN == PTE9))
        {
            port_init( UART5_RX_PIN, ALT3);
        }
        else
        {
            ASSERT(0);                           //���������������㣬ֱ�Ӷ���ʧ���ˣ����ùܽ�����
        }

        if((UART5_TX_PIN == PTD9) ||(UART5_TX_PIN == PTE8))
        {
            port_init( UART5_TX_PIN, ALT3);             //��PTD9��ʹ��UART5_RXD
        }
        else
        {
            ASSERT(0);                           //���������������㣬ֱ�Ӷ���ʧ���ˣ����ùܽ�����
        }
        break;
    default:
        break;
    }

    //���õ�ʱ��Ӧ�ý�ֹ���ͽ���
    UART_C2_REG(UARTN[uratn]) &= ~(0
                                   | UART_C2_TE_MASK
                                   | UART_C2_RE_MASK
                                  );


    //���ó�8λ��У��ģʽ
    //���� UART ���ݸ�ʽ��У�鷽ʽ��ֹͣλλ����ͨ������ UART ģ����ƼĴ��� C1 ʵ�֣�
    UART_C1_REG(UARTN[uratn]) |= (0
                                  //| UART_C2_M_MASK                    //9 λ�� 8 λģʽѡ�� : 0 Ϊ 8λ ��1 Ϊ 9λ��ע���˱�ʾ0����8λ�� �������9λ��λ8��UARTx_C3�
                                  //| UART_C2_PE_MASK                   //��żУ��ʹ�ܣ�ע���˱�ʾ���ã�
                                  //| UART_C2_PT_MASK                   //У��λ���� : 0 Ϊ żУ�� ��1 Ϊ ��У��
                                 );

    //���㲨���ʣ�����0��1ʹ���ں�ʱ�ӣ���������ʹ��busʱ��
    if ((uratn == UART0) || (uratn == UART1))
    {
        sysclk = core_clk_khz * 1000;                                   //�ں�ʱ��
    }
    else
    {
        sysclk = bus_clk_khz * 1000;                                    //busʱ��
    }

    //UART ������ = UART ģ��ʱ�� / (16 �� (SBR[12:0] + BRFA))
    //������ BRFA ������£� SBR = UART ģ��ʱ�� / (16 * UART ������)
    sbr = (uint16)(sysclk / (baud * 16));
    if(sbr > 0x1FFF)sbr = 0x1FFF;                                       //SBR �� 13bit�����Ϊ 0x1FFF

    //��֪ SBR ���� BRFA =  = UART ģ��ʱ�� / UART ������ - 16 ��SBR[12:0]
    brfa = (sysclk / baud)  - (sbr * 16);
    ASSERT( brfa <= 0x1F);                  //���ԣ������ֵ�����������������õ�����������Ĵ���������
                                            //����ͨ����������������������

    //д SBR
    temp = UART_BDH_REG(UARTN[uratn]) & (~UART_BDH_SBR_MASK);           //���� ��� SBR �� UARTx_BDH��ֵ
    UART_BDH_REG(UARTN[uratn]) = temp |  UART_BDH_SBR(sbr >> 8);        //��д��SBR��λ
    UART_BDL_REG(UARTN[uratn]) = UART_BDL_SBR(sbr);                     //��д��SBR��λ

    //д BRFD
    temp = UART_C4_REG(UARTN[uratn]) & (~UART_C4_BRFA_MASK) ;           //���� ��� BRFA �� UARTx_C4 ��ֵ
    UART_C4_REG(UARTN[uratn]) = temp |  UART_C4_BRFA(brfa);             //д��BRFA



    //����FIFO(FIFO���������Ӳ�������ģ������������)
    UART_PFIFO_REG(UARTN[uratn]) |= (0
                                     | UART_PFIFO_TXFE_MASK               //ʹ��TX FIFO(ע�ͱ�ʾ��ֹ)
                                     //| UART_PFIFO_TXFIFOSIZE(0)         //��ֻ����TX FIFO ��С��0Ϊ1�ֽڣ�1~6Ϊ 2^(n+1)�ֽ�
                                     | UART_PFIFO_RXFE_MASK               //ʹ��RX FIFO(ע�ͱ�ʾ��ֹ)
                                     //| UART_PFIFO_RXFIFOSIZE(0)         //��ֻ����RX FIFO ��С��0Ϊ1�ֽڣ�1~6Ϊ 2^(n+1)�ֽ�
                                    );

    /* �����ͺͽ��� */
    UART_C2_REG(UARTN[uratn]) |= (0
                                  | UART_C2_TE_MASK                     //����ʹ��
                                  | UART_C2_RE_MASK                     //����ʹ��
                                  //| UART_C2_TIE_MASK                  //�����жϻ�DMA��������ʹ�ܣ�ע���˱�ʾ���ã�
                                  //| UART_C2_TCIE_MASK                 //��������ж�ʹ�ܣ�ע���˱�ʾ���ã�
                                  //| UART_C2_RIE_MASK                  //�������жϻ�DMA��������ʹ�ܣ�ע���˱�ʾ���ã�
                                 );

    //�����Ƿ�������պͷ����жϡ�ͨ������ UART ģ��� C2 �Ĵ�����
    //RIE �� TIE λʵ�֡����ʹ���жϣ���������ʵ���жϷ������
}

/*!
 *  @brief      �ȴ�����1���ֽ�
 *  @param      UARTn_e       ģ��ţ�UART0~UART5��
 *  @param      ch          ���յ�ַ
 *  @since      v5.0
 *  @note       �����Ҫ��ѯ����״̬������ uart_query ��
                �����Ҫ��ѯ�������ݣ����� uart_querychar
 *  Sample usage:
                        char ch;
                        uart_getchar (UART3,&ch);   //�ȴ�����1���ֽڣ����浽 ch��
 */
void uart_getchar (UARTn_e uratn, char *ch)
{
    //�ȴ����յ����ݣ�ʹ����FIFO���������ַ��������У�����һ������˼��
    //while( !UART_RCFIFO_REG(UARTN[uratn]) );                                //�ȴ����յ�����������0
    //while( (UART_SFIFO_REG(UARTN[uratn]) & UART_SFIFO_RXEMPT_MASK)) ;     //�ȴ����ջ�����/FIFO �ǿյ�

    //���·����Ƿ�˼�����ٷ������ṩ�ķ���
    while (!(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK));       //�ȴ���������

    // ��ȡ���յ���8λ����
    *ch =  UART_D_REG(UARTN[uratn]);

    // ��ȡ 9λ���ݣ�Ӧ���ǣ���Ҫ�޸ĺ����ķ������ͣ���
    // *ch =   ((( UARTx_C3_REG(UARTN[uratn]) & UART_C3_R8_MASK ) >> UART_C3_R8_SHIFT ) << 8)   |   UART_D_REG(UARTN[uratn]);  //����9bit

}


/*!
 *  @brief      ��ѯ����1���ַ�
 *  @param      UARTn_e       ģ��ţ�UART0~UART5��
 *  @param      ch          ���յ�ַ
 *  @return     1Ϊ���ճɹ���0Ϊ����ʧ��
 *  @since      v5.0
 *  @note       �����Ҫ�ȴ����գ����� uart_getchar
 *  Sample usage:       char ch ;
                        if( uart_querychar (UART3,&ch) == 1)     //��ѯ����1���ַ������浽 ch��
                        {
                            printf("�ɹ����յ�һ���ֽ�");
                        }
 */
char uart_querychar (UARTn_e uratn, char *ch)
{
    if( UART_RCFIFO_REG(UARTN[uratn]) )         //��ѯ�Ƿ���ܵ�����
    {
        *ch  =   UART_D_REG(UARTN[uratn]);      //���ܵ�8λ������
        return  1;                              //���� 1 ��ʾ���ճɹ�
    }
    *ch = 0;                                    //���ղ�����Ӧ������˽�����
    return 0;                                   //����0��ʾ����ʧ��
}

/*!
 *  @brief      ��ѯ�����ַ���
 *  @param      UARTn_e       ģ��ţ�UART0~UART5��
 *  @param      str         ���յ�ַ
 *  @param      max_len     �����ճ���
 *  @return     ���յ����ֽ���Ŀ
 *  @since      v5.0
 *  Sample usage:       char str[100];
                        uint32 num;
                        num = uart_querystr (UART3,&str,100);
                        if( num != 0 )
                        {
                            printf("�ɹ����յ�%d���ֽ�:%s",num,str);
                        }
 */
uint32 uart_querystr (UARTn_e uratn, char *str, uint32 max_len)
{
    uint32 i = 0,j;

    for(j=0;j<10000;j++)                 // 10000 ����������ʱ�������и�������޸�
    {
        while(uart_querychar(uratn, str + i)  )
        {
            j=0;
            if( *(str + i) == NULL )    //���յ��ַ���������
            {
                return i;
            }

            i++;
            if(i >= max_len)            //�����趨�����ֵ���˳�
            {
                *(str + i) = 0;     //ȷ���ַ�����β��0x00
                return i;
            }
        }
    }

    *(str + i) = 0;                     //ȷ���ַ�����β��0x00
    return i;
}


/*!
 *  @brief      ��ѯ����buff
 *  @param      UARTn_e       ģ��ţ�UART0~UART5��
 *  @param      str         ���յ�ַ
 *  @param      max_len     �����ճ���
 *  @return     ���յ����ֽ���Ŀ
 *  @since      v5.0
 *  Sample usage:       char buff[100];
                        uint32 num;
                        num = uart_querybuff (UART3,&buff,100);
                        if( num != 0 )
                        {
                            printf("�ɹ����յ�%d���ֽ�:%s",num,buff);
                        }
 */
uint32 uart_querybuff (UARTn_e uratn, char *buff, uint32 max_len)
{
    uint32 i = 0,j;
    for(j=0;j<10000;j++)                 // 10000 ����������ʱ�������и�������޸�
    {
        while(uart_querychar(uratn, buff + i)  )
        {
            i++;
            if(i >= max_len)            //�����趨�����ֵ���˳�
            {
                return i;
            }
        }
    }

    return i;
}


/*!
 *  @brief      ���ڷ���һ���ֽ�
 *  @param      UARTn_e       ģ��ţ�UART0~UART5��
 *  @param      ch          ��Ҫ���͵��ֽ�
 *  @since      v5.0
 *  @note       printf��Ҫ�õ��˺���
 *  @see        fputc
 *  Sample usage:       uart_putchar (UART3, 'A');  //�����ֽ�'A'
 */
void uart_putchar (UARTn_e uratn, char ch)
{
    //�ȴ����ͻ�������
    while(!(UART_S1_REG(UARTN[uratn]) & UART_S1_TDRE_MASK));

    //��������
    UART_D_REG(UARTN[uratn]) = (uint8)ch;
}

/*!
 *  @brief      ��ѯ�Ƿ���ܵ�һ���ֽ�
 *  @param      UARTn_e       ģ��ţ�UART0~UART5��
 *  @return     ���յ����ݷ���1��û���յ����ݷ���0
 *  @since      v5.0
 *  Sample usage:       char ch;
                        if(uart_query (UART3) == 1)     //��ѯ�Ƿ���յ�����
                        {
                            ch = uart_getchar (UART3);  //�ȴ�����һ�����ݣ����浽 ch��
                        }
 */
char uart_query (UARTn_e uratn)
{
    if(UART_RCFIFO_REG(UARTN[uratn]))                 //���յ�����������0
        //if(!(UART_SFIFO_REG(UARTN[uratn]) & UART_SFIFO_RXEMPT_MASK))
        //if(UART_S1_REG(UARTN[uratn]) & UART_S1_TDRE_MASK)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    //return UART_RCFIFO_REG(UARTN[uratn]);
}

/*!
 *  @brief      ����ָ��len���ֽڳ������� ������ NULL Ҳ�ᷢ�ͣ�
 *  @param      UARTn_e       ģ��ţ�UART0~UART5��
 *  @param      buff        �����ַ
 *  @param      len         ��������ĳ���
 *  @since      v5.0
 *  Sample usage:       uart_putbuff (UART3,"1234567", 3); //ʵ�ʷ�����3���ֽ�'1','2','3'
 */
void uart_putbuff (UARTn_e uratn, uint8 *buff, uint32 len)
{
    while(len--)
    {
        uart_putchar(uratn, *buff);
        buff++;
    }
}


/*!
 *  @brief      �����ַ���(�� NULL ֹͣ����)
 *  @param      UARTn_e       ģ��ţ�UART0~UART5��
 *  @param      str         �ַ�����ַ
 *  @since      v5.0
 *  Sample usage:       uart_putstr (UART3,"1234567"); //ʵ�ʷ�����7���ֽ�
 */
void uart_putstr (UARTn_e uratn, const uint8 *str)
{
    while(*str)
    {
        uart_putchar(uratn, *str++);
    }
}

/*!
 *  @brief      �����ڽ����ж�
 *  @param      UARTn_e       ģ��ţ�UART0~UART5��
 *  @since      v5.0
 *  Sample usage:       uart_rx_irq_en(UART3);         //������3�����ж�
 */
void uart_rx_irq_en(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) |= UART_C2_RIE_MASK;                          //ʹ��UART�����ж�
    enable_irq((IRQn_t)((uratn << 1) + UART0_RX_TX_IRQn));                  //ʹ��IRQ�ж�
}

/*!
 *  @brief      �����ڷ����ж�
 *  @param      UARTn_e       ģ��ţ�UART0~UART5��
 *  @since      v5.0
 *  Sample usage:       uart_tx_irq_en(UART3);         //������3�����ж�
 */
void uart_tx_irq_en(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) |= UART_C2_TIE_MASK;                          //ʹ��UART�����ж�
    enable_irq((IRQn_t)((uratn << 1) + UART0_RX_TX_IRQn));                  //ʹ��IRQ�ж�
}

/*!
 *  @brief      �����ڷ�������ж�
 *  @param      UARTn_e       ģ��ţ�UART0~UART5��
 *  @since      v5.0
 *  Sample usage:       uart_tx_irq_en(UART3);         //������3�����ж�
 */
void uart_txc_irq_en(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) |= UART_C2_TCIE_MASK;                         //ʹ��UART�����ж�
    enable_irq((IRQn_t)((uratn << 1) + UART0_RX_TX_IRQn));                  //ʹ��IRQ�ж�
}

/*!
 *  @brief      �ش��ڽ����ж�
 *  @param      UARTn_e       ģ��ţ�UART0~UART5��
 *  @since      v5.0
 *  Sample usage:       uart_rx_irq_dis(UART3);         //�ش���3�����ж�
 */
void uart_rx_irq_dis(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) &= ~UART_C2_RIE_MASK;                         //��ֹUART�����ж�

    //��������жϻ�û�йأ��򲻹ر�IRQ
    if(!(UART_C2_REG(UARTN[uratn]) & (UART_C2_TIE_MASK | UART_C2_TCIE_MASK)) )
    {
        disable_irq((IRQn_t)((uratn << 1) + UART0_RX_TX_IRQn));             //��IRQ�ж�
    }
}

/*!
 *  @brief      �ش��ڷ����ж�
 *  @param      UARTn_e       ģ��ţ�UART0~UART5��
 *  @since      v5.0
 *  Sample usage:       uart_tx_irq_dis(UART3);         //�ش���3�����ж�
 */
void uart_txc_irq_dis(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) &= ~UART_C2_TCIE_MASK;                        //��ֹUART��������ж�

    //��������жϻ�û�йأ��򲻹ر�IRQ
    if(!(UART_C2_REG(UARTN[uratn]) & UART_C2_RIE_MASK) )
    {
        disable_irq((IRQn_t)((uratn << 1) + UART0_RX_TX_IRQn));             //��IRQ�ж�
    }
}

/*!
 *  @brief      �ش��ڷ����ж�
 *  @param      UARTn_e       ģ��ţ�UART0~UART5��
 *  @since      v5.0
 *  Sample usage:       uart_tx_irq_dis(UART3);         //�ش���3�����ж�
 */
void uart_tx_irq_dis(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) &= ~UART_C2_TIE_MASK;                         //��ֹUART�����ж�

    //��������жϻ�û�йأ��򲻹ر�IRQ
    if(!(UART_C2_REG(UARTN[uratn]) & UART_C2_RIE_MASK) )
    {
        disable_irq((IRQn_t)((uratn << 1) + UART0_RX_TX_IRQn));             //��IRQ�ж�
    }
}

/*!
 *  @brief      UART3�����жϷ�����
 *  @since      v5.0
 *  @warning    �˺�����Ҫ�û������Լ�������ɣ�����������ṩһ��ģ��
 *  Sample usage:       set_vector_handler(UART3_RX_TX_VECTORn , uart3_test_handler);    //�� uart3_handler ������ӵ��ж�����������Ҫ�����ֶ�����
 */
void uart3_test_handler(void)
{
    UARTn_e uratn = UART3;

    if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK)   //�������ݼĴ�����
    {
        //�û���Ҫ�����������

    }

    if(UART_S1_REG(UARTN[uratn]) & UART_S1_TDRE_MASK )  //�������ݼĴ�����
    {
        //�û���Ҫ����������

    }
}

/*!
    ��������:��λ��ʾ�������ݴ��ͺ���
    ����ˣ��⺣��
    ����޸�ʱ�䣺2015/1/27
 */

void SendToScope(uint8 ch,uint16 data)
{
   uint8 sum,i;
   
   // �ߵ�8λ����
   ScopeSendBuff[ch*2]=data/256;   
   ScopeSendBuff[ch*2+1]=data%256;
   
   // ����֡ͷ
   uart_putchar(UART5,251);
   uart_putchar(UART5,109);
   uart_putchar(UART5,37);
   
   // ֡ͷ�ʹ洢
   sum=397;
   for(i=0;i<DataNum;i++)
   {
        uart_putchar(UART5,ScopeSendBuff[i]);
        
        //�����ۼ�
        sum+=ScopeSendBuff[i];
    }
   
   //֡ͷ�������ܺͶ�256��������Ϊβ��У����
   
    uart_putchar(UART5,sum);
}