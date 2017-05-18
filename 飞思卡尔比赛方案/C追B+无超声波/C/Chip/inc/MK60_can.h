/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_can.h
 * @brief      CAN����
 * @author     ɽ��Ƽ�
 * @version    v5.1
 * @date       2014-04-25
 */

#ifndef _MK60_CAN_H_
#define _MK60_CAN_H_

/*! CAN��� */
typedef enum
{
    CAN0,
    CAN1,

} CANn_e;

/*! MB��������� */
typedef enum
{
    MB_NUM_0,       //��������0
    MB_NUM_1,
    MB_NUM_2,
    MB_NUM_3,
    MB_NUM_4,
    MB_NUM_5,
    MB_NUM_6,
    MB_NUM_7,
    MB_NUM_8,
    MB_NUM_9,
    MB_NUM_10,
    MB_NUM_11,
    MB_NUM_12,
    MB_NUM_13,
    MB_NUM_14,
    MB_NUM_15,

    NUMBER_OF_MB,
    MB_NUM_MAX = NUMBER_OF_MB,

} mb_num_e;

/*! CAN����ģʽ */
typedef enum
{
    CAN_NORMAL,         //����ģʽ
    CAN_LOOPBACK,       //����ģʽ
} CAN_mode_e;


/*! CAN�����ʵļĴ������ýṹ�� */
typedef struct
{
    uint32_t band;          //�����ʣ�Kb/s��
    uint16_t presdiv;       //��Ƶϵ��
    uint8_t  prop_seg;      //����ʱ���
    uint8_t  pseg1;         //��λ�����1
    uint8_t  pseg2;         //��λ�����2
    uint8_t  rjw;           //ͬ����ת���

    uint8_t  res[2];        //����

} CAN_band_cfg_t;

/*! CAN�����ʱ�� */
typedef enum        //���޸Ĵ˴���ֵ���򻹱����޸� can_band_cfg ����
{
    CAN_BAUD_10K    ,
    CAN_BAUD_20K    ,
    CAN_BAUD_50K    ,
    CAN_BAUD_100K   ,
    CAN_BAUD_125K   ,
    CAN_BAUD_250K   ,
    CAN_BAUD_500K   ,
    CAN_BAUD_1M     ,

    CAN_BAUD_MAX,

} CAN_BAUD_e;

/*! CANʱ��Դѡ�� */
typedef enum
{
    CAN_CLKSRC_BUS,   //BUS����
    CAN_CLKSRC_OSC,   //�ⲿ����
} CAN_CLKSRC_e;


/*! �û��Զ��� CAN ID */
typedef struct
{
    uint32  ID: 29;     //ID
    uint32  IDE: 1;     //IDE   // 1 : ��չID, 0: ��׼ID
    uint32  RTR: 1;     //RTR   // 1: Զ��֡, 0: ����֡
} CAN_USR_ID_t;


extern void    can_init        (CANn_e cann, CAN_BAUD_e band, CAN_mode_e mode,CAN_CLKSRC_e clksrc);    //CAN ��ʼ��
extern void    can_tx          (CANn_e cann, mb_num_e nMB, CAN_USR_ID_t id, uint8 len, void *buff);    //CAN ��������
extern void    can_rx          (CANn_e cann, mb_num_e nMB, CAN_USR_ID_t *id, uint8 *len, void *buff);  //CAN ��������

extern void    can_rxbuff_enble(CANn_e cann, mb_num_e nMB, CAN_USR_ID_t id);                           //ʹ�� CAN ���ջ�����
extern void    can_rxbuff_mask (CANn_e cann, mb_num_e nMB, uint32 mask,uint8 isIRMQ);                  //CAN �����������ã�ѡ��

extern void    can_irq_en      (CANn_e cann, mb_num_e nMB);                                            //ʹ�� CAN ���������պͷ����ж�
extern void    can_irq_dis     (CANn_e cann, mb_num_e nMB);                                            //��ֹ CAN ���������պͷ����ж�

extern void    can_clear_flag  (CANn_e cann, mb_num_e nMB);                                            //�� CAN �������жϱ�־
extern uint32  can_get_flag    (CANn_e cann, mb_num_e nMB);                                            //��� CAN �������жϱ�־

extern void    can_setband     (CANn_e cann, CAN_BAUD_e band);                                         //���� CAN �Ĳ�����


#endif

