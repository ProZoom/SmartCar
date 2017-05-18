/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       VCAN_MMA7455.c
 * @brief      MMA7455��������ʵ��
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-07-9
 */

#include "common.h"
#include "MK60_port.h"
#include "MK60_i2c.h"
#include "VCAN_MMA7455.h"

//�궨����õײ��I2C�ӿ�
#define MMA7455_OPEN(baud)      i2c_init(MMA7455_DEVICE,baud)
#define MMA7455_WR(reg,value)   i2c_write_reg(MMA7455_DEVICE,MMA7455_ADRESS,reg,value)  //mma7455 д�Ĵ���
#define MMA7455_RD(reg)         i2c_read_reg(MMA7455_DEVICE,MMA7455_ADRESS,reg)         //mma7455 ���Ĵ���

/*!
 *  @brief      MMA7455��ʼ�������� 2g ���̲���ģʽ
 *  @since      v5.0
 *  Sample usage:            mma7455_init();    //��ʼ�� MMA7455
 */
void mma7455_init(void)
{
    MMA7455_OPEN(400 * 1000);           //��ʼ��mma7455�ӿڣ����ò����� 400k

    /*MMA���� 2g ���̲���ģʽ*/
    MMA7455_WR(MMA7455_MCTL, 0x05);

    /*DRDY����λ,�ȴ��������*/
    while(!(MMA7455_RD(MMA7455_STATUS) & 0x01));
}

/*!
 *  @brief      MMA7455д�Ĵ���
 *  @param      reg         �Ĵ���
 *  @param      dat         ��Ҫд������ݵļĴ�����ַ
 *  @since      v5.0
 *  Sample usage:       mma7455_write_reg(MMA7455_XOFFL,0);   // д�Ĵ��� MMA7455_XOFFL Ϊ 0
 */
void mma7455_write_reg(uint8 reg, uint8 Data)
{
    MMA7455_WR(reg, Data);
}

/*!
 *  @brief      MMA7455���Ĵ���
 *  @param      reg         �Ĵ���
 *  @param      dat         ��Ҫ��ȡ���ݵļĴ�����ַ
 *  @since      v5.0
 *  Sample usage:       uint8 data = mma7455_read_reg(MMA7455_XOFFL);    // ���Ĵ��� MMA7455_XOFFL
 */
uint8 mma7455_read_reg(uint8 reg)
{
    return MMA7455_RD(reg);
}


