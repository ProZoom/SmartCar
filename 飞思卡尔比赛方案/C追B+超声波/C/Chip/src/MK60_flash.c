/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_flash.c
 * @brief      flash������
 * @author     ɽ��Ƽ�
 * @version    v5.2
 * @date       2014-10-09
 */


//����ͷ�ļ�

#include "MK60_flash.h"

//�궨�� FCCOB Number
#if defined(MK60DZ10)
#define     FCMD        FTFL_FCCOB0         //FTFL ����

#define     FADDR2      FTFL_FCCOB1         //Flash address [23:16]
#define     FADDR1      FTFL_FCCOB2         //Flash address [15:8]
#define     FADDR0      FTFL_FCCOB3         //Flash address [7:0]

#define     FDATA0      FTFL_FCCOB4         //Data Byte 0       //ע��һ�㣬4�ֽ�����FDATA3�����λ��FDATA0�����λ
#define     FDATA1      FTFL_FCCOB5         //Data Byte 1
#define     FDATA2      FTFL_FCCOB6         //Data Byte 2
#define     FDATA3      FTFL_FCCOB7         //Data Byte 3
#define     FDATA4      FTFL_FCCOB8         //Data Byte 4
#define     FDATA5      FTFL_FCCOB9         //Data Byte 5
#define     FDATA6      FTFL_FCCOBA         //Data Byte 6
#define     FDATA7      FTFL_FCCOBB         //Data Byte 7

//FCMD ����
#define     RD1BLK    0x00   // ������Flash
#define     RD1SEC    0x01   // ����������
#define     PGMCHK    0x02   // д����
#define     RDRSRC    0x03   // ��Ŀ������(4�ֽ�)
#define     PGM4      0x06   // д�볤��(4�ֽ�)
#define     ERSBLK    0x08   // ��������Flash
#define     ERSSCR    0x09   // ����Flash����
#define     PGMSEC    0x0B   // д������
#define     RD1ALL    0x40   // �����еĿ�
#define     RDONCE    0x41   // ֻ��һ��
#define     PGMONCE   0x43   // ֻдһ��
#define     ERSALL    0x44   // �������п�
#define     VFYKEY    0x45   // ��֤���ŷ���Կ��
#define     PGMPART   0x80   // д�����
#define     SETRAM    0x81   // �趨FlexRAM����

#elif defined(MK60F15)
#define     FCMD        FTFE_FCCOB0         //FTFL ����

#define     FADDR2      FTFE_FCCOB1         //Flash address [23:16]
#define     FADDR1      FTFE_FCCOB2         //Flash address [15:8]
#define     FADDR0      FTFE_FCCOB3         //Flash address [7:0]
//Must be 64-bit aligned (Flash address [2:0] = 000)  8�ֽڶ���

#define     FDATA0      FTFE_FCCOB4         //Data Byte 0       //ע��һ�㣬4�ֽ�����FDATA3�����λ��FDATA0�����λ
#define     FDATA1      FTFE_FCCOB5         //Data Byte 1
#define     FDATA2      FTFE_FCCOB6         //Data Byte 2
#define     FDATA3      FTFE_FCCOB7         //Data Byte 3
#define     FDATA4      FTFE_FCCOB8         //Data Byte 4
#define     FDATA5      FTFE_FCCOB9         //Data Byte 5
#define     FDATA6      FTFE_FCCOBA         //Data Byte 6
#define     FDATA7      FTFE_FCCOBB         //Data Byte 7

//FCMD ����
#define     RD1BLK    0x00   // ������Flash
#define     RD1SEC    0x01   // ����������
#define     PGMCHK    0x02   // д����
#define     RDRSRC    0x03   // ��Ŀ������(8�ֽ�)
#define     PGM8      0x07   // д�볤��(8�ֽ�)
#define     ERSBLK    0x08   // ��������Flash
#define     ERSSCR    0x09   // ����Flash����
#define     PGMSEC    0x0B   // д������
#define     RD1ALL    0x40   // �����еĿ�
#define     RDONCE    0x41   // ֻ��һ��
#define     PGMONCE   0x43   // ֻдһ��
#define     ERSALL    0x44   // �������п�
#define     VFYKEY    0x45   // ��֤���ŷ���Կ��
#define     PGMPART   0x80   // д�����
#define     SETRAM    0x81   // �趨FlexRAM����

#endif



/*!
 *  @brief      Flash����
 *  @return     ����ִ�н��(1�ɹ���0ʧ��)
 *  @since      v5.0
 */
__RAMFUNC uint8 flash_cmd()
{
    //д FTFL_FSTAT ���� Flash����
#if defined(MK60DZ10)
    FTFL_FSTAT =    (0
                     |  FTFL_FSTAT_CCIF_MASK        // ָ����ɱ�־(д1��0)
                     |  FTFL_FSTAT_RDCOLERR_MASK    // ����ͻ�����־(д1��0)
                     |  FTFL_FSTAT_ACCERR_MASK      // ���ʴ����־λ(д1��0)
                     |  FTFL_FSTAT_FPVIOL_MASK      // �Ƿ����ʱ�����־λ(д1��0)
                    );

    while(!(FTFL_FSTAT & FTFL_FSTAT_CCIF_MASK));    // �ȴ��������

    // �������־
    if( FTFL_FSTAT & (FTFL_FSTAT_ACCERR_MASK | FTFL_FSTAT_RDCOLERR_MASK | FTFL_FSTAT_FPVIOL_MASK | FTFL_FSTAT_MGSTAT0_MASK))
    {
        return 0 ;                                  //ִ���������
    }
    else
    {
        return 1;                                   //ִ������ɹ�
    }

#elif defined(MK60F15)
    FTFE_FSTAT =    (0
                     |  FTFE_FSTAT_CCIF_MASK        // ָ����ɱ�־(д1��0)
                     |  FTFE_FSTAT_RDCOLERR_MASK    // ����ͻ�����־(д1��0)
                     |  FTFE_FSTAT_ACCERR_MASK      // ���ʴ����־λ(д1��0)
                     |  FTFE_FSTAT_FPVIOL_MASK      // �Ƿ����ʱ�����־λ(д1��0)
                    );

    while(!(FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK));    // �ȴ��������

    // �������־
    if( FTFE_FSTAT & (FTFE_FSTAT_ACCERR_MASK | FTFE_FSTAT_RDCOLERR_MASK | FTFE_FSTAT_FPVIOL_MASK | FTFE_FSTAT_MGSTAT0_MASK))
    {
        return 0 ;                                  //ִ���������
    }
    else
    {
        return 1;                                   //ִ������ɹ�
    }

#endif
}


/*!
 *  @brief      ��ʼ��flash
 *  @since      v5.0
 */
__RAMFUNC void flash_init(void)
{
#if defined( MK60DZ10)
    // ���FlashԤ��ȡ������
    FMC_PFB0CR |= FMC_PFB0CR_S_B_INV_MASK;      //
    FMC_PFB1CR |= FMC_PFB0CR_S_B_INV_MASK;

    while(!(FTFL_FSTAT & FTFL_FSTAT_CCIF_MASK));    // �ȴ��������

    FTFL_FSTAT =    (0
                     |  FTFL_FSTAT_CCIF_MASK        // ָ����ɱ�־(д1��0)
                     |  FTFL_FSTAT_RDCOLERR_MASK    // ����ͻ�����־(д1��0)
                     |  FTFL_FSTAT_ACCERR_MASK      // ���ʴ����־λ(д1��0)
                     |  FTFL_FSTAT_FPVIOL_MASK      // �Ƿ����ʱ�����־λ(д1��0)
                    );

#elif defined(MK60F15)
    // ���FlashԤ��ȡ������
    FMC_PFB01CR |= FMC_PFB01CR_S_B_INV_MASK;      //
    //FMC_PFB23CR |= FMC_PFB23CR_S_B_INV_MASK;

    while(!(FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK));    // �ȴ��������

    FTFE_FSTAT =    (0
                     |  FTFE_FSTAT_CCIF_MASK        // ָ����ɱ�־(д1��0)
                     |  FTFE_FSTAT_RDCOLERR_MASK    // ����ͻ�����־(д1��0)
                     |  FTFE_FSTAT_ACCERR_MASK      // ���ʴ����־λ(д1��0)
                     |  FTFE_FSTAT_FPVIOL_MASK      // �Ƿ����ʱ�����־λ(д1��0)
                    );
#endif
    DELAY_MS(10);
}

/*!
 *  @brief      ����ָ��flash����
 *  @param      sector_num    �����ţ�K60N512ʵ��ʹ��0~255��
 *  @return     ִ�н��(1�ɹ���0ʧ��)
 *  @since      v5.0
 *  Sample usage:       flash_erase_sector(127);        //��������127
 */
__RAMFUNC uint8 flash_erase_sector(uint16 sector_num)
{
    uint32 addr = sector_num * FLASH_SECTOR_SIZE;

    // ���ò�������
    FCMD = ERSSCR;

    // ����Ŀ���ַ
    FADDR2 = ((Dtype *)&addr)->B[2];
    FADDR1 = ((Dtype *)&addr)->B[1];
    FADDR0 = ((Dtype *)&addr)->B[0];

    if(flash_cmd() == 0)
    {
        return 0;
    }

    if(sector_num == 0)
    {
#if defined(MK60DZ10)
        return flash_write(sector_num,0x00040C,0xFFFFFFFE);
#elif defined(MK60F15)
        return flash_write(sector_num,0x000408,0xFFFFFFFEFFFFFFFF );
#endif
    }

    return 1;
}


/*!
 *  @brief      д�볤�ֽ����ݵ� flashָ����ַ
 *  @param      sector_num      �����ţ�0 ~ FLASH_SECTOR_NUM��
 *  @param      offset          д�������ڲ�ƫ�Ƶ�ַ��0~2043 �� 4�ı�����
 *  @param      data            ��Ҫд�������
 *  @return     ִ�н��(1�ɹ���0ʧ��)
 *  @since      v5.0
 *  Sample usage:       flash_write(127,0,0xFFFFFFFE);        //����127,ƫ�Ƶ�ַΪ0,д������:0xFFFFFFFE
 */
__RAMFUNC uint8 flash_write(uint16 sector_num, uint16 offset, FLASH_WRITE_TYPE data)
{
    uint32 addr = sector_num * FLASH_SECTOR_SIZE  + offset ;
    uint32 tmpdata;

    ASSERT(offset % FLASH_ALIGN_ADDR == 0);             //ƫ��������Ϊ4�ı���
    //�˴���ʾ���棬���ǰ�ȫ�ġ���
    ASSERT(offset <= FLASH_SECTOR_SIZE);                //������СΪ2K����ƫ�������벻���� 0x800
    //�˴���ʾ���棬���ǰ�ȫ�ġ���

    // ����Ŀ���ַ
    FADDR2 = ((Dtype *)&addr)->B[2];
    FADDR1 = ((Dtype *)&addr)->B[1];
    FADDR0 = ((Dtype *)&addr)->B[0];

    // ���� ��32λ����
    tmpdata = (uint32)data;

    FDATA0 = ((Dtype *)&tmpdata)->B[3];                    // ����д������
    FDATA1 = ((Dtype *)&tmpdata)->B[2];
    FDATA2 = ((Dtype *)&tmpdata)->B[1];
    FDATA3 = ((Dtype *)&tmpdata)->B[0];

#if defined(MK60DZ10)

    // ���ò�������
    FCMD = PGM4;

#elif defined(MK60F15)

    // ���ò�������
    FCMD = PGM8;

    // ���� ��32λ����
    tmpdata = (uint32)(data>>32);

    FDATA4 = ((Dtype *)&tmpdata)->B[3];                 // ����д������
    FDATA5 = ((Dtype *)&tmpdata)->B[2];
    FDATA6 = ((Dtype *)&tmpdata)->B[1];
    FDATA7 = ((Dtype *)&tmpdata)->B[0];
#endif

    if(flash_cmd() == 0)
    {
        return 0;
    }

    return 1;  //�ɹ�ִ��

}


/*!
 *  @brief      д�����ݻ������� flashָ����ַ
 *  @param      sector_num      �����ţ�K60N512ʵ��ʹ��0~255��
 *  @param      offset          д�������ڲ�ƫ�Ƶ�ַ��0~2043 �� 4�ı�����
 *  @param      buf             ��Ҫд������ݻ������׵�ַ
 *  @return     ִ�н��(1�ɹ���0ʧ��)
 *  @since      v5.0
 *  Sample usage:           uint32 buff[10];
                            flash_write_buf(127,0,sizeof(buff),buff);        //����127,ƫ�Ƶ�ַΪ0,д������Դ��ַ:buff,��Ŀsizeof(buff)
 */

__RAMFUNC uint8 flash_write_buf(uint16 sector_num, uint16 offset, uint16 cnt, uint8 *buf)
{
    uint32  size;

    uint32  addr;
    uint32  data;

    ASSERT(offset % FLASH_ALIGN_ADDR == 0);                    //ƫ��������Ϊ4�ı���
    //�˴���ʾ���棬���ǰ�ȫ��(ram��������rom�������𾯸�)
    ASSERT(offset <= FLASH_SECTOR_SIZE );                    //������СΪ2K����ƫ�������벻���� 0x800
    //�˴���ʾ���棬���ǰ�ȫ��(ram��������rom�������𾯸�)

    addr = sector_num * FLASH_SECTOR_SIZE  + offset;      //�����ַ

    // ����д������
#if defined(MK60DZ10)
    FCMD = PGM4;
#elif defined(MK60F15)
    FCMD = PGM8;
#endif

    for(size = 0; size < cnt; size += FLASH_ALIGN_ADDR )
    {

        // ����Ŀ���ַ
        FADDR2 = ((Dtype *)&addr)->B[2];
        FADDR1 = ((Dtype *)&addr)->B[1];
        FADDR0 = ((Dtype *)&addr)->B[0];

        // ���� ��32λ����
        data =  *(uint32 *)buf;

        FDATA0 = ((Dtype *)&data)->B[3];                    // ����д������
        FDATA1 = ((Dtype *)&data)->B[2];
        FDATA2 = ((Dtype *)&data)->B[1];
        FDATA3 = ((Dtype *)&data)->B[0];

#if defined(MK60F15)
        // ���� ��32λ����
        data = *(uint32 *)(buf+4);

        FDATA4 = ((Dtype *)&data)->B[3];                    // ����д������
        FDATA5 = ((Dtype *)&data)->B[2];
        FDATA6 = ((Dtype *)&data)->B[1];
        FDATA7 = ((Dtype *)&data)->B[0];
#endif

        if(flash_cmd() == 0)
        {
            return 0;
        }

        addr += FLASH_ALIGN_ADDR;
        buf += FLASH_ALIGN_ADDR;
    }
    return 1;  //�ɹ�ִ��
}

