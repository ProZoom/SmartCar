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
 * @file       MK60_sdhc.c
 * @brief      SDģ�麯��
 * @author     ɽ��Ƽ� & gustavo(brtos����)
 * @version    v5.0
 * @date       2013-09-19
 */


#ifndef _MK60_SDHC_H_
#define _MK60_SDHC_H_ 1u

#define SDHC_CLOCK      (core_clk_khz * 1000)

//SD����ʼ���ݴ����蹤���ڵ���400 kb��s��Ƶ��
#define     SDHC_INIT_BANDRATE      300000                  //����ʱ�Ĳ�����

#define     SDHC_BANDRATE           (24*1000*1000)          //��������ʱ�Ĳ�����

#define     SDHC_BUS_WIDTH          ESDHC_BUS_WIDTH_4BIT    //����Ŀǰ�����֧�� 1bit �� 4bit


#define     SDCARD_BLOCK_SIZE       512                     //SD�����С

#define     SEND_OCR_TIME           100                     //���� CMD55 + ACMD41 ����Ĵ���


/* Error code returned by I/O functions */
#define IO_ERROR        (-1)

typedef enum
{
    ESDHC_IOCTL_INIT,
    ESDHC_IOCTL_SEND_CMD,
    ESDHC_IOCTL_GET_CARD,
    ESDHC_IOCTL_GET_BUS_WIDTH,
    ESDHC_IOCTL_SET_BUS_WIDTH,

} ESDHC_IOCTL_CMD_e;


/* ���������� disk_ioctrl ���� */
/* ESDHC ������ */
typedef enum
{
    ESDHC_IOCTL_OK,             //OK
    ESDHC_INIT_ERR,             //��ʼ��ʧ��
    ESDHC_CMD_ERR,              //�����������
    ESDHC_CMD_TIMEOUT,          //�������ʱ
    ESDHC_CMD_INVALID,          //��Ч����

    ESDHC_PARAM_INVALID,        //������Ч
    ESDHC_BUS_WIDTH_INVALID,    //���߿����Ч

    ESDHC_BUSY,                 //�豸æ
} ESDHC_IOCTL_ERR_e;


/* ESDHC ���߿�� */
typedef enum
{
    //ֵ�Ǹ��ݼĴ��� �����õ�
    ESDHC_BUS_WIDTH_1BIT    = 0,
    ESDHC_BUS_WIDTH_4BIT    = 1,
    ESDHC_BUS_WIDTH_8BIT    = 2,
} ESDHC_BUS_WIDTH_e;

/* ESDHC ������ */
typedef enum
{
    ESDHC_CARD_NONE                      ,//û�п�
    ESDHC_CARD_UNKNOWN                   ,//����ʶ��
    ESDHC_CARD_SD                        ,//SD
    ESDHC_CARD_SDHC                      ,//SDHC
    ESDHC_CARD_SDIO                      ,//SDIO
    ESDHC_CARD_SDCOMBO                   ,
    ESDHC_CARD_SDHCCOMBO                 ,
    ESDHC_CARD_MMC                       ,//MMC
    ESDHC_CARD_CEATA                     ,
} ESDHC_CARD_e;

/* ESDHC �������� */
typedef enum
{
    ESDHC_TYPE_NORMAL                    ,
    ESDHC_TYPE_SUSPEND                   ,
    ESDHC_TYPE_RESUME                    ,
    ESDHC_TYPE_ABORT                     ,
} ESDHC_TYPE_e;

/* ESDHC ���� */
#define ESDHC_CMD0                           (0)
#define ESDHC_CMD1                           (1)
#define ESDHC_CMD2                           (2)
#define ESDHC_CMD3                           (3)
#define ESDHC_CMD4                           (4)
#define ESDHC_CMD5                           (5)
#define ESDHC_CMD6                           (6)
#define ESDHC_CMD7                           (7)
#define ESDHC_CMD8                           (8)
#define ESDHC_CMD9                           (9)
#define ESDHC_CMD10                          (10)
#define ESDHC_CMD11                          (11)
#define ESDHC_CMD12                          (12)
#define ESDHC_CMD13                          (13)
#define ESDHC_CMD15                          (15)
#define ESDHC_CMD16                          (16)
#define ESDHC_CMD17                          (17)
#define ESDHC_CMD18                          (18)
#define ESDHC_CMD20                          (20)
#define ESDHC_CMD24                          (24)
#define ESDHC_CMD25                          (25)
#define ESDHC_CMD26                          (26)
#define ESDHC_CMD27                          (27)
#define ESDHC_CMD28                          (28)
#define ESDHC_CMD29                          (29)
#define ESDHC_CMD30                          (30)
#define ESDHC_CMD32                          (32)
#define ESDHC_CMD33                          (33)
#define ESDHC_CMD34                          (34)
#define ESDHC_CMD35                          (35)
#define ESDHC_CMD36                          (36)
#define ESDHC_CMD37                          (37)
#define ESDHC_CMD38                          (38)
#define ESDHC_CMD39                          (39)
#define ESDHC_CMD40                          (40)
#define ESDHC_CMD42                          (42)
#define ESDHC_CMD52                          (52)
#define ESDHC_CMD53                          (53)
#define ESDHC_CMD55                          (55)
#define ESDHC_CMD56                          (56)
#define ESDHC_CMD60                          (60)
#define ESDHC_CMD61                          (61)

#define ESDHC_ACMD6                          (6)
#define ESDHC_ACMD13                         (13)
#define ESDHC_ACMD22                         (22)
#define ESDHC_ACMD23                         (23)
#define ESDHC_ACMD41                         (41)
#define ESDHC_ACMD42                         (42)
#define ESDHC_ACMD51                         (51)




#define ESDHC_XFERTYP_RSPTYP_NO              (0x00)
#define ESDHC_XFERTYP_RSPTYP_136             (0x01)
#define ESDHC_XFERTYP_RSPTYP_48              (0x02)
#define ESDHC_XFERTYP_RSPTYP_48BUSY          (0x03)

#define ESDHC_XFERTYP_CMDTYP_ABORT           (0x03)

#define ESDHC_PROCTL_EMODE_INVARIANT         (0x02)

#define ESDHC_PROCTL_DTW_1BIT                (0x00)
#define ESDHC_PROCTL_DTW_4BIT                (0x01)
#define ESDHC_PROCTL_DTW_8BIT                (0x10)


typedef struct
{
    uint8  COMMAND;             //����
    uint8  TYPE;                //�������� SDHC_XFERTYP_CMDTYP
    uint8  READ;
    uint8  RES;                 //����
    uint32 ARGUMENT;            //����Ĳ����Ĵ��� (SDHC_CMDARG)
    uint32 BLOCKS;
    uint32 RESPONSE[4];

} ESDHC_CMD_t, *pESDHC_CMD_t;


/*!
 *  @brief      SD Card �ṹ��Ϣ
 */
typedef struct
{
    /* ʵ�ʿ�״̬                                   */
    uint32                CARD;

    /* �ײ����Ӧ��ʱ >= 250 ms                     */
    uint32               SD_TIMEOUT;

    /* ���豸�Ŀ�����                               */
    uint32                NUM_BLOCKS;

    /* ����ַ                                       */
    uint32                ADDRESS;

    /* ������ = ��Ѱַ (SD���ֽ�Ѱַ��SDHC�ǿ�Ѱַ) */
    uint8                 SDHC;

    /* �淶2����߰汾�Ŀ� = ��ͬ��CSD�Ĵ���        */
    uint8                 VERSION2;

    uint8                 RES[2];

} SDCARD_t, *pSDCARD_t;



extern SDCARD_t        SDHC_card;


extern void                 SDHC_init();                                        // SDHC ��ʼ��
extern ESDHC_IOCTL_ERR_e    SDHC_ioctl(ESDHC_IOCTL_CMD_e  cmd, void *param_ptr); // SDHC ��������
extern void                 SDHC_set_baudrate(uint32 baudrate);                 // SDHC����������
extern uint32               SDHC_cmd (pESDHC_CMD_t command);                    // SDHC����cmd����


#define SDHC_is_running()     (0 != (SDHC_PRSSTAT & (SDHC_PRSSTAT_RTA_MASK | SDHC_PRSSTAT_WTA_MASK | SDHC_PRSSTAT_DLA_MASK | SDHC_PRSSTAT_CDIHB_MASK | SDHC_PRSSTAT_CIHB_MASK)))

#define SDHC_STATUS_WAIT(MASK)      while (0 ==  (SDHC_IRQSTAT & (MASK)));
#define SDHC_STATUS_GET(MASK)       (SDHC_IRQSTAT & (MASK))




#endif  //_MK60_SDHC_H_
