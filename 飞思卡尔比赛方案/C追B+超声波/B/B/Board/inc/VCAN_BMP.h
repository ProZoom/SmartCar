#ifndef _VCAN_BMP_H_
#define _VCAN_BMP_H_


#define MAX_BMP_H       240         //BMP ֧����߸߶�
#define MAX_BMP_W       320         //BMP ֧����߿��
#define MAX_BMP_BIT     24          //BMP ֧�����λ��


/*              BMP�ļ�����ɽṹ                      */
//-------------------------------------------------------
//λͼ�ļ�ͷ��bitmap-file header��
//-------------------------------------------------------
//λͼ��Ϣͷ��bitmap-information header��
//-------------------------------------------------------
//��ɫ��/��ɫ�壨color table��
//-------------------------------------------------------
//λͼ���ݣ�bitmap-data��
//-------------------------------------------------------


typedef enum
{
    BI_RGB,             //��ѹ��
    BI_RLE8,            //ÿ������8���ص�RLEѹ�����룬ѹ����ʽ��2�ֽ����(�ظ����ؼ�������ɫ����)��
    BI_RLE4,            //ÿ������4���ص�RLEѹ�����룬ѹ����ʽ��2�ֽ����
    BI_BITFIELDS,       //ÿ�����صı�����ָ�������������
}biCompression_e;

#if defined(__ICCARM__)
#pragma   pack(2)
#endif
typedef struct              //����˫�ֽڶ���(���ߵ��ֽڶ���)
{
    //BMPλͼ�ļ�ͷ
    uint16 bfType;         //ƫ��:0x00,�ļ���־.'BM',����ʶ��BMPλͼ����
    uint32 bfSize;         //ƫ��:0x02,�ļ���С
    uint16 bfReserved1;    //ƫ��:0x06,����
    uint16 bfReserved2;    //ƫ��:0x08,����
    uint32 bfOffBits;      //ƫ��:0x0A,���ļ���ʼ��λͼ����(bitmap data)��ʼ֮��ĵ�ƫ����

    //BMPλͼ��Ϣͷ
    uint32 biSize;         //ƫ��:0x0E,λͼ��Ϣͷ�ֽ�������
    int32  biWidth;        //ƫ��:0x12,ͼ����
    int32  biHeight;       //ƫ��:0x16,ͼ��߶ȣ������һ��������˵��ͼ�������Ǵ�ͼ�����½ǿ�ʼɨ�裬����������Ͻ�
    uint16 biPlanes;       //ƫ��:0x1A,ΪĿ���豸˵��λ��������ֵ�����Ǳ���Ϊ1
    uint16 biBitCount;     //ƫ��:0x1C,˵��������/���أ���ֵΪ1��4��8��16��24����32
    uint32 biCompression;  //ƫ��:0x1E,˵��ͼ������ѹ�������͡���ֵ����Ϊ biCompression_e
    uint32 biSizeImage;    //ƫ��:0x22,˵��ͼ��Ĵ�С�����ֽ�Ϊ��λ������BI_RGB��ʽʱ��������Ϊ0
    uint32 biXPelsPerMeter;//ƫ��:0x26,˵��ˮƽ�ֱ��ʣ�������/�ױ�ʾ,�з�������
    uint32 biYPelsPerMeter;//ƫ��:0x2A,˵����ֱ�ֱ��ʣ�������/�ױ�ʾ,�з�������
    uint32 biClrUsed;      //ƫ��:0x2E,˵��λͼʵ��ʹ�õĲ�ɫ���е���ɫ������
    uint32 biClrImportant; //ƫ��:0x32,˵����ͼ����ʾ����ҪӰ�����ɫ��������Ŀ�������0����ʾ����Ҫ��

}BMP_FILEHEADER;
#if defined(__ICCARM__)
#pragma   pack()
#endif

//��ɫ�� ����ɫ�ṹ
typedef struct tagRGBQUAD {
    uint8    rgbBlue;
    uint8    rgbGreen;
    uint8    rgbRed;
    uint8    rgbReserved;
} RGBQUAD;



int8 SD2LCD_BMP(char *file,Site_t site);        //��SD����ȡbmp�ļ�������ʾ (Ŀǰ֧�� BMP ͼ��� 24λ���ɫ/RGB555/��ɫ)


#endif  //_VCAN_BMP_H_

