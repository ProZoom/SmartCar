#ifndef ORDERFORMAT_H
#define ORDERFORMAT_H
/*
ͨ��Э������
    PC�˷��ͣ�
        ������  :  4
        ����ʽ  :  ��ͷ	������	������H	������L
        ����
                ��λ�����ͣ�setValue(NUM1,1234);
                ��λ���յ���SEND_HEAD NUM1 12 34

                void InfSwap::setValue(char index,double value)
                {
                        while(readWriteFlag == true);
                        readWriteFlag = true;

                        if(value < 0)
                        {
                                index |= 0x80;
                                value = 0 - value;
                        }

                        if(((int)value)*100%100 != 0)
                        {
                                index |= 0x40;
                                value *= 100;
                        }

                        order[0] = SEND_HEAD;
                        order[1] = index;
                        order[2] = ((int)value)/100%100 +1 ;
                        order[3] = (((int)value)%100);

                        myCom->writeData((char *)order,4);
                        readWriteFlag = false;
                }


                ע�����
                        1.����Hλ��+1����λ��Ӧ�ö�����Hλ�Լ�1
                        2.�������8λΪ1����ֵΪ����
                        3.�������7λΪ1��������HλΪ�������֣�����LλΪС������
    PC������:
        ������  :  4
        ����ʽ  :  ��ͷ	������	������H	������L
                ��λ�����ͣ�fDebug(SEND1,1234)
                ��λ���յ���SEND_HEAD SEND1 12 34*/




#define SEND_HEAD       0xfe
#define SEND_EMPTY      0x00



//car send
#define NUM1            0x01
#define NUM2            0x02
#define NUM3            0x03
#define NUM4            0x04
#define NUM5            0x05
#define NUM6            0x06
#define NUM7            0x07
#define NUM8            0x08
#define NUM9            0x09
#define NUM10           0x0a
#define NUM11           0x0b
#define NUM12           0x0c
#define NUM13           0x0d
#define NUM14           0x0e
#define NUM15           0x0f

#define ROAD_LEFT       0x10
#define ROAD_RIGHT      0x11
#define ROAD_HEAD       0x12

#define REAL_SPEED      0x1d	//ʵ���ٶ�
#define WANT_SPEED      0x1e    //Ŀ���ٶ�
#define SERVO           0x1f    //���ֵ

//wave
#define P1_WAVE1        0x20    //ͨ��1 ����1
#define P1_WAVE2        0x21
#define P1_WAVE3        0x22
#define P1_WAVE4        0x23
#define P1_WAVE5        0x24
#define P1_WAVE6        0x25
#define P1_WAVE7        0x26
#define P1_WAVE8        0x27
#define P2_WAVE1        0x28    //ͨ��2 ����1
#define P2_WAVE2        0x29
#define P2_WAVE3        0x2a
#define P2_WAVE4        0x2b
#define P2_WAVE5        0x2c
#define P2_WAVE6        0x2d
#define P2_WAVE7        0x2e
#define P2_WAVE8        0x2f

#define P1_CLEAN        0x30
#define P2_CLEAN        0x31

//host send
#define SEND1           0x01
#define SEND2           0x02
#define SEND3           0x03
#define SEND4           0x04
#define SEND5           0x05
#define SEND6           0x06
#define SEND7           0x07
#define SEND8           0x08
#define SEND9           0x09
#define SPEED_CT_E      0x0a    //�ٶȿ���ʹ��
#define SPEED_CT        0x0b    //�ٶȿ���
#define START_CAR       0x0c    //����
#define STOP_CAR        0x0d    //ͣ��

#define ORDER_MAX       0x3f


#endif // ORDERFORMAT_H
