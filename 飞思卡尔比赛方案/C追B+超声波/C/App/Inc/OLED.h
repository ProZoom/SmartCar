/*
 * File:		nokia.h
 * Purpose:		Serial Input/Output routines
 *
 */

#ifndef _OELD_H
#define _OELD_H

/********************************************************************/
/*-----------------------------------------------------------------------
LCD_init          : 5110LCD初始化

编写日期          ：2012-11-01
最后修改日期      ：2012-11-01
-----------------------------------------------------------------------*/
 extern uint8 lanzhou96x64[768];
 void LCD_Init(void);
 void LCD_CLS(void);
 void LCD_P6x8Str(uint8 x,uint8 y,uint8 ch[]);
 void LCD_P8x16Str(uint8 x,uint8 y,uint8 ch[]);
 void LCD_P14x16Str(uint8 x,uint8 y,uint8 ch[]);
 void LCD_Print(uint8 x, uint8 y, uint8 ch[]);
 void LCD_PutPixel(uint8 x,uint8 y);
 void LCD_Rectangle(uint8 x1,uint8 y1,uint8 x2,uint8 y2,uint8 gif);
 void Draw_LQLogo(void);
 void Draw_LibLogo(void);
 void Draw_BMP(uint8 x0,uint8 y0,uint8 x1,uint8 y1,uint8 bmp[]); 
 void LCD_Fill(uint8 dat);
 void Dly_ms(uint16 ms);
 void Display(int16 i,uint8 x,uint8 y);
 void DisplayFloat(int16 i,uint8 x,uint8 y);

    

/********************************************************************/

#endif
