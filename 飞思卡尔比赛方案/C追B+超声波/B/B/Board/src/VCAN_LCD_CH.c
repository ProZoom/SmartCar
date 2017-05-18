#include "common.h"


#include "VCAN_lcd.h"
#include "ff.h"





FATFS myfs;                    // Work area (file system object) for logical drive
FIL myfsrc;            // file objects
FRESULT myres;                 // FatFs function common result code
uint32 mybr;               // File R/W count

/*******************************************************************************
* Function Name  : GetGBKCode_from_sd
* Description    : ��SD���ֿ��ж�ȡ�������ݵ�ָ���Ļ�����
* Input          : pBuffer---���ݱ����ַ
*				   					c--�����ַ����ֽ���
* Output         : None
* Return         : 0(success)  -1(fail)
* Attention		 	 : None
*******************************************************************************/
int GetGBKCode_from_sd(unsigned char* pBuffer,const unsigned char * c)
{
    unsigned char High8bit,Low8bit;
    unsigned int pos;
    High8bit=*c;     /* ȡ��8λ���� */
    Low8bit=*(c+1);  /* ȡ��8λ���� */

    pos = ((High8bit-0xb0)*94+Low8bit-0xa1)*LCD_CH_SIZE ;	
    f_mount(0, &myfs);
    myres = f_open(&myfsrc , "0:/HZLIB.bin", FA_OPEN_EXISTING | FA_READ);

    if ( myres == FR_OK )
    {
        f_lseek (&myfsrc, pos);														 //ָ��ƫ��
        myres = f_read( &myfsrc, pBuffer, LCD_CH_SIZE, &mybr );		 //16*16��С�ĺ��� ����ģ ռ��16*2���ֽ�

        f_close(&myfsrc);

        return 0;
    }
    else
    {
        return -1;
    }
}

/********************************************************************
 * ��������LCD_Char_CH
 * ����  ����ʾ���������ַ�
 * ����  : 	x: 0~(319-16)
 *         	y: 0~(239-16)
 *			str: �����ַ�����ַ
 *			Color: �ַ���ɫ
 *			bkColor: ������ɫ
 * ���  ����
 * ����  ��	LCD_Char_CH(200,100,"��",0,0);
 * ע��	 ������������1�ĺ����ַ�������ʾ����ضϣ�ֻ��ʾ��ǰ��һ������
************************************************************************/
void LCD_Char_CH(Site_t site,const uint8 *str,uint16 Color,uint16 bkColor)
{
    uint8 i,j;
    uint8 buffer[32];
    uint8 *pbuf = buffer;
    uint8 tmp_char=0;

    const Size_t size   = {LCD_CH_W,LCD_CH_H};

    GetGBKCode_from_sd(buffer,str);  /* ȡ��ģ���� */

    LCD_PTLON(site, size);              //����
    LCD_RAMWR();                        //д�ڴ�

    for (i=0;i < LCD_CH_SIZE;i++)
    {
        tmp_char = *pbuf++;

        for(j=0;j<8;j++)
        {
            if(tmp_char & (0x80))
            {
                LCD_WR_DATA( Color );
            }
            else
            {
                LCD_WR_DATA( bkColor );
            }
            tmp_char <<= 1;
        }
    }
}

void LCD_Str_CH(Site_t site,const uint8 *str  , uint16 Color ,uint16 bkColor) 		//��ʾ16*16�����ַ���
{
    while(*str != '\0')
    {
        if(site.x>(LCD_W-16))
            {	
             /*����*/
            site.x =0;
            site.y +=LCD_CH_H;
        }
        if(site.y >(LCD_H-LCD_CH_W))
        {
             /*���¹���*/
             site.y =0;
             site.x =0;
        }
        LCD_Char_CH(site,str,Color,bkColor);
        str += 2 ;
        site.x += LCD_CH_W ;	
    }
}


void LCD_Str_ENCH(Site_t site,const uint8 *str  , uint16 Color ,uint16 bkColor) 		//��ʾ16*16�����ַ���
{
    while(*str != '\0')
    {
        if( *str < 0x80)       //Ӣ��
        {
            if(site.x>(LCD_W-LCD_EN_W))
            {	
                 /*����*/
                site.x =0;
                site.y +=LCD_EN_H;
            }
            if(site.y >(LCD_H-LCD_EN_H))
            {
                 /*���¹���*/
                 site.y =0;
                 site.x =0;
            }
            LCD_char(site,*str,Color,bkColor);
            str += 1 ;
            site.x += LCD_EN_W ;	
        }
        else
        {
            if(site.x>(LCD_W-LCD_CH_W))
            {	
                 /*����*/
                site.x =0;
                site.y +=LCD_CH_H;
            }
            if(site.y >(LCD_H-LCD_CH_H))
            {
                 /*���¹���*/
                 site.y =0;
                 site.x =0;
            }
            LCD_Char_CH(site,str,Color,bkColor);
            str += 2 ;
            site.x += LCD_CH_W ;	
        }
    }
}

void LCD_FChar_CH (Site_t site,const uint8 *str,uint16 Color,uint16 bkColor)
{
    uint8 i,j;
    //uint8 *pbuf = str;
    uint8 tmp_char=0;

    const Size_t size   = {LCD_CH_W,LCD_CH_H};

    LCD_PTLON(site, size);              //����
    LCD_RAMWR();                        //д�ڴ�

    for (i=0;i < LCD_CH_SIZE;i++)
    {
        tmp_char = *str++;

        for(j=0;j<8;j++)
        {
            if(tmp_char & (0x80))
            {
                LCD_WR_DATA( Color );
            }
            else
            {
                LCD_WR_DATA( bkColor );
            }
            tmp_char <<= 1;
        }
    }
}

void LCD_FStr_CH (Site_t site,const uint8 *str,uint16 num,uint16 Color,uint16 bkColor)
{
    while(num--)
    {
        if(site.x>(LCD_W-16))
        {
             /*����*/
            site.x =0;
            site.y +=LCD_CH_H;
        }
        if(site.y >(LCD_H-LCD_CH_W))
        {
             /*���¹���*/
             site.y =0;
             site.x =0;
        }
        LCD_FChar_CH(site,str,Color,bkColor);
        str += LCD_CH_SIZE ;
        site.x += LCD_CH_W ;
    }
}

