/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_pit.c
 * @brief      pit��ʱ��������
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-08-23
 */
#include "common.h"
#include  "MK60_PIT.h"     //�����жϼ�ʱ��

/*!
 *  @brief      PITn��ʱ�ж�
 *  @param      PITn        ģ��ţ�PIT0~PIT3��
 *  @param      cnt         ��ʱ�ж�ʱ��(��λΪbusʱ������)
 *  @since      v5.0
 *  Sample usage:
                pit_init(PIT0, 1000);                          //��ʱ 1000 ��busʱ�� ���ж�
                set_vector_handler(PIT0_VECTORn,pit_hander);   // �����жϷ��������ж���������
                enable_irq(PIT0_IRQn);                         // ʹ��PIT�ж�
 */
void pit_init(PITn_e pitn, uint32 cnt)
{
    //PIT �õ��� Bus Clock ����Ƶ��
    //������� = ����Ƶ�� * ʱ��

    ASSERT( cnt > 0 );                          //�ö��Լ�� ʱ����벻��Ϊ 0

    SIM_SCGC6       |= SIM_SCGC6_PIT_MASK;          //ʹ��PITʱ��

    PIT_MCR         = (0
                       //| PIT_MCR_MDIS_MASK       //����PIT��ʱ��ʱ��ѡ��0��ʾʹ��PIT��1��ʾ��ֹPIT��
                       //| PIT_MCR_FRZ_MASK        //����ģʽ��ֹͣ���У�0��ʾ�������У�1��ʾֹͣ���У�
                      );

    PIT_LDVAL(pitn)  = cnt - 1 ;                    //��������ж�ʱ��

    PIT_Flag_Clear(pitn);                           //���жϱ�־λ

    PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;        //��ֹPITn��ʱ����������ռ���ֵ��
    PIT_TCTRL(pitn)  = ( 0
                         | PIT_TCTRL_TEN_MASK        //ʹ�� PITn��ʱ��
                         | PIT_TCTRL_TIE_MASK        //��PITn�ж�
                       );

    //enable_irq((int)pitn + PIT0_IRQn);            //���ж�
}

/*!
 *  @brief      PITn��ʱ
 *  @param      PITn        ģ��ţ�PIT0~PIT3��
 *  @param      cnt         ��ʱʱ��(��λΪbusʱ������)
 *  @since      v5.0
 *  Sample usage:
                    pit_delay(PIT0, 1000);                         //��ʱ 1000 ��busʱ��
 */
void pit_delay(PITn_e pitn, uint32 cnt)
{
    //PIT �õ��� Bus Clock ����Ƶ��
    //������� = ����Ƶ�� * ʱ��

    ASSERT( cnt > 0 );              //�ö��Լ�� ʱ����벻��Ϊ 0

    SIM_SCGC6       |= SIM_SCGC6_PIT_MASK;                          //ʹ��PITʱ��

    PIT_MCR         &= ~(PIT_MCR_MDIS_MASK | PIT_MCR_FRZ_MASK );    //ʹ��PIT��ʱ��ʱ�� ������ģʽ�¼�������

    PIT_TCTRL(pitn) &= ~( PIT_TCTRL_TEN_MASK );                     //����PIT ���Ա����ü���ֵ��Ч

    PIT_LDVAL(pitn)  = cnt - 1;                                     //��������ж�ʱ��

    PIT_Flag_Clear(pitn);                                           //���жϱ�־λ

    PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;                        //��ֹPITn��ʱ����������ռ���ֵ��
    PIT_TCTRL(pitn)  = ( 0
                         | PIT_TCTRL_TEN_MASK                        //ʹ�� PITn��ʱ��
                         //| PIT_TCTRL_TIE_MASK                      //��PITn�ж�
                       );

    while( !(PIT_TFLG(pitn)& PIT_TFLG_TIF_MASK));

    PIT_Flag_Clear(pitn);                                           //���жϱ�־λ
}

/*!
 *  @brief      PITn��ʱ��ʼ
 *  @param      PITn        ģ��ţ�PIT0~PIT3��
 *  @since      v5.0
 *  Sample usage:
                    pit_time_start(PIT0);                          //PIT0��ʱ��ʼ
 */
void pit_time_start(PITn_e pitn)
{
    //PIT �õ��� Bus Clock ����Ƶ��
    //������� = ����Ƶ�� * ʱ��

    SIM_SCGC6       |= SIM_SCGC6_PIT_MASK;                          //ʹ��PITʱ��

    PIT_MCR         &= ~(PIT_MCR_MDIS_MASK | PIT_MCR_FRZ_MASK );    //ʹ��PIT��ʱ��ʱ�� ������ģʽ�¼�������

    PIT_TCTRL(pitn) &= ~( PIT_TCTRL_TEN_MASK );                     //����PIT ���Ա����ü���ֵ��Ч

    PIT_LDVAL(pitn)  = ~0;                                          //��������ж�ʱ��

    PIT_Flag_Clear(pitn);                                           //���жϱ�־λ

    PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;                        //��ֹPITn��ʱ����������ռ���ֵ��
    PIT_TCTRL(pitn)  = ( 0
                         | PIT_TCTRL_TEN_MASK                        //ʹ�� PITn��ʱ��
                         //| PIT_TCTRL_TIE_MASK                      //��PITn�ж�
                       );
}

/*!
 *  @brief      ��ȡ PITn��ʱʱ��(��ʱʱ��ر� ��ʱ��)
 *  @param      PITn        ģ��ţ�PIT0~PIT3��
 *  @since      v5.0
 *  Sample usage:
                        uint32 time = pit_time_get(PIT0);                         //��ȡ PITn��ʱʱ��
                        if(time != ~0)       //û��ʱ
                        {
                            printf("\n��ʱʱ��Ϊ��%d us",time*1000/bus_clk_khz);
                        }
 */
uint32 pit_time_get(PITn_e pitn)
{
    uint32 val;

    val = (~0) - PIT_CVAL(pitn);

    if(PIT_TFLG(pitn)& PIT_TFLG_TIF_MASK)                           //�ж��Ƿ�ʱ�䳬ʱ
    {
        PIT_Flag_Clear(pitn);                                       //���жϱ�־λ
        PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;                    //��ֹPITn��ʱ����������ռ���ֵ��
        return ~0;
    }

    if(val == (~0))
    {
        val--;              //ȷ�� ������ ~0
    }
    return val;
}

/*!
 *  @brief      �ر� pit
 *  @param      PITn        ģ��ţ�PIT0~PIT3��
 *  @since      v5.0
 *  Sample usage:
                        pit_close(PIT0);                         //�ر�PIT
 */
void pit_close(PITn_e pitn)
{
    PIT_Flag_Clear(pitn);                                       //���жϱ�־λ
    PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;                    //��ֹPITn��ʱ����������ռ���ֵ��
}