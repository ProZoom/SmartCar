/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       VCAN_UI_VAR.h
 * @brief      UI ��������
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2014-01-04
 */

#ifndef _VCAN_UI_VAR_H_
#define _VCAN_UI_VAR_H_


#define VAR_SELECT_HOLD_OFFSET      ((VAR_MAX+4-1)/4)       //���Ͽ���ʱ��ÿ���л�ƫ������Ŀǰ����Ϊ����������1/4.���޸ĳ�������
#define VAR_VALUE_HOLE_OFFSET       10

#define UI_VAR_USE_LCD              1                       //ͬ���ź�ʱ��Ҫ�����Ƿ�ʹ��LCD��ʾ���ݣ�1Ϊ��ʾ��0Ϊ����ʾ��


typedef struct
{
    uint32  val;            //Ŀǰ��ֵ
    uint32  oldval;         //ͨ������£���������ͬ���޸ĺ�û��ȷ�ϼ�������ͬ����ȷ�ϼ����ͺ�����ͬ
                            //������͵�ֵ
    uint32  minval;         //��Сֵ
    uint32  maxval;         //���ֵ
    Site_t  site;           //LCD ��ʾ������λ��
} ui_var_info_t;            //������Ϣ

//�����ķ��������
typedef enum
{
    //�����ı��

    /* 8λ���� */
    CAR_CTRL,
    VAR1,
    VAR2,
    VAR_8BIT = VAR2, //8λ�����Ľ������

    /* 16λ���� */
    VAR3,
    VAR4,
    VAR_16BIT  = VAR4,          //16λ�����Ľ������

    /* 32λ���� */
    VAR5,
    VAR6,
    VAR_32BIT  = VAR6,          //32λ�����Ľ������

    VAR_MAX,        //������Ŀ
} var_tab_e;

typedef enum
{
    VAR_NEXT,           //��һ��
    VAR_PREV,           //��һ��
    VAR_NEXT_HOLD,      //���£�ƫ��Ϊ��VAR_SELECT_HOLD_OFFSET
    VAR_PREV_HOLD,      //���ϣ�ƫ��Ϊ��VAR_SELECT_HOLD_OFFSET

    VAR_ADD,            //��1
    VAR_SUB,            //��1
    VAR_ADD_HOLD,       //��ӣ�ƫ��Ϊ��VAR_VALUE_HOLE_OFFSET
    VAR_SUB_HOLD,       //�����ƫ��Ϊ��VAR_VALUE_HOLE_OFFSET

    VAR_OK,             //ȷ��
    VAR_CANCEL,         //ȡ��

    VAR_EVENT_MAX,
} ui_var_event_e;

typedef enum
{
    CAR_NULL,           //������

    //ͼ��ķ���
    CAR_IMG_CLOSE,      //�ر�С��ͼ����(��������ͷͼ�������CCD�ȴ���������)
    CAR_IMG_OPEN,       //����С��ͼ����

    //һ��ͣ������
    CAR_START,          //����
    CAR_STOP,           //ͣ��

    //ң��С��
    CAR_FORWARD,        //ǰ��
    CAR_BACK,           //����

    CAR_LEFT,           //��ת
    CAR_RIGHT,          //��ת

    CAR_FAST,           //����
    CAR_SLOW,           //����





    //�û����� �˴����� һЩ�Զ��幦��
    //�����ӹ��ܣ���ر�֤ ���պͷ������˶��� ʹ����ͬ�� ���ƹ��ܱ��


    CAR_CTRL_MAX,

}CAR_CTRL_MODE_e;


extern uint32   last_tab;           //�����յ��ı������

void var_init();

//���� �� ��ֵ �� ��ֵ
extern void     save_var(var_tab_e var_tal, uint32 var_data);       //�����ű�����ֵ
extern void     get_var(var_tab_e var_tal, uint32 *var_data);       //��ȡ��ű�����ֵ (���ݱ�������ҵ�������ַ���Ӷ���ȡ������ֵ)
extern void     updata_var(var_tab_e var_tal);                      //���±�ű�����ֵ���޸ı�����ֵ����Ҫ���ô˺��������±�ű�����ֵ��

extern void     var_display(uint8 tab);                             //��ʾ��ű�����ֵ
extern uint8    var_syn(uint8 tab);                                 //ͬ��ָ����ֵ��tab Ϊ VAR_NUM ʱ��ʾȫ��ͬ����С����ͬ����Ӧ�ı�ű���


//���� �� ѡ�� �� �Ӽ� ����
extern void     var_select(ui_var_event_e  ctrl);   //ѡ���л�����
extern void     var_value(ui_var_event_e ctrl);     //�ı������ֵ
extern void     var_ok();                           //ȷ�ϵ�ǰѡ���
extern void     val_cancel();                       //ȡ����ǰѡ���ֵ  OK


extern uint8    car_ctrl_get();                     //��ȡС����������
extern uint8    car_ctrl_syn(CAR_CTRL_MODE_e mode); //����С����������



#endif  //_VCAN_UI_VAR_H_

