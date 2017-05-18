/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       VCAN_LCD_ILI9341.c
 * @brief      LCD ILI9341������
 * @author     ɽ��Ƽ�
 * @version    v5.1
 * @date       2014-02-03
 */

#include "common.h"
#include "MK60_gpio.h"
#include "VCAN_LCD_ILI9341.h"



#if (USE_LCD == LCD_ILI9341)

//�������õ��� IO�ܽ�
#define LCD_RST    PTC13                //��λ
#define LCD_BL     PTB8                 //����


#define ILI9341_DELAY()          DELAY_MS(100)
#define ILI9341_DELAYMS(ms)      DELAY_MS(ms)


#define ILI9341_H       240
#define ILI9341_W       320

#if ((ILI9341_DIR_DEFAULT&1 )== 0)      //����
uint16  ili9341_h   = ILI9341_H;
uint16  ili9341_w   = ILI9341_W;
#else
uint16  ili9341_h   = ILI9341_W;
uint16  ili9341_w   = ILI9341_H;
#endif
uint8   ili9341_dir = ILI9341_DIR_DEFAULT;
/*!
 *  @brief      LCD_ILI9341��ʼ��
 *  @since      v5.0
 */
void    LCD_ILI9341_init()
{
    gpio_init (LCD_BL, GPO, 1); //LCD����ܽ����1����ʾ�ر�LCD����

    //��λLCD
    gpio_init (LCD_RST, GPO, 0);

    ILI9341_DELAYMS(1);
    GPIO_SET   (LCD_RST, 1);

    //��ʼ������
    flexbus_8080_init();

    ILI9341_DELAY();
    LCD_ILI9341_WR_CMD(0xCF);
    LCD_ILI9341_WR_DATA(0x00);
    LCD_ILI9341_WR_DATA(0x81);
    LCD_ILI9341_WR_DATA(0x30);

    ILI9341_DELAY();
    LCD_ILI9341_WR_CMD(0xED);
    LCD_ILI9341_WR_DATA(0x64);
    LCD_ILI9341_WR_DATA(0x03);
    LCD_ILI9341_WR_DATA(0x12);
    LCD_ILI9341_WR_DATA(0x81);

    ILI9341_DELAY();
    LCD_ILI9341_WR_CMD(0xE8);
    LCD_ILI9341_WR_DATA(0x85);
    LCD_ILI9341_WR_DATA(0x10);
    LCD_ILI9341_WR_DATA(0x78);

    ILI9341_DELAY();
    LCD_ILI9341_WR_CMD(0xCB);
    LCD_ILI9341_WR_DATA(0x39);
    LCD_ILI9341_WR_DATA(0x2C);
    LCD_ILI9341_WR_DATA(0x00);
    LCD_ILI9341_WR_DATA(0x34);
    LCD_ILI9341_WR_DATA(0x02);

    ILI9341_DELAY();
    LCD_ILI9341_WR_CMD(0xF7);
    LCD_ILI9341_WR_DATA(0x20);

    ILI9341_DELAY();
    LCD_ILI9341_WR_CMD(0xEA);
    LCD_ILI9341_WR_DATA(0x00);
    LCD_ILI9341_WR_DATA(0x00);

    ILI9341_DELAY();
    LCD_ILI9341_WR_CMD(0xB1);
    LCD_ILI9341_WR_DATA(0x00);
    LCD_ILI9341_WR_DATA(0x1B);

    ILI9341_DELAY();
    LCD_ILI9341_WR_CMD(0xB6);
    LCD_ILI9341_WR_DATA(0x0A);
    LCD_ILI9341_WR_DATA(0xA2);

    ILI9341_DELAY();
    LCD_ILI9341_WR_CMD(0xC0);
    LCD_ILI9341_WR_DATA(0x35);

    ILI9341_DELAY();
    LCD_ILI9341_WR_CMD(0xC1);
    LCD_ILI9341_WR_DATA(0x11);

    LCD_ILI9341_WR_CMD(0xC5);
    LCD_ILI9341_WR_DATA(0x45);
    LCD_ILI9341_WR_DATA(0x45);

    LCD_ILI9341_WR_CMD(0xC7);
    LCD_ILI9341_WR_DATA(0xA2);

    LCD_ILI9341_WR_CMD(0xF2);
    LCD_ILI9341_WR_DATA(0x00);

    LCD_ILI9341_WR_CMD(0x26);
    LCD_ILI9341_WR_DATA(0x01);
    ILI9341_DELAY();
    LCD_ILI9341_WR_CMD(0xE0); //Set Gamma
    LCD_ILI9341_WR_DATA(0x0F);
    LCD_ILI9341_WR_DATA(0x26);
    LCD_ILI9341_WR_DATA(0x24);
    LCD_ILI9341_WR_DATA(0x0B);
    LCD_ILI9341_WR_DATA(0x0E);
    LCD_ILI9341_WR_DATA(0x09);
    LCD_ILI9341_WR_DATA(0x54);
    LCD_ILI9341_WR_DATA(0xA8);
    LCD_ILI9341_WR_DATA(0x46);
    LCD_ILI9341_WR_DATA(0x0C);
    LCD_ILI9341_WR_DATA(0x17);
    LCD_ILI9341_WR_DATA(0x09);
    LCD_ILI9341_WR_DATA(0x0F);
    LCD_ILI9341_WR_DATA(0x07);
    LCD_ILI9341_WR_DATA(0x00);
    LCD_ILI9341_WR_CMD(0XE1); //Set Gamma
    LCD_ILI9341_WR_DATA(0x00);
    LCD_ILI9341_WR_DATA(0x19);
    LCD_ILI9341_WR_DATA(0x1B);
    LCD_ILI9341_WR_DATA(0x04);
    LCD_ILI9341_WR_DATA(0x10);
    LCD_ILI9341_WR_DATA(0x07);
    LCD_ILI9341_WR_DATA(0x2A);
    LCD_ILI9341_WR_DATA(0x47);
    LCD_ILI9341_WR_DATA(0x39);
    LCD_ILI9341_WR_DATA(0x03);
    LCD_ILI9341_WR_DATA(0x06);
    LCD_ILI9341_WR_DATA(0x06);
    LCD_ILI9341_WR_DATA(0x30);
    LCD_ILI9341_WR_DATA(0x38);
    LCD_ILI9341_WR_DATA(0x0F);
    ILI9341_DELAY();


    ILI9341_DELAY();
    LCD_ILI9341_WR_CMD(0x3a); // Memory Access Control
    LCD_ILI9341_WR_DATA(0x55);
    LCD_ILI9341_WR_CMD(0x11); //Exit Sleep
    ILI9341_DELAY();
    LCD_ILI9341_WR_CMD(0x29); //display on


    LCD_SET_DIR(ili9341_dir);   //Һ��������ʾ����

    LCD_ILI9341_WR_CMD(0x2c);

    PTXn_T(LCD_BL,OUT) = 0;     //��LCD����
}

/*!
 *  @brief      ����ILI9341GRAMָ��ɨ�跽��
 *  @param      option    ����ѡ��0~3��
 *  @since      v5.0
 */
void LCD_ILI9341_dir(uint8 option)
{

    option = option % 4;


    ili9341_dir = option;


    switch(option)
    {
        case 0:
        {
            /*����*/
            LCD_ILI9341_WR_CMD(0x36);
            LCD_ILI9341_WR_DATA(0xA8);    //����

            LCD_ILI9341_WR_CMD(0X2A);
            LCD_ILI9341_WR_DATA(0x00);  //start
            LCD_ILI9341_WR_DATA(0x00);
            LCD_ILI9341_WR_DATA(0x01);  //end
            LCD_ILI9341_WR_DATA(0x3F);

            LCD_ILI9341_WR_CMD(0X2B);
            LCD_ILI9341_WR_DATA(0x00);   //start
            LCD_ILI9341_WR_DATA(0x00);
            LCD_ILI9341_WR_DATA(0x00);   //end
            LCD_ILI9341_WR_DATA(0xEF);

            ili9341_h   = ILI9341_H;
            ili9341_w   = ILI9341_W;
        }
        break;
        case 1:
        {
            /*����*/
            LCD_ILI9341_WR_CMD(0x36);
            LCD_ILI9341_WR_DATA(0xD8);  //����

            LCD_ILI9341_WR_CMD(0X2A);
            LCD_ILI9341_WR_DATA(0x00);
            LCD_ILI9341_WR_DATA(0x00);
            LCD_ILI9341_WR_DATA(0x00);
            LCD_ILI9341_WR_DATA(0xEF);

            LCD_ILI9341_WR_CMD(0X2B);
            LCD_ILI9341_WR_DATA(0x00);
            LCD_ILI9341_WR_DATA(0x00);
            LCD_ILI9341_WR_DATA(0x01);
            LCD_ILI9341_WR_DATA(0x3F);

            ili9341_h   = ILI9341_W;
            ili9341_w   = ILI9341_H;
        }
        break;
        case 2:
        {
            /*����*/
            LCD_ILI9341_WR_CMD(0x36);
            LCD_ILI9341_WR_DATA(0x68);    //����

            LCD_ILI9341_WR_CMD(0X2A);
            LCD_ILI9341_WR_DATA(0x00);  //start
            LCD_ILI9341_WR_DATA(0x00);
            LCD_ILI9341_WR_DATA(0x01);  //end
            LCD_ILI9341_WR_DATA(0x3F);

            LCD_ILI9341_WR_CMD(0X2B);
            LCD_ILI9341_WR_DATA(0x00);   //start
            LCD_ILI9341_WR_DATA(0x00);
            LCD_ILI9341_WR_DATA(0x00);   //end
            LCD_ILI9341_WR_DATA(0xEF);
            ili9341_h   = ILI9341_H;
            ili9341_w   = ILI9341_W;
        }
        break;
        case 3:
        {
            /*����*/
            LCD_ILI9341_WR_CMD(0x36);
            LCD_ILI9341_WR_DATA(0x18);  //����

            LCD_ILI9341_WR_CMD(0X2A);
            LCD_ILI9341_WR_DATA(0x00);
            LCD_ILI9341_WR_DATA(0x00);
            LCD_ILI9341_WR_DATA(0x00);
            LCD_ILI9341_WR_DATA(0xEF);

            LCD_ILI9341_WR_CMD(0X2B);
            LCD_ILI9341_WR_DATA(0x00);
            LCD_ILI9341_WR_DATA(0x00);
            LCD_ILI9341_WR_DATA(0x01);
            LCD_ILI9341_WR_DATA(0x3F);

            ili9341_h   = ILI9341_W;
            ili9341_w   = ILI9341_H;
        }
        break;
        default:
            //���ڿ�ͷ��У�飬�������ִ�е�����
            //LCD_ILI9341_dir(ILI9341_DIR_DEFAULT);
            break;

    }

}

/*!
 *  @brief      ����ILI9341����
 *  @param      site        ���Ͻ�����λ��
 *  @param      size        ������С
 *  @since      v5.0
 */
void LCD_ILI9341_ptlon(Site_t site, Size_t size)
{

    LCD_ILI9341_WR_CMD(0X2A);
    LCD_ILI9341_WR_DATA(site.x >> 8); //start
    LCD_ILI9341_WR_DATA(site.x & 0xFF);
    LCD_ILI9341_WR_DATA((site.x + size.W - 1) >> 8); //end
    LCD_ILI9341_WR_DATA((site.x  + size.W - 1) & 0xFF);

    LCD_ILI9341_WR_CMD(0X2B);
    LCD_ILI9341_WR_DATA(site.y >> 8); //start
    LCD_ILI9341_WR_DATA(site.y & 0xFF);
    LCD_ILI9341_WR_DATA((site.y + size.H - 1) >> 8); //end
    LCD_ILI9341_WR_DATA((site.y + size.H - 1) & 0xFF);
}

/*!
 *  @brief      ��ȡ ILI9341 �߶�
 *  @return     ILI9341 �߶�
 *  @since      v5.0
 */
uint16 ILI9341_get_h()
{
    return ili9341_h;
}

/*!
 *  @brief      ��ȡ ILI9341 ���
 *  @return     ILI9341 ���
 *  @since      v5.0
 */
uint16 ILI9341_get_w()
{
    return ili9341_w;
}

/*!
 *  @brief      ��ȡ ILI9341 ��ʾ����
 *  @return     ILI9341 ����
 *  @since      v5.0
 */
uint8 ILI9341_get_dir()
{
    return ili9341_dir;
}


#endif //(USE_LCD == LCD_ILI9341)

