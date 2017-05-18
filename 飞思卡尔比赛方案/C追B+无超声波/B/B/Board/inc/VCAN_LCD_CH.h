#ifndef _VCAN_LCD_CH_H_
#define _VCAN_LCD_CH_H_

//�����ֿ����� ����Ҫ�� SD�� ��Ŀ¼ �� ���� HZLIB.bin �ַ��ļ� ��
#define LCD_CH_H    16
#define LCD_CH_W    16

#define LCD_CH_SIZE     (LCD_CH_W*LCD_CH_H/8)         //����ռ�ÿռ��С



extern void LCD_Char_CH (Site_t site,const uint8 *str,uint16 Color,uint16 bkColor);
extern void LCD_Str_CH  (Site_t site,const uint8 *str, uint16 Color ,uint16 bkColor);   //�������ַ���
extern void LCD_Str_ENCH(Site_t site,const uint8 *str  , uint16 Color ,uint16 bkColor); //��Ӣ����ַ���

//�������ʾ �������ǵ��� flash ����ַ�������ʾ����
extern void LCD_FChar_CH (Site_t site,const uint8 *strbuf,             uint16 Color,uint16 bkColor);
extern void LCD_FStr_CH  (Site_t site,const uint8 *strbuf,uint16 num,  uint16 Color,uint16 bkColor);

#define LCD_FSTR_CH(site,strbuf,Color,bkColor)   LCD_FStr_CH (site,strbuf,sizeof(strbuf)/LCD_CH_SIZE,Color,bkColor)


#endif  //ifndef _VCAN_LCD_CH_H_