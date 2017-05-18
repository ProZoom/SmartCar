/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       VCAN_KEY.c
 * @brief      KEY��������ʵ��
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-07-10
 */


/*
 * ����ͷ�ļ�
 */
#include "common.h"
#include "MK60_port.h"
#include "MK60_gpio.h"
#include "VCAN_key.h"


/*
 * ���� KEY ��Ŷ�Ӧ�Ĺܽ�
 */
PTXn_e KEY_PTxn[KEY_MAX] = {PTA0, PTA1, PTA2, PTA3,PTD7, PTD13, PTC14, PTC15};


/*!
 *  @brief      ��ʼ��key�˿�(key С�� KEY_MAX ʱ��ʼ�� ��Ӧ�˿ڣ������ʼ��ȫ���˿�)
 *  @param      KEY_e    KEY���
 *  @since      v5.0
 *  Sample usage:       key_init (KEY_U);    //��ʼ�� KEY_U
 */
void    key_init(KEY_e key)
{
    if(key < KEY_MAX)
    {
        gpio_init(KEY_PTxn[key], GPI, 0);
        port_init_NoALT(KEY_PTxn[key], PULLUP);         //���ָ��ò��䣬�����ı�����ѡ��
    }
    else
    {
        key = KEY_MAX;

        //��ʼ��ȫ�� ����
        while(key--)
        {
            gpio_init(KEY_PTxn[key], GPI, 0);
            port_init_NoALT(KEY_PTxn[key], PULLUP);         //���ָ��ò��䣬�����ı�����ѡ��
        }

    }
}

/*!
 *  @brief      ��ȡkey״̬��������ʱ������
 *  @param      KEY_e           KEY���
 *  @return     KEY_STATUS_e    KEY״̬��KEY_DOWN��KEY_DOWN��
 *  @since      v5.0
 *  Sample usage:
                    if(key_get(KEY_U) ==  KEY_DOWN)
                    {
                        printf("\n��������")
                    }
 */
KEY_STATUS_e key_get(KEY_e key)
{
    if(gpio_get(KEY_PTxn[key]) == KEY_DOWN)
    {
        return KEY_DOWN;
    }
    return KEY_UP;
}


/*!
 *  @brief      ���key״̬������ʱ������
 *  @param      KEY_e           KEY���
 *  @return     KEY_STATUS_e    KEY״̬��KEY_DOWN��KEY_DOWN��
 *  @since      v5.0
 *  Sample usage:
                    if(key_check(KEY_U) ==  KEY_DOWN)
                    {
                        printf("\n��������")
                    }
 */
KEY_STATUS_e key_check(KEY_e key)
{
    if(key_get(key) == KEY_DOWN)
    {
        DELAY_MS(10);
        if( key_get(key) == KEY_DOWN)
        {
            return KEY_DOWN;
        }
    }
    return KEY_UP;
}


/*********************  ���´�����ʵ�ְ�����ʱɨ�裬������Ϣ��FIFO  ********************/
/*
 * ���尴����ϢFIFO״̬
 */
typedef enum
{
    KEY_MSG_EMPTY,      //û�а�����Ϣ
    KEY_MSG_NORMAL,     //�������а�����Ϣ��������
    KEY_MSG_FULL,       //������Ϣ��
} key_msg_e;

/*
 * ���尴����ϢFIFO��صı���
 */
KEY_MSG_t           key_msg[KEY_MSG_FIFO_SIZE];             //������ϢFIFO
volatile uint8      key_msg_front = 0, key_msg_rear = 0;    //����FIFO��ָ��
volatile uint8      key_msg_flag = KEY_MSG_EMPTY;           //������ϢFIFO״̬


/*!
 *  @brief      ���Ͱ�����Ϣ��FIFO
 *  @param      KEY_MSG_t       ������Ϣ
 *  @since      v5.0
 *  Sample usage:
                    KEY_MSG_t *keymsg;
                    keymsg.key      = KEY_U;
                    keymsg.status   = KEY_HOLD;
                    send_key_msg(keymsg);                   //����
 */
void send_key_msg(KEY_MSG_t keymsg)
{
    uint8 tmp;
    //������FIFO��
    if(key_msg_flag == KEY_MSG_FULL)
    {
        //����ֱ�Ӳ�����
        return ;
    }
    key_msg[key_msg_rear].key = keymsg.key;
    key_msg[key_msg_rear].status = keymsg.status;

    key_msg_rear++;

    if(key_msg_rear >= KEY_MSG_FIFO_SIZE)
    {
        key_msg_rear = 0;                       //��ͷ��ʼ
    }

    tmp = key_msg_rear;
    if(tmp == key_msg_front)                   //׷��ƨ���ˣ�����
    {
        key_msg_flag = KEY_MSG_FULL;
    }
    else
    {
        key_msg_flag = KEY_MSG_NORMAL;
    }
}


/*!
 *  @brief      ��FIFO���ȡ������Ϣ
 *  @param      KEY_MSG_t       ������Ϣ
 *  @return     �Ƿ��ȡ������Ϣ��1Ϊ��ȡ�ɹ���0Ϊû��ȡ��������Ϣ��
 *  @since      v5.0
 *  Sample usage:
                    KEY_MSG_t keymsg;
                    if(get_key_msg(&keymsg) == 1)
                    {
                        printf("\n���°���KEY%d,����Ϊ%d��0Ϊ���£�1Ϊ����2Ϊ������",keymsg.key,keymsg.status);
                    }
 */
uint8 get_key_msg(KEY_MSG_t *keymsg)
{
    uint8 tmp;

    if(key_msg_flag == KEY_MSG_EMPTY)               //������ϢFIFOΪ�գ�ֱ�ӷ���0
    {
        return 0;
    }

    keymsg->key = key_msg[key_msg_front].key;       //��FIFO�����л�ȡ����ֵ
    keymsg->status = key_msg[key_msg_front].status; //��FIFO�����л�ȡ��������

    key_msg_front++;                                //FIFO����ָ���1��ָ����һ����Ϣ

    if(key_msg_front >= KEY_MSG_FIFO_SIZE)          //FIFOָ�����������0��ʼ����
    {
        key_msg_front = 0;                          //��ͷ��ʼ������ѭ���������飩
    }

    tmp = key_msg_rear;
    if(key_msg_front == tmp)                        //�Ƚ϶��׺Ͷ�β�Ƿ�һ����һ�����ʾFIFO�ѿ���
    {
        key_msg_flag = KEY_MSG_EMPTY;
    }
    else
    {
        key_msg_flag = KEY_MSG_NORMAL;
    }

    return 1;
}

/*!
 *  @brief      ��ʱ���key״̬
 *  @since      v5.0
 *  @note       �˺�����Ҫ���� ��ʱ�жϷ��������ʱ10msִ��һ��
 */
void key_IRQHandler(void)
{

    KEY_e   keynum;
    static uint8 keytime[KEY_MAX];                          //��̬���飬��������鰴��ʱ��

    KEY_MSG_t keymsg;                                       //������Ϣ

    for(keynum = (KEY_e)0 ; keynum < KEY_MAX; keynum ++)    //ÿ��������ѯ
    {
        if(key_get(keynum) == KEY_DOWN)                     //�жϰ����Ƿ���
        {
            keytime[keynum]++;                              //����ʱ���ۼ�

            if(keytime[keynum] <= KEY_DOWN_TIME)            //�ж�ʱ���Ƿ�û��������ȷ�ϰ���ʱ��
            {
                continue;                                   //û�ﵽ��������ȴ�
            }
            else if(keytime[keynum] == KEY_DOWN_TIME + 1 )  //�ж�ʱ���Ƿ�Ϊ����ȷ�ϰ���ʱ��
            {
                //ȷ�ϰ�������
                keymsg.key = keynum;
                keymsg.status = KEY_DOWN;
                send_key_msg(keymsg);                       //�Ѱ���ֵ�Ͱ������ͷ�����Ϣ��FIFO
            }
            else if(keytime[keynum] <= KEY_HOLD_TIME)       //�Ƿ�û��������HOLD����ȷ��ʱ��
            {
                continue;                                   //û������������ȴ�
            }
            else if(keytime[keynum]  == KEY_HOLD_TIME + 1)  //�Ƿ�Ϊ����holdȷ��ʱ��
            {
                //ȷ�ϳ���HOLD
                keymsg.key = keynum;
                keymsg.status = KEY_HOLD;
                send_key_msg(keymsg);                       //����
                keytime[keynum] = KEY_DOWN_TIME + 1;
            }
            else
            {
                keytime[keynum] = KEY_DOWN_TIME + 1;        //�����ظ���� hold ״̬
            }
        }
        else
        {
            if(keytime[keynum] > KEY_DOWN_TIME)             //���ȷ�Ϲ����°���
            {
                keymsg.key = keynum;
                keymsg.status = KEY_UP;
                send_key_msg(keymsg);                       //���Ͱ���������Ϣ
            }

            keytime[keynum] = 0;                            //ʱ���ۼ���0
        }
    }
}



