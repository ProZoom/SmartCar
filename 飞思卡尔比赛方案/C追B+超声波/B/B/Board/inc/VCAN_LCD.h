/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       VCAN_LCD.h
 * @brief      LCD ������
 * @author     ɽ��Ƽ�
 * @version    v5.1
 * @date       2013-09-02
 */



#include "VCAN_LCD_ILI9341.h"
#include "VCAN_LCD_ST7735S.h"
#include "VCAN_LCD_ST7735R.h"

#include "VCAN_ASCII.h"
#include "VCAN_LCD_CH.h"                        //������ʾ


/******������ɫ*****/
#define RED     0XF800    //��ɫ
#define GREEN   0X07E0    //��ɫ
#define BLUE    0X001F    //��ɫ
#define BRED    0XF81F
#define GRED    0XFFE0    //��ɫ
#define GBLUE   0X07FF    //
#define BLACK   0X0000    //��ɫ
#define WHITE   0XFFFF    //��ɫ
#define YELLOW  0xFFE0    //��ɫ

#define FCOLOUR         BLUE    //����������ɫ
#define BCOLOUR         RED     //���屳����ɫ

//��ȡRGB��3Ԫ�أ�rgb565������16λ
#define RGB565_R(rgb565)                ((uint8)(((uint16)(rgb565)>>11) &0x1F))
#define RGB565_G(rgb565)                ((uint8)(((uint16)(rgb565)>> 5) &0x3F))
#define RGB565_B(rgb565)                ((uint8)( (uint16)(rgb565)      &0x1F))


#define GRAY_2_RGB565(gray)             ((uint16)((((uint8)(gray)>>3)<<11)|(((uint8)(gray)>>2)<<5)|((uint8)(gray)>>3)))

#define RGB565_2_GRAY(rgb565)           ((uint8)(((RGB565_R(rgb565)*235+RGB565_G(rgb565)*613+RGB565_B(rgb565)*625)+1)>>8))  //  31*235+63*613+31*625+1  = 255*256


#define RGB565_H(rgb565)                ((uint8)(((uint16)(rgb565))>>8))
#define RGB565_L(rgb565)                (uint8)(rgb565))

#define RGB24_RGB565(R,G,B)             ((uint16)((((uint8)(R)>>3)<<11)|(((uint8)(G)>>2)<<5)|((uint8)(B)>>3)))
#define RGB555_RGB565(rgb555)           (((((uint16)(rgb555)) & (uint16)(~0x1F)) << 1) |  (((uint16)(rgb555)) & 0x1F))
#define BGR555_RGB565(bgr555)           ((((uint16)(bgr555)<<11) & (uint16)(0x1F<<11)) | (((uint16)(bgr555)) & (uint16)(0x1F<<5)) | (((uint16)(bgr555)>>10) & (uint16)(0x1F)))



//��Ҫ�ײ�ʵ�ֵļ����궨�壺
//LCD_H                         //��
//LCD_W                         //��

//LCD_INIT()                    //��ʼ��
//LCD_PTLON(site,size)          //����
//LCD_RAMWR()                   //дģʽ
//LCD_WR_DATA(data)             //д����
//LCD_WR_DATA_B(data)           //д����(���)��16λҺ����LCD_WR_DATAһ�¼��ɣ�8λҺ������˳��

//����Ϊ��ѡ
//LCD_RAMRD()                   //��ģʽ
//LCD_SET_DIR(opt)              //������ʾ����(0~3)
//LCD_DIR                       //��ȡ��ʾ����(0~3)


//����Ϊ�ڲ����ã���������ĺ궨�����
//LCD_WR_CMD(cmd)               //����
//LCD_RD_DATA()                 //������

//8λҺ��������Ҫ������Ĭ����16λ��
//LCD_WR_8CMD(cmd)               //����
//LCD_WR_8DATA(data)            //д����


//�����ֿⶨ�� Ӣ����ĸ�� ��ߡ�
#define LCD_EN_W        8
#define LCD_EN_H        16


/***************  LCD��ʼ��  ***************/

void LCD_init(void);                                                                                    //LCD��ʼ��

/***************  LCD�滭  ***************/

extern void LCD_point          (Site_t,                                        uint16 rgb565);                 //����
extern void LCD_points         (Site_t *site,  uint32 point_num,               uint16 rgb565);                 //��һ�ѵ�
extern void LCD_rectangle      (Site_t, Size_t,                                uint16 rgb565);                 //������
extern void LCD_char           (Site_t,        uint8 ascii,                    uint16 Color, uint16 bkColor);  //��ʾ8*16�ַ�
extern void LCD_str            (Site_t,        uint8 *Str,                     uint16 Color, uint16 bkColor);  //��ʾ8*16�ַ���

extern void LCD_cross          (Site_t,uint16 len,                              uint16 rgb565);                 //��ʮ����

extern void LCD_clear          (uint16 rgb565);     //����


extern void LCD_num            (Site_t,        uint32 num,                     uint16 Color, uint16 bkColor);  //��ʾ����
#define MAX_NUM_BIT 5                                                                                   //���ֵ����λ������������������Ļ�������֣�
#define LCD_num_C(site,num,color,bkColor)           LCD_num_BC(site, num,MAX_NUM_BIT,color,bkColor)     //��ʾ���֣�����������Ļ�������֣�
extern void LCD_num_BC         (Site_t,        uint32 num, uint8 max_num_bit,  uint16 Color, uint16 bkColor);  //��ʾ���֣�����������Ļ�������֣�

extern void LCD_Img_gray       (Site_t site, Size_t size, uint8 *img);                   //��ʾ�Ҷ�ͼ��
extern void LCD_Img_gray_Z     (Site_t site, Size_t size, uint8 *img, Size_t imgsize);   //��ʾ�Ҷ�ͼ��(������)

#define BINARY_BGCOLOR  WHITE       //�����ֵ��ͼ�񱳾���ɫ
#define BINARY_COLOR    BLACK       //�����ֵ��ͼ��ǰ����ɫ
extern void LCD_Img_Binary     (Site_t site, Size_t size, uint8 *img);                   //��ʾ��ֵ��ͼ��
extern void LCD_Img_Binary_Z   (Site_t site, Size_t size, uint8 *img, Size_t imgsize);   //��ʾ��ֵ��ͼ��(������)

extern void LCD_wave(Site_t site,Size_t size,uint8 *img,uint8 maxval,uint16 Color ,uint16 bkColor); //������ʾ����������ɫ��maxval ���ֵ��ʾ������ֵ�������������ֵ����
extern void LCD_wave_display(Site_t site,Size_t size,uint8 *img,uint8 maxval,uint16 Color);        //������ʾ������������ɫ


