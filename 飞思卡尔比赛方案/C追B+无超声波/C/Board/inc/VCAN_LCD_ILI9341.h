/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       VCAN_LCD_ILI9341.h
 * @brief      LCD ILI9341������
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-09-03
 */
#ifndef _VCAN_LCD_ILI9341_H_
#define _VCAN_LCD_ILI9341_H_

#include "VCAN_LCD.h"
#include "MK60_flexbus.h"


//�û�����
#define ILI9341_DIR_DEFAULT     1       //����Ĭ�Ϸ��� (0~3)


#if (USE_LCD == LCD_ILI9341)

/*********************** API�ӿ� ******************************/
//�ṩAPI�ӿڸ�LCD����

#define LCD_H                   ILI9341_get_h()                 //��
#define LCD_W                   ILI9341_get_w()                 //��

#define LCD_INIT()              LCD_ILI9341_init()              //��ʼ��
#define LCD_PTLON(site,size)    LCD_ILI9341_ptlon(site,size)    //����
#define LCD_RAMWR()             LCD_ILI9341_WR_CMD(0x2C)        //дģʽ
#define LCD_WR_DATA(data)       LCD_ILI9341_WR_DATA(data)       //д����
#define LCD_WR_CMD(cmd)         LCD_ILI9341_WR_CMD(cmd)         //����
#define LCD_SET_DIR(opt)        LCD_ILI9341_dir(opt)            //����

#define LCD_DIR                 ILI9341_get_dir()               //��ȡ����

#endif //(USE_LCD == LCD_ILI9341)

/********************* ILI9341�ڲ�ʵ�� ***********************/

#define LCD_ILI9341_CMD_ADDR                    FB_8080_CMD
#define LCD_ILI9341_DATA_ADDR                   FB_8080_DATA

#define LCD_ILI9341_WR_CMD(cmd)         do{LCD_ILI9341_CMD_ADDR  = cmd ;}while(0)
#define LCD_ILI9341_WR_DATA(data)       do{LCD_ILI9341_DATA_ADDR = data;}while(0)


extern void     LCD_ILI9341_init();
extern void     LCD_ILI9341_dir(uint8 option);
extern void     LCD_ILI9341_ptlon(Site_t site, Size_t size);
extern uint16   ILI9341_get_h();
extern uint16   ILI9341_get_w();
extern uint8    ILI9341_get_dir();




#endif //   _VCAN_LCD_ILI9341_H_