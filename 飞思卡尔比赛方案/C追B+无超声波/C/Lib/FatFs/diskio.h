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
 * @file       diskio.h
 * @brief      FATFS �ײ�ӿ�ʵ�ֺ���
 * @author     ɽ��Ƽ� & gustavo(brtos����)
 * @version    v5.0
 * @date       2013-09-19
 */


#ifndef _DISKIO
#define _DISKIO

#define _READONLY   0   /* 1: Remove write functions */
#define _USE_IOCTL  1   /* 1: Use disk_ioctl fucntion */


#include "common.h"
#include "integer.h"


/* Status of Disk Functions */
typedef uint8   DSTATUS;

/* Results of Disk Functions */
typedef enum
{
    RES_OK = 0,                 /* 0: �ɹ�      */
    RES_ERROR,                  /* 1: R/W ����  */
    RES_WRPRT,                  /* 2: д����    */
    RES_NOTRDY,                 /* 3: δ����    */
    RES_PARERR                  /* 4: ������Ч  */
} DRESULT;


/* Disk Status Bits (DSTATUS) */
#define STA_NOINIT      0x01    /* ����δ��ʼ�� 1��ʾδ��ʼ����0��ʾ�ѳ�ʼ��   */
#define STA_NODISK      0x02    /* ����δ����    */
#define STA_PROTECT     0x04    /* д����        */


/* Generic command (defined for FatFs) */
#define CTRL_SYNC           0   /* ��ˢ���̻��� */
#define GET_SECTOR_COUNT    1   /* ��ȡ���̴�С */
#define GET_SECTOR_SIZE     2   /* ��ȡ������С (for multiple sector size (_MAX_SS >= 1024)) */
#define GET_BLOCK_SIZE      3   /* ��ȡ���С (for only f_mkfs()) */
#define CTRL_ERASE_SECTOR   4   /* ǿ��ɾ��һ������ (for only _USE_ERASE) */

/* Generic command */
#define CTRL_POWER          5   /* Get/Set power status */
#define CTRL_LOCK           6   /* Lock/Unlock media removal */
#define CTRL_EJECT          7   /* Eject media */

/* MMC/SDC specific ioctl command */
#define MMC_GET_TYPE        10  /* Get card type */
#define MMC_GET_CSD         11  /* Get CSD */
#define MMC_GET_CID         12  /* Get CID */
#define MMC_GET_OCR         13  /* Get OCR */
#define MMC_GET_SDSTAT      14  /* Get SD status */

/* ATA/CF specific ioctl command */
#define ATA_GET_REV         20  /* Get F/W revision */
#define ATA_GET_MODEL       21  /* Get model name */
#define ATA_GET_SN          22  /* Get serial number */

/* NAND specific ioctl command */
#define NAND_FORMAT         30  /* Create physical format */


/*--------------��ֲʱ��Ҫ��д�ļ����ײ㺯��--------------*/
extern DSTATUS disk_initialize (uint8);                          //��ʼ�����̣�ֻ֧�ִ���0��
extern DSTATUS disk_status (uint8);                              //���ش���״̬
extern DRESULT disk_read (uint8, uint8 *, uint32, uint8);           //����������
extern DRESULT disk_ioctl (uint8, uint8, void *);                 //���̿���
extern uint32   get_fattime (void);                              //��ȡ��ǰʱ��

#if _READONLY == 0
DRESULT disk_write (uint8, const uint8 *, uint32, uint8);           //д��������
#endif


#endif
