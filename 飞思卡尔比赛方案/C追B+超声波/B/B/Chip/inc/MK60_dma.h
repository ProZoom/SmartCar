/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_dma.h
 * @brief      DMA������
 * @author     ɽ��Ƽ�
 * @version    v5.1
 * @date       2014-04-25
 */


#ifndef _MK60_DMA_H_
#define _MK60_DMA_H_


typedef enum
{
    /*        ����ͨ��            */
    Channel_Disabled        = 0,

    /*        UART            */
    DMA_UART0_Rx            = 2,
    DMA_UART0_Tx            = 3,
    DMA_UART1_Rx            = 4,
    DMA_UART1_Tx            = 5,
    DMA_UART2_Rx            = 6,
    DMA_UART2_Tx            = 7,
    DMA_UART3_Rx            = 8,
    DMA_UART3_Tx            = 9,
    DMA_UART4_Rx            = 10,
    DMA_UART4_Tx            = 11,
    DMA_UART5_Rx            = 12,
    DMA_UART5_Tx            = 13,

    /*        I2S            */
    DMA_I2S0_Rx             = 14,
    DMA_I2S0_Tx             = 15,

    /*        SPI            */
    DMA_SPI0_Rx             = 16,
    DMA_SPI0_Tx             = 17,
    DMA_SPI1_Rx             = 18,
    DMA_SPI1_Tx             = 19,

    /*        I2C            */
    DMA_I2C0                = 22,
    DMA_I2C1                = 23,

    /*        FTM            */
    DMA_FTM0_CH0            = 24,
    DMA_FTM0_CH1            = 25,
    DMA_FTM0_CH2            = 26,
    DMA_FTM0_CH3            = 27,
    DMA_FTM0_CH4            = 28,
    DMA_FTM0_CH5            = 29,
    DMA_FTM0_CH6            = 30,
    DMA_FTM0_CH7            = 31,

    DMA_FTM1_CH0            = 32,
    DMA_FTM1_CH1            = 33,

    DMA_FTM2_CH0            = 34,
    DMA_FTM2_CH1            = 35,

    DMA_FTM3_CH0            = 36,
    DMA_FTM3_CH1            = 37,
    DMA_FTM3_CH2            = 38,

    DMA_FTM1_CH3            = 39,

    /*     ADC/DAC/CMP/CMT    */
    DMA_ADC0                = 40,
    DMA_ADC1                = 41,
    DMA_CMP0                = 42,
    DMA_CMP1                = 43,
    DMA_CMP2                = 44,
    DMA_DAC0                = 45,
    DMA_DAC1                = 46,
    DMA_CMT                 = 47,

    DMA_PDB                 = 48,

    DMA_PORTA              = 49,
    DMA_PORTB              = 50,
    DMA_PORTC              = 51,
    DMA_PORTD              = 52,
    DMA_PORTE              = 53,

    DMA_FTM3_CH4            = 54,
    DMA_FTM3_CH5            = 55,
    DMA_FTM3_CH6            = 56,
    DMA_FTM3_CH7            = 57,

    DMA_Always_EN1          = 58,
    DMA_Always_EN2          = 59,
    DMA_Always_EN3          = 60,
    DMA_Always_EN4          = 61,
    DMA_Always_EN5          = 62,
    DMA_Always_EN6          = 63,
} DMA_sources;



typedef enum
{
    DADDR_RECOVER = 0,        //�ָ�Ŀ�ĵ�ַ
    DADDR_KEEPON  = 1,        //Ŀ�ĵ�ַ���ֲ���
} DMA_cfg;

typedef enum      //DMAÿ�δ����ֽ���
{
    DMA_BYTE1 = 0,
    DMA_BYTE2 = 1,
    DMA_BYTE4 = 2,
    DMA_BYTE16 = 4
} DMA_BYTEn;


typedef enum
{
    DMA_CH0,
    DMA_CH1,
    DMA_CH2,
    DMA_CH3,
    DMA_CH4,
    DMA_CH5,
    DMA_CH6,
    DMA_CH7,
    DMA_CH8,
    DMA_CH9,
    DMA_CH10,
    DMA_CH11,
    DMA_CH12,
    DMA_CH13,
    DMA_CH14,
    DMA_CH15
} DMA_CHn;


#define  DMA_IRQ_EN(DMA_CHn)    enable_irq((IRQn_t)((IRQn_t)DMA_CHn + DMA0_IRQn))               //����DMAͨ����������ж�
#define  DMA_IRQ_DIS(DMA_CHn)   disable_irq((IRQn_t)((IRQn_t)DMA_CHn + DMA0_IRQn))              //��ֹDMAͨ����������ж�

#define  DMA_IRQ_CLEAN(DMA_CHn) DMA_INT|=(DMA_INT_INT0_MASK<<DMA_CHn)           //���ͨ�������жϱ�־λ

#define  DMA_EN(DMA_CHn)        DMA_ERQ |= (DMA_ERQ_ERQ0_MASK<<(DMA_CHn))       //ʹ��ͨ��Ӳ��DMA����
#define  DMA_DIS(DMA_CHn)       DMA_ERQ &=~(DMA_ERQ_ERQ0_MASK<<(DMA_CHn))       //��ֹͨ��Ӳ��DMA����

#if defined(MK60DZ10)
#define  DMA_CHn_DIS(DMA_CHn)   DMAMUX_CHCFG_REG(DMAMUX_BASE_PTR,DMA_CHn) &= ~DMAMUX_CHCFG_SOURCE(Channel_Disabled)  //����ͨ��
#elif defined(MK60F15)
#define  DMA_CHn_DIS(DMA_CHn)   DMAMUX_CHCFG_REG(DMAMUX0_BASE_PTR,DMA_CHn) &= ~DMAMUX_CHCFG_SOURCE(Channel_Disabled)  //����ͨ��
#endif


//��ʼ��DMA��ʹ��PORT�˿�����ͨ��DMA���䵽BUFF������
extern void dma_portx2buff_init(DMA_CHn, void *SADDR, void *DADDR, PTXn_e, DMA_BYTEn, uint32 count, uint32 cfg);

//DMA �������ã�������Ϻ�һЩ������ı䣬��Ҫ���¸�ֵ
static inline void dma_repeat(DMA_CHn CHn,void *SADDR, void *DADDR,uint32 count)
{
    DMA_IRQ_CLEAN(CHn);
    DMA_CITER_ELINKNO(CHn)  = DMA_CITER_ELINKNO_CITER(count);   //��ǰ��ѭ������
    DMA_BITER_ELINKNO(CHn)  = DMA_BITER_ELINKNO_BITER(count);   //��ʼ��ѭ������

    DMA_SADDR(CHn) =    (uint32)SADDR;                      // ����  Դ��ַ
    DMA_DADDR(CHn) =    (uint32)DADDR;                      // ����Ŀ�ĵ�ַ

    DMA_EN(DMA_CH0);                        //ʹ��DMA Ӳ������


}

#endif  //_MK60_DMA_H_