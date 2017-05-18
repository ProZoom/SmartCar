/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_usb.c
 * @brief      USB �����(Ŀǰ �� �� ���� ���� )
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-09-26
 * @note       ������ ��ֲ ��˼������˾ �ṩ�� K60 �ο�����
 */


#include "common.h"
#include "usb.h"
#include "MK60_usb.h"


/*!
 *  @brief      USBģ�� ��ʼ��
 *  @since      v5.0
 */
void usb_init(void)
{
    /* ������� */
    Setup_Pkt = (tUSB_Setup *)BufferPointer[bEP0OUT_ODD];
    gu8USB_State = uPOWER;                          //���״̬Ϊ�ϵ�״̬
    /* MPU ���� */
    MPU_CESR = 0;                                   // MPU is disable. All accesses from all bus masters are allowed

    /* SIM ���� */
#ifdef USB_CLOCK_CLKIN
    FLAG_SET(SIM_SCGC5_PORTE_SHIFT, SIM_SCGC5);
    PORTE_PCR26 = (0 | PORT_PCR_MUX(7));            // Enabling PTE26 as CLK input

#endif


#if defined(MK60DZ10)
#ifdef USB_CLOCK_PLL
    FLAG_SET(SIM_SOPT2_PLLFLLSEL_SHIFT, SIM_SOPT2); // Select PLL output
#endif


#ifndef USB_CLOCK_CLKIN
    FLAG_SET(SIM_SOPT2_USBSRC_SHIFT, SIM_SOPT2);    // PLL/FLL selected as CLK source
#endif

    SIM_CLKDIV2 = ((SIM_CLKDIV2 & ~( SIM_CLKDIV2_USBDIV_MASK | SIM_CLKDIV2_USBFRAC_MASK ))
                   | SIM_CLKDIV2_USBDIV(USB_CLK_DIV)                    //USB ��Ƶ����
                   | (USB_CLK_FRAC << SIM_CLKDIV2_USBFRAC_SHIFT)        //USB ��Ƶ����
                   //USB clk = PLL �� [ (USBFRAC+1) / (USBDIV+1) ]
                  );
    SIM_SCGC4 |= SIM_SCGC4_USBOTG_MASK;             //USB Clock Gating
                                                    //����USBģ���ʱ��Դ
#elif defined(MK60F15)
#ifdef USB_CLOCK_PLL
    SIM_SOPT2 |=(0
                | SIM_SOPT2_PLLFLLSEL(1)       /** PLL0 reference */   
                | SIM_SOPT2_USBFSRC(0)         /** MCGPLLCLK as CLKC source */
                | SIM_SOPT2_USBF_CLKSEL_MASK   /** USB fractional divider like USB reference clock */  
                );

#endif


#ifndef USB_CLOCK_CLKIN
    FLAG_SET(SIM_SOPT2_USBHSRC_SHIFT, SIM_SOPT2);    // PLL/FLL selected as CLK source
#endif

    SIM_CLKDIV2 = ((SIM_CLKDIV2 & ~( SIM_CLKDIV2_USBFSDIV_MASK | SIM_CLKDIV2_USBFSFRAC_MASK ))
                   | SIM_CLKDIV2_USBFSDIV(USB_CLK_DIV)                  //USB ��Ƶ����
                   | (USB_CLK_FRAC << SIM_CLKDIV2_USBFSFRAC_SHIFT)      //USB ��Ƶ����
                   //USB clk = PLL �� [ (USBFRAC+1) / (USBDIV+1) ]
                  );

    //SIM_SCGC6 |= SIM_SCGC6_USB2OTG_MASK;
    SIM_SCGC4 |= SIM_SCGC4_USBFS_MASK;             //USB Clock Gating
                                                    //����USBģ���ʱ��Դ
#endif


    /* NVICģ������ */
    set_vector_handler(USB0_VECTORn, USB_ISR);
    enable_irq(USB0_IRQn);                          //ʹ��NVIC�е�USB OTG�ж�

    /* USBģ������ */
    USB0_USBTRC0 |= USB_USBTRC0_USBRESET_MASK;      //��λUSBģ��
    while(FLAG_CHK(USB_USBTRC0_USBRESET_SHIFT, USB0_USBTRC0));
    USB0_BDTPAGE1 = (uint8)((uint32)tBDTtable >> 8); //���õ�ǰ������������BDT
    USB0_BDTPAGE2 = (uint8)((uint32)tBDTtable >> 16);
    USB0_BDTPAGE3 = (uint8)((uint32)tBDTtable >> 24);

    // �� USB ��λ���
    FLAG_SET(USB_ISTAT_USBRST_MASK, USB0_ISTAT);

    // ʹ�� USB ��λ�ж�
    FLAG_SET(USB_INTEN_USBRSTEN_SHIFT, USB0_INTEN);

    // Enable weak pull downs
    USB0_USBCTRL = 0x40;

    USB0_USBTRC0 |= 0x40;

    USB0_CTL |= 0x01;

    // ����ʹ��
    FLAG_SET(USB_CONTROL_DPPULLUPNONOTG_SHIFT, USB0_CONTROL);
}


/*!
 *  @brief      USB ���⴮�� ��ʼ��
 *  @since      v5.0
 */
void usb_com_init(void)
{
    usb_init(); //��ʼ��USBģ��

    CDC_Init(); //��ʼ��USB CDCģʽ
}


/*!
 *  @brief      USB �ȴ�ö��
 *  @since      v5.0
 */
void usb_enum_wait(void)
{
    while(gu8USB_State != uENUMERATED);//�ȴ�USB�豸��ö��
}

/*!
 *  @brief      USB ���⴮�� ����
 *  @param      rx_buf          ���ݽ��ջ�����
 *  @return     ���յ��ĳ���
 *  @since      v5.0
 */
uint8 usb_com_rx(uint8_t *rx_buf)
{
    uint8 len;
    uint8 temp = 0;
    uint8 *pu8EPBuffer;

    if(FLAG_CHK(EP_OUT, gu8USB_Flags)) // ��������ݵ���
    {
        len = USB_EP_OUT_SizeCheck(EP_OUT);
        temp = len;
        pu8EPBuffer = gu8EP3_OUT_ODD_Buffer;

        while(len--)
            *rx_buf++ = *pu8EPBuffer++;

        usbEP_Reset(EP_OUT);
        usbSIE_CONTROL(EP_OUT);
        FLAG_CLR(EP_OUT, gu8USB_Flags);
    }
    return temp;
}


/*!
 *  @brief      USB ���⴮�� ����
 *  @param      tx_buf          ���ݷ��ͻ�����
 *  @param      ��Ҫ���͵����ݳ���
 *  @since      v5.0
 */
void usb_com_tx(uint8 *tx_buf, uint8 len)
{
    EP_IN_Transfer(EP_IN, tx_buf, len);
}




