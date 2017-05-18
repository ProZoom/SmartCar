#ifndef _VCAN_OV7725_WOLF_H_
#define _VCAN_OV7725_WOLF_H_

#include "VCAN_OV7725_REG.h"
#if ( USE_CAMERA == CAMERA_OV7725_WOLF )
//��������ͷ������
#define CAMERA_USE_HREF     0               //�Ƿ�ʹ�� ���ж� (0 Ϊ ��ʹ�ã�1Ϊʹ��)
#define CAMERA_COLOR        1               //����ͷ�����ɫ �� 0 Ϊ �ڰ׶�ֵ��ͼ�� ��1 Ϊ �Ҷ� ͼ�� ��2 Ϊ RGB565 ͼ��
#define CAMERA_POWER        0               //����ͷ ��Դѡ�� 0 Ϊ 3.3V ,1 Ϊ 5V

//��������ͷ����ӿ�
#define camera_init(imgaddr)    ov7725_wolf_init(imgaddr)
#define camera_get_img()        ov7725_wolf_get_img()

#define camera_vsync()          ov7725_wolf_vsync()
#define camera_href()           //ov7725_wolf_href()
#define camera_dma()            ov7725_wolf_dma()

//���� ����ͷ ����
#define CAMERA_DMA_CH       OV7725_WOLF_DMA_CH                          //��������ͷ��DMA�ɼ�ͨ��
#define CAMERA_W            OV7725_WOLF_W                               //��������ͷͼ����
#define CAMERA_H            OV7725_WOLF_H                               //��������ͷͼ��߶�

#define CAMERA_SIZE         OV7725_WOLF_SIZE                            //ͼ��ռ�ÿռ��С
#define CAMERA_DMA_NUM      OV7725_WOLF_DMA_NUM                         //DMA�ɼ�����

#endif  //#if ( USE_CAMERA == CAMERA_OV7725_WOLF )  

//��������ͷ ����
#define OV7725_WOLF_DMA_CH       DMA_CH0                                //��������ͷ��DMA�ɼ�ͨ��
#define OV7725_WOLF_W            80                                     //��������ͷͼ���ȣ��û������޸ģ�
#define OV7725_WOLF_H            60                                     //��������ͷͼ��߶ȣ��û������޸ģ�
#define OV7725_WOLF_SIZE         (OV7725_WOLF_W * OV7725_WOLF_H )       //ͼ��ռ�ÿռ��С
#define OV7725_WOLF_DMA_NUM      (OV7725_WOLF_SIZE )                    //DMA�ɼ�����


extern  uint8   ov7725_wolf_init(uint8 *imgaddr);
extern  void    ov7725_wolf_get_img(void);

extern  void    ov7725_wolf_vsync(void);
extern  void    ov7725_wolf_dma(void);


#endif  //_VCAN_OV7725_WOLF_H_


