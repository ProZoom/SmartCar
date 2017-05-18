/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       misc.c
 * @brief      ɽ��K60 ƽ̨������Ҫ�õ��ĺ�����ʵ��
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-06-26
 */

#include "common.h"
#include "MK60_uart.h"


/*!
 *  @brief      �����ж��������ַ
 *  @param      vtor    �µ��ж��������ַ
 *  @since      v5.0
 *  @author     ��˼������˾
 *  Sample usage:       write_vtor ((uint32)__VECTOR_RAM);  //�µ��ж�������ַ
 */
void write_vtor (int vtor)
{
    ASSERT(vtor % 0x200 == 0);   //Vector Table base offset field. This value must be a multiple of 0x200.

    /* Write the VTOR with the new value */
    SCB->VTOR = vtor;
}



/*!
 *  @brief      �����ж�����������жϷ�����
 *  @since      v5.0
 *  @warning    ֻ���ж�������λ��icfָ����RAM����ʱ���˺�������Ч
 *  Sample usage:       set_vector_handler(UART3_RX_TX_VECTORn , uart3_handler);    //�� uart3_handler ������ӵ��ж�������
 */
void set_vector_handler(VECTORn_t vector , void pfunc_handler(void))
{
    extern uint32 __VECTOR_RAM[];

    ASSERT(SCB->VTOR == (uint32)__VECTOR_RAM);  //���ԣ�����ж��������Ƿ��� RAM ��

    __VECTOR_RAM[vector] = (uint32)pfunc_handler;
}

void vcan_cpy( uint8 *dst, uint8 *src, uint32 count)
{
    uint32 n;
    if(count != 0)
    {
        //printf("-");

        n = (count + 7 ) / 8 ;
        switch (count % 8 )
        {
            do
            {
            case 0 :
                * dst ++ = * src ++ ;
            case 7 :
                * dst ++ = * src ++ ;
            case 6 :
                * dst ++ = * src ++ ;
            case 5 :
                * dst ++ = * src ++ ;
            case 4 :
                * dst ++ = * src ++ ;
            case 3 :
                * dst ++ = * src ++ ;
            case 2 :
                * dst ++ = * src ++ ;
            case 1 :
                * dst ++ = * src ++ ;
            }
            while ( -- n >    0 );
        }
    }
}

