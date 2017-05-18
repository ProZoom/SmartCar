/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 *     ��������brtos������ȡ���޸Ķ���:http://code.google.com/p/brtos/downloads/detail?name=BRTOS%201.66%20Kinetis%20-%20SD%20card%20-%20BMP%2024%20bit.rar&can=2&q=
 *
 * @file       diskio.c
 * @brief      FATFS �ײ�ӿ�ʵ�ֺ���
 * @author     ɽ��Ƽ� & gustavo(brtos����)
 * @version    v5.0
 * @date       2013-09-19
 */

#include "common.h"
#include "MK60_sdhc.h"
#include "diskio.h"


static volatile DSTATUS   Stat             = STA_NOINIT;    /* Ӳ��״̬         */

/*!
 *  @brief      Ӳ�̳�ʼ��
 *  @param      drv                 �豸��(Ŀǰ�����֧��Ϊ 0)
 *  @since      v5.0
 */
DSTATUS disk_initialize (unsigned char drv)
{
    uint32                      param, c_size, c_size_mult, read_bl_len;
    ESDHC_CMD_t                 command;

    if (drv) return STA_NOINIT;         /* Ŀǰ�����֧��1���豸 */
    if (Stat & STA_NODISK) return Stat; /* û�в��뿨 */
    if ((Stat & STA_NOINIT) == 0)   return 0;/* û�г�ʼ�� */

    SDHC_card.SD_TIMEOUT = 0;
    SDHC_card.NUM_BLOCKS = 0;
    SDHC_card.ADDRESS = 0;
    SDHC_card.SDHC = FALSE;
    SDHC_card.VERSION2 = FALSE;

    /* ��ʼ���ͼ�⿨ */
    if (ESDHC_IOCTL_OK != SDHC_ioctl (ESDHC_IOCTL_INIT, NULL))
    {
        return FALSE;
    }

    /* SDHC ��� */
    param = 0;
    if (ESDHC_IOCTL_OK != SDHC_ioctl (ESDHC_IOCTL_GET_CARD, &param))
    {
        return FALSE;
    }
    if ((ESDHC_CARD_SD == param) || (ESDHC_CARD_SDHC == param) || (ESDHC_CARD_SDCOMBO == param) || (ESDHC_CARD_SDHCCOMBO == param))
    {
        if ((ESDHC_CARD_SDHC == param) || (ESDHC_CARD_SDHCCOMBO == param))
        {
            SDHC_card.SDHC = TRUE;
        }
    }
    else
    {
        return FALSE;
    }

    /* ��ʶ�� */
    command.COMMAND = ESDHC_CMD2;
    command.TYPE = ESDHC_TYPE_NORMAL;
    command.ARGUMENT = 0;
    command.READ = FALSE;
    command.BLOCKS = 0;
    if (ESDHC_IOCTL_OK != SDHC_ioctl (ESDHC_IOCTL_SEND_CMD, &command))
    {
        return FALSE;
    }

    /* ��ÿ���ַ */
    command.COMMAND = ESDHC_CMD3;
    command.TYPE = ESDHC_TYPE_NORMAL;
    command.ARGUMENT = 0;
    command.READ = FALSE;
    command.BLOCKS = 0;
    if (ESDHC_IOCTL_OK != SDHC_ioctl (ESDHC_IOCTL_SEND_CMD, &command))
    {
        return FALSE;
    }
    SDHC_card.ADDRESS = command.RESPONSE[0] & 0xFFFF0000;

    /* ��ÿ����� */
    command.COMMAND = ESDHC_CMD9;
    command.TYPE = ESDHC_TYPE_NORMAL;
    command.ARGUMENT = SDHC_card.ADDRESS;
    command.READ = FALSE;
    command.BLOCKS = 0;
    if (ESDHC_IOCTL_OK != SDHC_ioctl (ESDHC_IOCTL_SEND_CMD, &command))
    {
        return FALSE;
    }
    if (0 == (command.RESPONSE[3] & 0x00C00000))
    {
        read_bl_len = (command.RESPONSE[2] >> 8) & 0x0F;
        c_size = command.RESPONSE[2] & 0x03;
        c_size = (c_size << 10) | (command.RESPONSE[1] >> 22);
        c_size_mult = (command.RESPONSE[1] >> 7) & 0x07;
        SDHC_card.NUM_BLOCKS = (c_size + 1) * (1 << (c_size_mult + 2)) * (1 << (read_bl_len - 9));
    }
    else
    {
        SDHC_card.VERSION2 = TRUE;
        c_size = (command.RESPONSE[1] >> 8) & 0x003FFFFF;
        SDHC_card.NUM_BLOCKS = (c_size + 1) << 10;
    }

    /* ѡ�� */
    command.COMMAND = ESDHC_CMD7;
    command.TYPE = ESDHC_TYPE_NORMAL;
    command.ARGUMENT = SDHC_card.ADDRESS;
    command.READ = FALSE;
    command.BLOCKS = 0;
    if (ESDHC_IOCTL_OK != SDHC_ioctl (ESDHC_IOCTL_SEND_CMD, &command))
    {
        return FALSE;
    }

    /* ���ÿ�Ĵ�С */
    command.COMMAND = ESDHC_CMD16;
    command.TYPE = ESDHC_TYPE_NORMAL;
    command.ARGUMENT = SDCARD_BLOCK_SIZE;
    command.READ = FALSE;
    command.BLOCKS = 0;
    if (ESDHC_IOCTL_OK != SDHC_ioctl (ESDHC_IOCTL_SEND_CMD, &command))
    {
        return FALSE;
    }

    if (ESDHC_BUS_WIDTH_4BIT == SDHC_BUS_WIDTH)
    {
        /* Ӧ�ó����ض������� */
        command.COMMAND = ESDHC_CMD55;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = SDHC_card.ADDRESS;
        command.READ = FALSE;
        command.BLOCKS = 0;
        if (ESDHC_IOCTL_OK != SDHC_ioctl (ESDHC_IOCTL_SEND_CMD, &command))
        {
            return FALSE;
        }

        /* �������ߴ��� == 4 */
        command.COMMAND = ESDHC_ACMD6;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = 2;
        command.READ = FALSE;
        command.BLOCKS = 0;
        if (ESDHC_IOCTL_OK != SDHC_ioctl (ESDHC_IOCTL_SEND_CMD, &command))
        {
            return FALSE;
        }

        param = ESDHC_BUS_WIDTH_4BIT;
        if (ESDHC_IOCTL_OK != SDHC_ioctl (ESDHC_IOCTL_SET_BUS_WIDTH, &param))
        {
            return FALSE;
        }
    }

    Stat &= ~STA_NOINIT;        /* �� STA_NOINIT */

    return (Stat & 0x03);
}

/*!
 *  @brief      ��MMC�������ݰ�
 *  @param      buff                ���� SDCARD_BLOCK_SIZE ���ֽڵ����ݿ鵽���ݻ�������ַ
 *  @param      btr                 �ֽ���Ŀ(������ 4 �ı���)
 *  @since      v5.0
 */
static int rcvr_datablock (uint8   *buff, uint32  btr)
{
    uint32  bytes, i, j;
    uint32  *ptr = (uint32 *)buff;


    /* Workaround for random bit polling failures - not suitable for IO cards    */

    while(!(SDHC_IRQSTAT & SDHC_IRQSTAT_BRR_MASK));
    SDHC_IRQSTAT =  SDHC_IRQSTAT_BRR_MASK ;

    /* Read data in 4 byte counts */
    bytes = btr;
    while (bytes)
    {
        i = bytes > SDCARD_BLOCK_SIZE ? SDCARD_BLOCK_SIZE : bytes;
        for (j = (i + 3) >> 2; j != 0; j--)
        {
            if (SDHC_IRQSTAT & (SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK))
            {
                SDHC_IRQSTAT |= SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK | SDHC_IRQSTAT_BRR_MASK;
                return 0;
            }

            while (0 == (SDHC_PRSSTAT & SDHC_PRSSTAT_BREN_MASK)) {};

            //С��
            *ptr++ = SDHC_DATPORT;
        }
        bytes -= i;
    }

    return 1;                       /* Return with success */
}

/*!
 *  @brief      ������
 *  @param      drv                 ������(Ŀǰ�����֧��Ϊ 0)
 *  @param      buff                ��������ַ
 *  @param      sector              ������
 *  @param      count               ������(1~255)
 *  @return     DRESULT             ִ�н��
 *  @since      v5.0
 */
DRESULT disk_read (         //����������
    uint8  drv,             /* ����������� (0) */
    uint8  *buff,           /* ָ�����ݻ��������洢���������� */
    uint32 sector,          /* ��ʼ�������� (LBA) */
    uint8  count            /* ��������(1..255) */
)
{
    ESDHC_CMD_t command;

    if (drv || (!count)) return RES_PARERR;       //drv ֻ��Ϊ 0�� count ���벻����0
    if (Stat & STA_NOINIT) return RES_NOTRDY;     //δ����

    /* ������ */
    if ((NULL == buff))
    {
        return RES_PARERR;//������Ч
    }

    if (!SDHC_card.SDHC)
    {
        sector *= SDCARD_BLOCK_SIZE;    /* �����Ҫ,ת��Ϊ�ֽڵ�ַ */
    }

    if (count == 1) /* ����� */
    {
        command.COMMAND = ESDHC_CMD17;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = sector;
        command.READ = TRUE;
        command.BLOCKS = count;

        if (ESDHC_IOCTL_OK == SDHC_ioctl (ESDHC_IOCTL_SEND_CMD, &command))
        {
            if (rcvr_datablock(buff, SDCARD_BLOCK_SIZE))
            {
                count = 0;
            }
        }
    }
    else
    {
        /* ���� */
        //
        command.COMMAND = ESDHC_CMD18;
        //command.COMMAND = ESDHC_CMD17;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = sector;
        command.READ = TRUE;
        command.BLOCKS = count;

        if (ESDHC_IOCTL_OK == SDHC_ioctl (ESDHC_IOCTL_SEND_CMD, &command))
        {
            if (rcvr_datablock(buff, SDCARD_BLOCK_SIZE * count))
            {
                count = 0;
            }
        }
    }

    return count ? RES_ERROR : RES_OK;
}

/*!
 *  @brief      �������ݰ��� MMC
 *  @param      buff                ���� SDCARD_BLOCK_SIZE ���ֽڵ����ݿ�����ݻ�������ַ
 *  @param      btr                 �ֽ���Ŀ(������ 4 �ı���)
 *  @since      v5.0
 */
static int xmit_datablock (const uint8 *buff, uint32 btr    )
{
    uint32  bytes, i;
    uint32  *ptr = (uint32 *)buff;

    /* Write data in 4 byte counts */
    bytes = btr;
    while (bytes)
    {
        i = bytes > SDCARD_BLOCK_SIZE ? SDCARD_BLOCK_SIZE : bytes;
        bytes -= i;
        for (i = (i + 3) >> 2; i != 0; i--)
        {
            if (SDHC_IRQSTAT & (SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK))
            {
                SDHC_IRQSTAT |= SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK | SDHC_IRQSTAT_BWR_MASK;
                return IO_ERROR;
            }
            while (0 == (SDHC_PRSSTAT & SDHC_PRSSTAT_BWEN_MASK)) {};

            SDHC_DATPORT = *ptr++;
        }
    }

    return 1;
}



/*!
 *  @brief      д����
 *  @param      drv                 ������(Ŀǰ�����֧��Ϊ 0)
 *  @param      buff                ��������ַ
 *  @param      sector              ������
 *  @param      count               ������(1~255)
 *  @return     DRESULT             ִ�н��
 *  @since      v5.0
 */
DRESULT disk_write (uint8  drv, const uint8  *buff, uint32 sector, uint8  count)
{
    ESDHC_CMD_t command;
    //pSDCARD_t    sdcard_ptr = (pSDCARD_t)&SDHC_card;

    if (drv || !count) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;
    if (Stat & STA_PROTECT) return RES_WRPRT;

    /* Check parameters */
    if ((NULL == buff))
    {
        return RES_PARERR;      // ������Ч
    }

    if (!SDHC_card.SDHC)
    {
        sector *= SDCARD_BLOCK_SIZE;    /* Convert to byte address if needed */
    }

    if (count == 1) /* Single block write */
    {
        command.COMMAND = ESDHC_CMD24;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = sector;
        command.READ = FALSE;
        command.BLOCKS = count;

        if (ESDHC_IOCTL_OK == SDHC_ioctl (ESDHC_IOCTL_SEND_CMD, &command))
        {
            if (xmit_datablock(buff, SDCARD_BLOCK_SIZE))
            {
                count = 0;
            }
        }
    }
    else
    {
        command.COMMAND = ESDHC_CMD25;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = sector;
        command.READ = FALSE;
        command.BLOCKS = count;

        if (ESDHC_IOCTL_OK == SDHC_ioctl (ESDHC_IOCTL_SEND_CMD, &command))
        {
            if (xmit_datablock(buff, SDCARD_BLOCK_SIZE * count))
            {
                count = 0;
            }
            while((SDHC_IRQSTAT & SDHC_IRQSTAT_TC_MASK)==0);

            if (SDHC_IRQSTAT & (SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK))
            {
                SDHC_IRQSTAT |= SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK;
            }
            SDHC_IRQSTAT |= SDHC_IRQSTAT_TC_MASK | SDHC_IRQSTAT_BRR_MASK | SDHC_IRQSTAT_BWR_MASK;


        }
    }

    /* Wait for card ready / transaction state */
    do
    {
        command.COMMAND = ESDHC_CMD13;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = SDHC_card.ADDRESS;
        command.READ = FALSE;
        command.BLOCKS = 0;
        if (ESDHC_IOCTL_OK != SDHC_ioctl (ESDHC_IOCTL_SEND_CMD, &command))
        {
            return RES_ERROR;
        }

        /* Card status error check */
        if (command.RESPONSE[0] & 0xFFD98008)
        {
            return RES_ERROR;
        }
    }
    while (0x000000900 != (command.RESPONSE[0] & 0x00001F00));

    return count ? RES_ERROR : RES_OK;
}


/*!
 *  @brief      Ӳ�� ��������
 *  @param      drv                 ������
 *  @param      ctrl                ��������
 *  @param      buff                ��������ַ�����ڽ��պͷ��Ϳ�������
 *  @since      v5.0
 */
DRESULT disk_ioctl (
    uint8 drv,      /* Physical drive nmuber (0) */
    uint8 ctrl,     /* Control code */
    void  *buff     /* Buffer to send/receive control data */
)
{
    DRESULT              res;
    ESDHC_CMD_t command;
    //pSDCARD_t  sdcard_ptr = (pSDCARD_t)&SDHC_card;

    if (drv) return RES_PARERR;

    res = RES_ERROR;

    if (Stat & STA_NOINIT) return RES_NOTRDY;

    switch (ctrl)
    {
    case CTRL_SYNC :        /* Make sure that no pending write process. Do not remove this or written sector might not left updated. */
        res = RES_OK;
        break;

    case GET_SECTOR_COUNT : /* Get number of sectors on the disk (uint32) */
        *(unsigned long *)buff = SDHC_card.NUM_BLOCKS;
        res = RES_OK;
        break;

    case GET_SECTOR_SIZE :  /* Get R/W sector size (WORD) */
        *(unsigned short *)buff = SDCARD_BLOCK_SIZE;
        res = RES_OK;
        break;

    case GET_BLOCK_SIZE :           /* Get erase block size in unit of sector (uint32) */
        // Implementar
        command.COMMAND = ESDHC_CMD9;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = SDHC_card.ADDRESS;
        command.READ = FALSE;
        command.BLOCKS = 0;
        if (ESDHC_IOCTL_OK != SDHC_ioctl (ESDHC_IOCTL_SEND_CMD, &command))
        {
            return RES_ERROR;
        }
        if (0 == (command.RESPONSE[3] & 0x00C00000))
        {
            //SD V1
            *(unsigned long *)buff = ((((command.RESPONSE[2] >> 18) & 0x7F) + 1) << (((command.RESPONSE[3] >> 8) & 0x03) - 1));
        }
        else
        {
            //SD V2
            // Implementar
            //*(uint32*)buff = (((command.RESPONSE[2] >> 18) & 0x7F) << (((command.RESPONSE[3] >> 8) & 0x03) - 1));
        }
        res = RES_OK;
        break;

    default:
        res = RES_PARERR;
    }

    return res;
}


/*!
 *  @brief      ��ȡӲ��״̬
 *  @return     Ӳ��״̬
 *  @since      v5.0
 */
DSTATUS disk_status (
    uint8 drv       /* Physical drive nmuber (0) */
)
{
    if (drv) return STA_NOINIT;     /* Supports only single drive */
    return Stat;
}


/*!
 *  @brief      ��ȡʱ��(Ϊ������ӿ���Ҫ����ӣ�ʵ���ϲ�û��ʵ�ֹ���)
 *  @return     �������Ϊ 0
 *  @since      v5.0
 */
uint32  get_fattime (void)
{
    return   0;
}


