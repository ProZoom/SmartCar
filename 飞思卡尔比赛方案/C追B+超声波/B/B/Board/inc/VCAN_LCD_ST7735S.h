/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       VCAN_LCD_ST7735S.h
 * @brief      LCD ������
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-09-07
 */

#ifndef _VCAN_LCD_ST7735S_H_
#define _VCAN_LCD_ST7735S_H_

#include "MK60_gpio.h"
#include "VCAN_LCD.h"

//�û�����
#define ST7735S_DIR_DEFAULT     0       //����Ĭ�Ϸ��� (0~3)

#if (USE_LCD == LCD_ST7735S)

/*********************** API�ӿ� ******************************/
//�ṩAPI�ӿڸ�LCD����

#define LCD_H                   ST7735S_get_h()                 //��
#define LCD_W                   ST7735S_get_w()                 //��

#define LCD_INIT()              LCD_ST7735S_init()              //��ʼ��
#define LCD_PTLON(site,size)    LCD_ST7735S_ptlon(site,size)    //����
#define LCD_RAMWR()             LCD_WR_CMD(0x2C)                //дģʽ
#define LCD_WR_DATA(data)       do{LCD_ST7735S_WR_8DATA((uint8)((data)>>8) );LCD_ST7735S_WR_8DATA( (uint8)(data) );}while(0)       //д����
#define LCD_WR_CMD(cmd)         LCD_ST7735S_WR_8CMD(cmd)        //����
#define LCD_SET_DIR(opt)        LCD_ST7735S_dir(opt)            //����

#define LCD_DIR                 ST7735S_get_dir()               //��ȡ����


//ͨ���������ã��ٶ�̫���ˣ��ĳɺ궨�����
//#define LCD_WR_8DATA(data)      LCD_ST7735S_wr_data(data)       //д����
//#define LCD_WR_8CMD(cmd)        LCD_ST7735S_wr_cmd(cmd)         //����

#endif  //(USE_LCD == LCD_ST7735S)

/********************* ST7735S�ڲ�ʵ�� ***********************/
//LCD �ĹܽŶ���
#define     LCD_ST7735S_WR      PTC9
#define     LCD_ST7735S_RD      PTC10
#define     LCD_ST7735S_CS      PTC11
#define     LCD_ST7735S_RS      PTC12
#define     LCD_ST7735S_RST     PTC13

//�˲�����Ҫ�޸ĳ�ʼ����Ĵ���
#define     LCD_ST7735S_P0              PTC_B0_OUT
#define     LCD_ST7735S_PIN             PTC_B0_IN
#define     LCD_ST7735S_PDDR_OUT()      (DDRC_B0 = 0xff)
#define     LCD_ST7735S_PDDR_IN()       (DDRC_B0 = 0)


#define ST7735S_DELAY()                         //do{asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");}while(0)
#define ST7735S_DELAY_MS(ms)    DELAY_MS(ms)


extern void     LCD_ST7735S_init();
extern void     LCD_ST7735S_dir(uint8 option);
extern void     LCD_ST7735S_ptlon(Site_t site, Size_t size);
extern uint16   ST7735S_get_h();
extern uint16   ST7735S_get_w();
extern uint8    ST7735S_get_dir();


extern void LCD_ST7735S_wr_data(uint8 data);
extern void LCD_ST7735S_wr_cmd(uint8 cmd);

#define     LCD_ST7735S_WR_OUT      PTXn_T(LCD_ST7735S_WR,OUT)
#define     LCD_ST7735S_RD_OUT      PTXn_T(LCD_ST7735S_RD,OUT)
#define     LCD_ST7735S_CS_OUT      PTXn_T(LCD_ST7735S_CS,OUT)
#define     LCD_ST7735S_RS_OUT      PTXn_T(LCD_ST7735S_RS,OUT)
#define     LCD_ST7735S_RST_OUT     PTXn_T(LCD_ST7735S_RST,OUT)

#define LCD_ST7735S_WR_8CMD(cmd)    do\
                            {\
                                LCD_ST7735S_RD_OUT=1;\
                                LCD_ST7735S_RS_OUT=0;\
                                LCD_ST7735S_CS_OUT=0;\
                                LCD_ST7735S_P0=(uint8)(cmd);\
                                LCD_ST7735S_WR_OUT=0;\
                                LCD_ST7735S_WR_OUT=1;\
                                LCD_ST7735S_CS_OUT=1;\
                            }while(0)   //LCD_WR=0;LCD_WR=1;����һ��������

#define LCD_ST7735S_WR_8DATA(data)  do\
                            {\
                                LCD_ST7735S_RD_OUT=1;\
                                LCD_ST7735S_RS_OUT=1;\
                                LCD_ST7735S_CS_OUT=0;\
                                LCD_ST7735S_P0=(uint8)(data);\
                                LCD_ST7735S_WR_OUT=0;\
                                LCD_ST7735S_WR_OUT=1;\
                                LCD_ST7735S_CS_OUT=1;\
                            }while(0)   //LCD_WR=0;������д�����ݵ�RAM


#define LCD_ST7735S_WR_DATA(data)       do{LCD_ST7735S_WR_8DATA((uint8)((data)>>8) );LCD_ST7735S_WR_8DATA( (uint8)(data) );}while(0)       //д����



#endif  //_VCAN_LCD_ST7735S_H_
