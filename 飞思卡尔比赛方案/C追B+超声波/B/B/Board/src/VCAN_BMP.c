//֧��24bit BMPͼƬ��ʾ
#include "common.h"
#include "VCAN_BMP.h"
#include "ff.h"
#include "vcan_lcd.h"


#define BMP_SIZE(W,H)           ((((W)*4+3)/4)*(H))         //((w * 4)+3)/4  �� Ϊ�� �� 1 ����Ϊ bmp ��� �� 4 �� ����
#define BMP_BYTECNT(W,H,BIT)    (BMP_SIZE(W,H)*BIT/8)

#define MAX_BMP_SIZE            BMP_SIZE(MAX_BMP_W,MAX_BMP_H)
#define MAX_BMP_BYTECNT         BMP_BYTECNT(MAX_BMP_W,MAX_BMP_H,MAX_BMP_BIT)

#define BMP_LINE_BYTE(W,BIT)    ((((W)*(BIT)+31)/32)*4)
#define MAX_BMP_LINE_BYTE       BMP_LINE_BYTE(MAX_BMP_W,MAX_BMP_BIT)



//BMPһ��ͼ��
uint8 BMP_Buffer[MAX_BMP_LINE_BYTE];


int8 SD2LCD_BMP(char *file,Site_t site0)
{
    //�ļ�ϵͳ��ص� ����
    FATFS fs={0};
    FIL fsrc={0};   
    FRESULT res;
    uint32 br;    

    
    BMP_FILEHEADER BmpHead;

    int Width,Height ;
    uint32 OffBits,Bitcnt ;



    uint8 r,g,b ;
    uint16 rgb;

    Size_t size;
    Site_t site;

    int x=0,y=0,BytesPerLine,Step ;

    uint8 *pColor;


    f_mount(0,&fs);

    //���ļ�
    res=f_open(&fsrc,file,FA_OPEN_EXISTING | FA_READ);
    if(res)
    {
        goto bmp_exit_false;
    }

    //��ȡBMP ͷ���ļ�
    res=f_read(&fsrc,&BmpHead,sizeof(BmpHead),&br);
    if(res)
    {
        goto bmp_exit_false;
    }

    Width       = BmpHead.biWidth;
    Height      = BmpHead.biHeight;
    OffBits     = BmpHead.bfOffBits;        //bmp��ɫ������ʼλ��
    Bitcnt      = BmpHead.biBitCount;

    if((BmpHead.bfType != 0x4d42)||(BmpHead.biCompression))           //'BM' ��  ûѹ��
    {
        DEBUG_PRINTF("\r\n��֧�ָ�ͼƬ��ʾ��");
        goto bmp_exit_false;
    }

    if(Width > MAX_BMP_W)                   //ͼƬ��Ȳ��ܳ���MAX_BMP_W
    {
        DEBUG_PRINTF("\r\nͼƬ̫���޷���ʾ��");
        goto bmp_exit_false;
    }

    if(Height>0)                            //����߶��� ������ͼ������½����϶�����
    {
        y = Height -1;
        Step = -1;
    }
    else if(Height < 0)                     //����߶��� ������ͼ������Ͻ����¶�����
    {
        Height = -Height;
        y = 0;
        Step = 1;
    }
    else
    {
        goto bmp_exit_false;
    }

    //һ��һ�ж�ȡ
    size.H = 1;
    size.W = Width;
    site.x = site0.x;
    BytesPerLine = BMP_LINE_BYTE(Width,Bitcnt);       //һ�е���Ŀ

    switch(Bitcnt)
    {
        case 24:        //24λ���ɫ :BGR24
        {
            //��ʼ��ͼ
            res=f_lseek(&fsrc,OffBits); //�ҵ�bmp��ɫ������ʼλ��

            for(;(y<Height)&&(y>=0);y+=Step)
            {
                res=f_read(&fsrc,BMP_Buffer,BytesPerLine,&br);
                if(res) goto bmp_exit_false;
                pColor = BMP_Buffer;

                //���ڵ���Һ������ӿ�Ч�ʲ��ߣ�����ֱ�ӵ��õײ�
                site.y = site0.y +y;
                LCD_PTLON(site, size);              //����
                LCD_RAMWR();                        //д�ڴ�

                for(x=0;x<Width;x++)
                {
                    //BGR24
                    b=*pColor++;
                    g=*pColor++;
                    r=*pColor++;

                    rgb = RGB24_RGB565(r,g,b);
                    LCD_WR_DATA( rgb);
                }
            }
            break;

        }

        //16λ�У���͵�5λ��ʾ��ɫ�������м��5λ��ʾ��ɫ�������ߵ�5λ��ʾ��ɫ������һ��ռ����15λ����ߵ�һλ��������Ϊ0�����ָ�ʽҲ������555 16λλͼ��
        //����� RBG565 ,��ô biCompression = BI_BITFIELDS ��
        case 16:        //16λ�߲�ɫ
        {
            //��ʼ��ͼ
            res=f_lseek(&fsrc,OffBits); //�ҵ�bmp��ɫ������ʼλ��

            for(;(y<Height)&&(y>=0);y+=Step)
            {
                res=f_read(&fsrc,BMP_Buffer,BytesPerLine,&br);
                if(res)goto bmp_exit_false;
                pColor = BMP_Buffer;

                //���ڵ���Һ������ӿ�Ч�ʲ��ߣ�����ֱ�ӵ��õײ�
                site.y = site0.y +y;
                LCD_PTLON(site, size);              //����
                LCD_RAMWR();                        //д�ڴ�

                for(x=0;x<Width;x++)
                {
                    rgb = *(uint16 *)pColor;    //bgr555
                    rgb = RGB555_RGB565(rgb);

                    LCD_WR_DATA( rgb);

                    pColor+=2;
                }
            }
            break;
        }

        //��ɫBMP���ļ���ͷ������2��RGBQUAD�ĵ�ɫ����ɫ����ʾ 0 �� 1 �ֱ��Ӧ����ɫ��
        //һ���� 0Ϊ�� �� 1 Ϊ ��
        case 1:         //��ɫ
        {
            //�����ļ�ͷ�����϶� ���԰�
            res=f_read(&fsrc,BMP_Buffer,2*sizeof(RGBQUAD),&br);
            uint16 color[2];
            color[0] = RGB24_RGB565(BMP_Buffer[2],BMP_Buffer[1],BMP_Buffer[0]);
            color[1] = RGB24_RGB565(BMP_Buffer[6],BMP_Buffer[5],BMP_Buffer[4]);

            //��ʼ��ͼ
            res=f_lseek(&fsrc,OffBits);         //�ҵ�bmp��ɫ������ʼλ��

            Width = (Width+7)/8;                //8������һ����(+7�ǽ�1)

            for(;(y<Height)&&(y>=0);y+=Step)
            {
                res=f_read(&fsrc,BMP_Buffer,BytesPerLine,&br);
                if(res)goto bmp_exit_false;
                pColor = BMP_Buffer;

                //���ڵ���Һ������ӿ�Ч�ʲ��ߣ�����ֱ�ӵ��õײ�
                site.y = site0.y +y;
                LCD_PTLON(site, size);              //����
                LCD_RAMWR();                        //д�ڴ�

                for(x=0;x<Width;x++)
                {
                    rgb = color[(*pColor >> 7)&0x01];
                    LCD_WR_DATA( rgb);
                    rgb = color[(*pColor >> 6)&0x01];
                    LCD_WR_DATA( rgb);
                    rgb = color[(*pColor >> 5)&0x01];
                    LCD_WR_DATA( rgb);
                    rgb = color[(*pColor >> 4)&0x01];
                    LCD_WR_DATA( rgb);
                    rgb = color[(*pColor >> 3)&0x01];
                    LCD_WR_DATA( rgb);
                    rgb = color[(*pColor >> 2)&0x01];
                    LCD_WR_DATA( rgb);
                    rgb = color[(*pColor >> 1)&0x01];
                    LCD_WR_DATA( rgb);
                    rgb = color[(*pColor >> 0)&0x01];
                    LCD_WR_DATA( rgb);

                    pColor+=1;
                }
            }
    
            break;

        }
        
        default:
        DEBUG_PRINTF("\r\n��֧�ָ�ͼƬ��ʾ��");
        goto bmp_exit_false;

    }

    f_close(&fsrc);
    f_mount(0,NULL);

    return TRUE;

bmp_exit_false:
    f_close(&fsrc);
    f_mount(0,NULL);

    return FALSE;
}
/*************************** End of file ****************************/
