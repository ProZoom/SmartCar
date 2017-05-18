/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       VCAN_camera.h
 * @brief      ����ͷ�����ӿ��ض���
 * @author     ɽ��Ƽ�
 * @version    v5.2.1
 * @date       2015-04-01
 */


#ifndef _VCAN_CAMERA_H_
#define _VCAN_CAMERA_H_


#define CAMERA_OV7725_EAGLE         2       //ɽ��ӥ��
#define CAMERA_OV7725_WOLF          3       //ɽ������


#define USE_CAMERA      CAMERA_OV7725_EAGLE   //ѡ��ʹ�õ� ����ͷ

typedef struct
{
    uint8 addr;                 /*�Ĵ�����ַ*/
    uint8 val;                   /*�Ĵ���ֵ*/
} reg_s;

//����ͼ��ɼ�״̬
typedef enum
{
    IMG_NOTINIT = 0,
    IMG_FINISH,             //ͼ��ɼ����
    IMG_FAIL,               //ͼ��ɼ�ʧ��(�ɼ���������)
    IMG_GATHER,             //ͼ��ɼ���
    IMG_START,              //��ʼ�ɼ�ͼ��
    IMG_STOP,               //��ֹͼ��ɼ�
} IMG_STATUS_e;



#include  "VCAN_SCCB.h"
#include  "VCAN_OV7725_Eagle.h"

extern void img_extract(void *dst, void *src, uint32_t srclen);


// ����ͷ �ӿ�ͳһ�ĳ� ����ģʽ

//  camera_init(imgaddr);
//  camera_get_img();
//  camera_cfg(rag,val)


//  camera_vsync()  //���ж�
//  camera_href()   //���ж�
//  camera_dma()    //DMA�ж�

// ��Ҫ �ṩ ���� �궨��
// #define  CAMERA_USE_HREF    1     //�Ƿ�ʹ�� ���ж� (0 Ϊ ��ʹ�ã�1Ϊʹ��)
// #define  CAMERA_COLOR       1     //����ͷ�����ɫ �� 0 Ϊ �ڰ׶�ֵ��ͼ�� ��1 Ϊ �Ҷ� ͼ�� ��2 Ϊ RGB565 ͼ��
// #define  CAMERA_POWER       0     //����ͷ ��Դѡ�� 0 Ϊ 3.3V ,1 Ϊ 5V



#endif


