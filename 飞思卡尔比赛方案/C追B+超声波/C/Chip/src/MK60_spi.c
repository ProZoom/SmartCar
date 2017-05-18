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

#include "common.h"
#include "MK60_port.h"
#include "MK60_spi.h"


SPI_MemMapPtr SPIN[3] = {SPI0_BASE_PTR, SPI1_BASE_PTR, SPI2_BASE_PTR}; //��������ָ�����鱣�� SPIx �ĵ�ַ



#define SPI_TX_WAIT(SPIn)     while(  ( SPI_SR_REG(SPIN[SPIn]) & SPI_SR_TXRXS_MASK ) == 1 ) //�ȴ����� ���
#define SPI_RX_WAIT(SPIn)     while(  ( SPI_SR_REG(SPIN[SPIn]) & SPI_SR_RFDF_MASK ) == 0 )  //�ȴ����� FIFOΪ�ǿ�
#define SPI_EOQF_WAIT(SPIn)   while(  (SPI_SR_REG(SPIN[SPIn]) & SPI_SR_EOQF_MASK ) == 0 )   //�ȴ��������


/*!
 *  @brief      SPI��ʼ��������ģʽ
 *  @param      SPIn_e          SPIģ��(SPI0��SPI1��SPI2)
 *  @param      SPIn_PCSn_e     Ƭѡ�ܽű��
 *  @param      SPI_CFG         SPI���ӻ�ģʽѡ��
 *  @since      v5.0
 *  Sample usage:       uint32 baud = spi_init(SPI0,SPIn_PCS0, MASTER,10*1000*1000);              //��ʼ��SPI,ѡ��CS0,����ģʽ, ������Ϊ1M ,������ʵ�����ʵ�baud����
 */
uint32 spi_init(SPIn_e spin, SPI_PCSn_e pcs, SPI_CFG master,uint32 baud)
{
    uint8  br,pbr;
    uint32 clk = bus_clk_khz*1000/baud;
    uint32 Scaler[] = {2,4,6,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768};
    uint8  Prescaler[] = {2,3,5,7};
    uint32 fit_clk,fit_br=0,fit_pbr,min_diff =~0,diff;
    uint32 tmp;

    //����CS����һ��SCK��ʱ��
    uint8 pcssck,cssck,fit_pcssck,fit_cssck;

    //ʹ��SPIģ��ʱ�ӣ�����SPI���Ź���
    if(spin == SPI0)
    {
        SIM_SCGC6 |= SIM_SCGC6_DSPI0_MASK;

        //���йܽŸ���
        port_init(SPI0_SCK_PIN , ALT2  );
        port_init(SPI0_SOUT_PIN, ALT2  );
        port_init(SPI0_SIN_PIN , ALT2  );

        if(pcs & SPI_PCS0)
            port_init(SPI0_PCS0_PIN, ALT2  );

        if(pcs & SPI_PCS1)
            port_init(SPI0_PCS1_PIN, ALT2  );

        if(pcs & SPI_PCS2)
            port_init(SPI0_PCS2_PIN, ALT2  );

        if(pcs & SPI_PCS3)
            port_init(SPI0_PCS3_PIN, ALT2  );

        if(pcs & SPI_PCS4)
            port_init(SPI0_PCS4_PIN, ALT2  );

        if(pcs & SPI_PCS5)
            port_init(SPI0_PCS5_PIN, ALT3  );
    }
    else if(spin == SPI1)
    {
#if defined(MK60DZ10)
        SIM_SCGC6 |= SIM_SCGC6_SPI1_MASK;
#elif defined(MK60F15)
        SIM_SCGC6 |= SIM_SCGC6_DSPI1_MASK;
#endif
        port_init(SPI1_SCK_PIN , ALT2  );
        port_init(SPI1_SOUT_PIN, ALT2  );
        port_init(SPI1_SIN_PIN , ALT2  );

        if(pcs & SPI_PCS0)
            port_init(SPI1_PCS0_PIN, ALT2  );

        if(pcs & SPI_PCS1)
            port_init(SPI1_PCS1_PIN, ALT2  );

        if(pcs & SPI_PCS2)
            port_init(SPI1_PCS2_PIN, ALT2  );

        if(pcs & SPI_PCS3)
            port_init(SPI1_PCS3_PIN, ALT2  );
    }
    else if(spin == SPI2)
    {
#if defined(MK60DZ10)
        SIM_SCGC3 |= SIM_SCGC3_SPI2_MASK;
#elif defined(MK60F15)
        SIM_SCGC3 |= SIM_SCGC3_DSPI2_MASK;
#endif

        port_init(SPI2_SCK_PIN , ALT2  );
        port_init(SPI2_SOUT_PIN, ALT2  );
        port_init(SPI2_SIN_PIN , ALT2  );

        if(pcs & SPI_PCS0)
            port_init(SPI2_PCS0_PIN, ALT2  );

        if(pcs & SPI_PCS1)
            port_init(SPI2_PCS1_PIN, ALT2  );
    }
    else
    {
        //���ݽ����� spi ģ������ֱ���ж϶���ʧ��
        ASSERT(0);
    }

    SPI_MCR_REG(SPIN[spin]) = ( 0
                                | SPI_MCR_CLR_TXF_MASK     //��� Tx FIFO ������
                                | SPI_MCR_CLR_RXF_MASK     //��� Rx FIFO ������
                                | SPI_MCR_HALT_MASK        //ֹͣSPI����
                              );

    /*************  ���־λ  ***************/
    SPI_SR_REG(SPIN[spin]) = (0
                              | SPI_SR_EOQF_MASK    //���Ͷ��п��ˣ��������
                              | SPI_SR_TFUF_MASK    //����FIFO�����־λ��SPIΪ�ӻ�ģʽ��Tx FIFOΪ�գ����ⲿSPI����ģʽ�������䣬��־λ�ͻ���1��д1��0
                              | SPI_SR_TFFF_MASK    //����FIFO����־λ�� д1����DMA���������ִ���FIFO���˾ͻ���0�� 0��ʾTx FIFO����
                              | SPI_SR_RFOF_MASK    //����FIFO�����־λ��
                              | SPI_SR_RFDF_MASK    //����FIFO��ı�־λ��д1����DMA���������ִ���FIFO���˾ͻ���0��0��ʾRx FIFO��
                             );


    //�������ӻ�ģʽ���ù���ģʽ��MCU�ṩ�������Ƶ����1/2��Ƶ�����ӻ�Ƶ����1/4��Ƶ
    if(master == MASTER)
    {
        SPI_MCR_REG(SPIN[spin]) =  (0
                                    |  SPI_MCR_MSTR_MASK        //Master,����ģʽ
                                    |  SPI_MCR_PCSIS(pcs)
                                    |  SPI_MCR_PCSIS_MASK
                                   );


        for(br = 0;br < 0x10;br++)
        {
            for(pbr = 0;pbr < 4;pbr++)
            {
                tmp = Scaler[br] * Prescaler[pbr];
                diff = abs(tmp - clk);
                if(min_diff > diff)
                {
                    //��ס �������
                    min_diff = diff;
                    fit_br = br;
                    fit_pbr = pbr;

                    if(min_diff == 0)
                    {
                        //�պ�ƥ��

                        goto SPI_CLK_EXIT;
                    }

                }

            }
        }

SPI_CLK_EXIT:
        fit_clk =  bus_clk_khz *1000 /(Scaler[fit_br] * Prescaler[fit_pbr]);

        //��Ҫ��һ�� CS �� CLK ��ʱ�䣨�����ʵİ����ڣ�,���������� (1<<(CSSCK + 1 ))*( 2*PCSSCK+1) = fclk /(2*  baud)
        clk =  bus_clk_khz*1000/fit_clk/2;
        min_diff = ~0;
        fit_cssck = 0x0F;
        fit_pcssck = 3;
        for(cssck = 0;cssck<=0x0F;cssck++)
        {
            tmp = 1<<(cssck+1) ;
            pcssck = (clk/tmp-1)/2;
            if(pcssck>3)continue;       //���ܳ��� 3
            tmp = tmp * (2*pcssck+1);
            diff = abs(tmp-clk);
            if(min_diff > diff)
            {
                //��ס �������
                min_diff = diff;
                fit_cssck = cssck;
                fit_pcssck = pcssck;

                if(min_diff == 0)
                {
                    //�պ�ƥ��
                    goto SPI_CSSCK_EXIT;
                }
            }
        }

SPI_CSSCK_EXIT:

        SPI_CTAR_REG(SPIN[spin], 0) = (0
                                       //| SPI_CTAR_DBR_MASK    //˫������ ������ DBR=1��CPHA=1��PBR=00����SCK Duty Cycle Ϊ 50/50
                                       //| SPI_CTAR_CPHA_MASK   //������SCK�����ظı䣨����������½��ر���׽�������ȡ���������0����֮��  w25x16�������ض�ȡ���ݣ�NRF24L01�������ض�ȡ����
                                       | SPI_CTAR_PBR(fit_pbr)        //�����ʷ�Ƶ�� ��0~3 ��Ӧ�ķ�ƵֵPrescalerΪ 2��3��5��7

                                       | SPI_CTAR_PDT(0x00)     //��ʱ����Ϊ PDT*2+1 ������PDTΪ3������ʱ����Ϊ7��PDTΪ2bit
                                       | SPI_CTAR_BR(fit_br)         //�����ʼ�����ֵ ,��BR<=3,��ƵScaler Ϊ 2*��BR+1�� ����BR>=3����ƵScaler Ϊ 2^BR  ��BRΪ4bit
                                       //SCK ������ = (Bus clk/Prescaler) x [(1+DBR)/Scaler ]          fSYS Ϊ Bus clock
                                       //              50M / 2         x [ 1  /  2  ] = 25M   ��������������

                                       //| SPI_CTAR_CPOL_MASK   //ʱ�Ӽ��ԣ�1��ʾ SCK ����Ծ״̬Ϊ�ߵ�ƽ,   NRF24L01 ����ԾΪ�͵�ƽ
                                       | SPI_CTAR_FMSZ(0x07)    //ÿ֡���� 7bit+1 ����8bit ��FMSZĬ�Ͼ���8��
                                       // | SPI_CTAR_LSBFE_MASK //1Ϊ��λ��ǰ��

                                       // �������������ǵ��� CS �ź����˵���һ��CLK��ʱ��
                                       | SPI_CTAR_CSSCK(fit_cssck)    // x ��0~0x0F
                                       | SPI_CTAR_PCSSCK(fit_pcssck)    //����Ƭѡ�ź���Ч��ʱ�ӵ�һ�����س��ֵ���ʱ��Ԥ��Ƶֵ��tcsc��ʱԤ��Ƶ 2*x+1�� x 0~3
                                      );
    }
    else
    {
        //Ĭ�ϴӻ�ģʽ
        SPI_CTAR_SLAVE_REG(SPIN[spin], 0) = (0
                                             | SPI_CTAR_SLAVE_FMSZ(0x07)
                                             | SPI_CTAR_SLAVE_CPOL_MASK
                                             | SPI_CTAR_SLAVE_CPHA_MASK
                                            );
    }


    SPI_MCR_REG(SPIN[spin]) &= ~SPI_MCR_HALT_MASK;     //����SPI���䡣1Ϊ��ͣ��0Ϊ����

    return fit_clk;

}

/*!
 *  @brief      SPI���ͽ��պ���
 *  @param      SPIn_e          SPIģ��(SPI0��SPI1��SPI2)
 *  @param      SPIn_PCSn_e     Ƭѡ�ܽű��
 *  @param      modata          ���͵����ݻ�������ַ(����Ҫ������ NULL)
 *  @param      midata          ��������ʱ���յ������ݵĴ洢��ַ(����Ҫ������ NULL)
 *  @since      v5.0
 *  Sample usage:           spi_mosi(SPI0,SPIn_PCS0,buff,buff,2);    //����buff�����ݣ������յ�buff�����Ϊ2�ֽ�
 */
void spi_mosi(SPIn_e spin, SPI_PCSn_e pcs, uint8 *modata, uint8 *midata, uint32 len)
{
    uint32 i = 0;
    do
    {
        /*************  ���־λ  ***************/
        SPI_SR_REG(SPIN[spin]) = (0
                                  | SPI_SR_EOQF_MASK    //���Ͷ��п��ˣ�������ϱ�־
                                  | SPI_SR_TFUF_MASK    //����FIFO�����־λ��SPIΪ�ӻ�ģʽ��Tx FIFOΪ�գ����ⲿSPI����ģʽ�������䣬��־λ�ͻ���1��д1��0
                                  | SPI_SR_TFFF_MASK    //����FIFO����־λ�� д1����DMA���������ִ���FIFO���˾ͻ���0�� 0��ʾTx FIFO����
                                  | SPI_SR_RFOF_MASK    //����FIFO�����־λ��
                                  | SPI_SR_RFDF_MASK    //����FIFO��ı�־λ��д1����DMA���������ִ���FIFO���˾ͻ���0��0��ʾRx FIFO��
                                 );


        /************** ��FIFO������ **************/
        SPI_MCR_REG(SPIN[spin])    |=  (0
                                        | SPI_MCR_CLR_TXF_MASK  //д1�� Tx FIFO ������
                                        | SPI_MCR_CLR_RXF_MASK  //д1�� Rx FIFO ������
                                       );

    }
    while( (SPI_SR_REG(SPIN[spin]) & SPI_SR_RFDF_MASK));            //��� Rx FIFO �ǿգ�����FIFO.

    /***************** ����len-1������ *******************/                                                ;
    for(i = 0; i < (len - 1); i++)
    {
        SPI_PUSHR_REG(SPIN[spin]) = (0
                                     | SPI_PUSHR_CTAS(0)             //ѡ��CTAR0�Ĵ���
                                     | SPI_PUSHR_CONT_MASK           //1Ϊ �����ڼ䱣��PCSn�ź� ����������������
                                     | SPI_PUSHR_PCS(pcs)
                                     | SPI_PUSHR_TXDATA(modata[i])     //Ҫ���������
                                    );

        while(!(SPI_SR_REG(SPIN[spin]) & SPI_SR_RFDF_MASK));        //RFDFΪ1��Rx FIFO is not empty.
        if(midata != NULL)
        {
            midata[i] = (uint8)SPI_POPR_REG(SPIN[spin]);                  //������յ�������
        }
        else
        {
            SPI_POPR_REG(SPIN[spin]);
        }
        SPI_SR_REG(SPIN[spin]) |= SPI_SR_RFDF_MASK;
    }

    /***************** �������һ������ *******************/
    SPI_PUSHR_REG(SPIN[spin]) = (0
                                 | SPI_PUSHR_CTAS(0)                 //ѡ��CTAR0�Ĵ���
                                 | SPI_PUSHR_PCS(pcs)
                                 | SPI_PUSHR_EOQ_MASK                //1Ϊ ����SPI��������
                                 | SPI_PUSHR_TXDATA(modata[i])
                                );

    SPI_EOQF_WAIT(spin);                                            //�ȴ�������ɡ�(Ҫ��ʱ��RX FIFO�Ķ����������Ȼ��������޵ȴ�)

    while( !(SPI_SR_REG(SPIN[spin]) & SPI_SR_RFDF_MASK));           //RFDFΪ1��Rx FIFO is not empty.
    if(midata != NULL)
    {
        midata[i] = (uint8)SPI_POPR_REG(SPIN[spin]);                  //������յ�������
    }
    else
    {
        SPI_POPR_REG(SPIN[spin]);
    }
    SPI_SR_REG(SPIN[spin]) |= SPI_SR_RFDF_MASK;                     //д1���RFDF�����Rx FIFO �ǿյ�
}

/*!
 *  @brief      SPI���ͽ��պ���
 *  @param      SPIn_e          SPIģ��(SPI0��SPI1��SPI2)
 *  @param      SPIn_PCSn_e     Ƭѡ�ܽű��
 *  @param      mocmd           ���͵����������ַ(����Ҫ������ NULL)
 *  @param      micmd           ��������ʱ���յ������ݵĴ洢��ַ(����Ҫ������ NULL)
 *  @param      modata          ���͵����ݻ�������ַ(����Ҫ������ NULL)
 *  @param      midata          ��������ʱ���յ������ݵĴ洢��ַ(����Ҫ������ NULL)
 *  @since      v5.0
 *  Sample usage:           spi_mosi_cmd(SPI0,SPIn_PCS0,cmd,NULL,buff,buff,1,2);    //����cmd/buff�����ݣ�������cmd����ʱ�����ݣ�����buff����ʱ�����ݵ�buff����ȷֱ�Ϊ1��2�ֽ�
 */
void spi_mosi_cmd(SPIn_e spin, SPI_PCSn_e pcs, uint8 *mocmd , uint8 *micmd , uint8 *modata , uint8 *midata, uint32 cmdlen , uint32 len)
{
    uint32 i = 0;
    do
    {
        /*************  ���־λ  ***************/
        SPI_SR_REG(SPIN[spin]) = (0
                                  | SPI_SR_EOQF_MASK    //���Ͷ��п��ˣ�������ϱ�־
                                  | SPI_SR_TFUF_MASK    //����FIFO�����־λ��SPIΪ�ӻ�ģʽ��Tx FIFOΪ�գ����ⲿSPI����ģʽ�������䣬��־λ�ͻ���1��д1��0
                                  | SPI_SR_TFFF_MASK    //����FIFO����־λ�� д1����DMA���������ִ���FIFO���˾ͻ���0�� 0��ʾTx FIFO����
                                  | SPI_SR_RFOF_MASK    //����FIFO�����־λ��
                                  | SPI_SR_RFDF_MASK    //����FIFO��ı�־λ��д1����DMA���������ִ���FIFO���˾ͻ���0��0��ʾRx FIFO��
                                 );

        /************** ��FIFO������ **************/
        SPI_MCR_REG(SPIN[spin])    |=  (0
                                        | SPI_MCR_CLR_TXF_MASK      //д1�� Tx FIFO ������
                                        | SPI_MCR_CLR_RXF_MASK      //д1�� Rx FIFO ������
                                       );

    }
    while( (SPI_SR_REG(SPIN[spin]) & SPI_SR_RFDF_MASK));            //��� Rx FIFO �ǿգ�����FIFO.

    /***************** ����cmdlen������ *******************/                                                ;
    for(i = 0; i < cmdlen; i++)
    {
        SPI_PUSHR_REG(SPIN[spin]) = (0
                                     | SPI_PUSHR_CTAS(0)             //ѡ��CTAR0�Ĵ���
                                     | SPI_PUSHR_CONT_MASK           //1Ϊ �����ڼ䱣��PCSn�ź� ����������������
                                     | SPI_PUSHR_PCS(pcs)
                                     | SPI_PUSHR_TXDATA(mocmd[i])    //Ҫ���������
                                    );

        while(!(SPI_SR_REG(SPIN[spin]) & SPI_SR_RFDF_MASK));        //RFDFΪ1��Rx FIFO is not empty.
        if(micmd != NULL)
        {
            micmd[i] = (uint8)SPI_POPR_REG(SPIN[spin]);             //������յ�������
        }
        else
        {
            SPI_POPR_REG(SPIN[spin]);                               //��ȡFIFO����(������ȡ��������)
        }
        SPI_SR_REG(SPIN[spin]) |= SPI_SR_RFDF_MASK;
    }

    /***************** ����len-1������ *******************/                                                ;
    for(i = 0; i < (len - 1); i++)
    {
        SPI_PUSHR_REG(SPIN[spin]) = (0
                                     | SPI_PUSHR_CTAS(0)             //ѡ��CTAR0�Ĵ���
                                     | SPI_PUSHR_CONT_MASK           //1Ϊ �����ڼ䱣��PCSn�ź� ����������������
                                     | SPI_PUSHR_PCS(pcs)
                                     | SPI_PUSHR_TXDATA(modata[i])     //Ҫ���������
                                    );

        while(!(SPI_SR_REG(SPIN[spin]) & SPI_SR_RFDF_MASK));        //RFDFΪ1��Rx FIFO is not empty.

        if(midata != NULL)
        {
            midata[i] = (uint8)SPI_POPR_REG(SPIN[spin]);             //������յ�������
        }
        else
        {
            SPI_POPR_REG(SPIN[spin]);                               //��ȡFIFO����(������ȡ��������)
        }
        SPI_SR_REG(SPIN[spin]) |= SPI_SR_RFDF_MASK;
    }
    /***************** �������һ������ *******************/
    SPI_PUSHR_REG(SPIN[spin]) = (0
                                 | SPI_PUSHR_CTAS(0)          //ѡ��CTAR0�Ĵ���
                                 | SPI_PUSHR_PCS(pcs)
                                 | SPI_PUSHR_EOQ_MASK         //End Of Queue��1Ϊ ����SPI��������
                                 | SPI_PUSHR_TXDATA(modata[i])
                                );

    SPI_EOQF_WAIT(spin);    //Ҫ��ʱ��RX FIFO�Ķ����������Ȼ��������޵ȴ�

    while( !(SPI_SR_REG(SPIN[spin]) & SPI_SR_RFDF_MASK));    //RFDFΪ1��Rx FIFO is not empty.
    if(midata != NULL)
    {
        midata[i] = (uint8)SPI_POPR_REG(SPIN[spin]);             //������յ�������
    }
    else
    {
        SPI_POPR_REG(SPIN[spin]);                               //��ȡFIFO����(������ȡ��������)
    }
    SPI_SR_REG(SPIN[spin]) |= SPI_SR_RFDF_MASK;
}


