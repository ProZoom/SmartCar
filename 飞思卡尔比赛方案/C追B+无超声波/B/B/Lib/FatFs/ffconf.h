/*---------------------------------------------------------------------------/
/  FatFs - FAT file system module configuration file  R0.09  (C)ChaN, 2011
/----------------------------------------------------------------------------/
/
/ CAUTION! Do not forget to make clean the project after any changes to
/ the configuration options.
/
/----------------------------------------------------------------------------*/
#ifndef _FFCONF
#define _FFCONF 6502    /* Revision ID */


/*---------------------------------------------------------------------------/
/ Functions and Buffer Configurations
/----------------------------------------------------------------------------*/

#define _FS_TINY        0   /* 0:Normal or 1:Tiny */
/* When _FS_TINY is set to 1, FatFs uses the sector buffer in the file system
/  object instead of the sector buffer in the individual file object for file
/  data transfer. This reduces memory consumption 512 bytes each file object. */
// �ļ�ϵͳΪ��׼�Ļ���΢�͵ģ�Ĭ��Ϊ��׼��(0)��

#define _FS_READONLY    0   /* 0:Read/Write or 1:Read only */
// �ļ�ϵͳ�Ƿ�Ϊֻ����Ĭ��Ϊ�ɶ�д(0)����ֻ����f_write��f_sync�� f_unlink��
// f_mkdir��f_chmod��f_rename��f_truncate��f_getfree�����ã�


#define _FS_MINIMIZE    0   /* 0 to 3 */
//�ü��ļ�ϵͳ�Ĺ��ܣ�Ĭ��Ϊȫ������(0)����Ϊ1��2����Ƴ��󲿷����ӡ�Ŀ¼�ȹ��ܣ�
/* The _FS_MINIMIZE option defines minimization level to remove some functions.
/
/   0: Full function.
/   1: f_stat, f_getfree, f_unlink, f_mkdir, f_chmod, f_truncate and f_rename
/      are removed.
/   2: f_opendir and f_readdir are removed in addition to 1.
/   3: f_lseek is removed in addition to 2. */


#define _USE_STRFUNC    2   /* 0:Disable or 1-2:Enable */
/* To enable string functions, set _USE_STRFUNC to 1 or 2. */
//�Ƿ������ַ���������Ĭ��Ϊ������(0)���������������һ�����������Ϊ1���ɣ�
//���������windows�£�Ϊ��֤�ļ�������(�绻�з���\n���ͻس�����\r��)���齫��������Ϊ2��

#define _USE_MKFS       0   /* 0:Disable or 1:Enable */
/* To enable f_mkfs function, set _USE_MKFS to 1 and set _FS_READONLY to 0 */
//�Ƿ�����ʹ��f_mkfs������Ĭ��Ϊ0�����ڴ����ļ��У����鿪����

#define _USE_FORWARD    0   /* 0:Disable or 1:Enable */
/* To enable f_forward function, set _USE_FORWARD to 1 and set _FS_TINY to 1. */
//��������f_forward������ֻ�п���tiny�ļ�ϵͳʱ���õ����ú������ڽ���д����������
//ת�浽�������У��Խ�ʡRAM�ռ䣻


#define _USE_FASTSEEK   0   /* 0:Disable or 1:Enable */
/* To enable fast seek feature, set _USE_FASTSEEK to 1. */
//�Ƿ�������������Ĭ��Ϊ0�������󣬻�ʹ��FIL�ṹ���е�cltblԪ�����ӿ�������



/*---------------------------------------------------------------------------/
/ Locale and Namespace Configurations
/----------------------------------------------------------------------------*/

#define _CODE_PAGE  936
//ָ��Ŀ��ϵͳʹ�õ�OEM����ҳ��Ĭ��Ϊ����(932)����Ϊ936�������ģ�
//OEM��ʲô��˼�أ���OS�����У�unicode��һ��˫�ֽ��ַ����룬�������Ļ���Ӣ�ģ�
//������������ͳһ��2���ֽڣ��������е��κα��루ASCII��GB�ȣ��������ݡ�
//WindowsNT(2000)���ں˼�ʹ�øñ��룬�������ݽ����ں�ǰת����UNICODE��
//�˳��ں˺���ת���ɰ汾��صı��루ͨ����ΪOEM���ڼ������İ��¼�ΪGB)��

/* The _CODE_PAGE specifies the OEM code page to be used on the target system.
/  Incorrect setting of the code page can cause a file open failure.
/
/   932  - Japanese Shift-JIS (DBCS, OEM, Windows)
/   936  - Simplified Chinese GBK (DBCS, OEM, Windows)   ��������
/   949  - Korean (DBCS, OEM, Windows)
/   950  - Traditional Chinese Big5 (DBCS, OEM, Windows)
/   1250 - Central Europe (Windows)
/   1251 - Cyrillic (Windows)
/   1252 - Latin 1 (Windows)
/   1253 - Greek (Windows)
/   1254 - Turkish (Windows)
/   1255 - Hebrew (Windows)
/   1256 - Arabic (Windows)
/   1257 - Baltic (Windows)
/   1258 - Vietnam (OEM, Windows)
/   437  - U.S. (OEM)
/   720  - Arabic (OEM)
/   737  - Greek (OEM)
/   775  - Baltic (OEM)
/   850  - Multilingual Latin 1 (OEM)
/   858  - Multilingual Latin 1 + Euro (OEM)
/   852  - Latin 2 (OEM)
/   855  - Cyrillic (OEM)
/   866  - Russian (OEM)
/   857  - Turkish (OEM)
/   862  - Hebrew (OEM)
/   874  - Thai (OEM, Windows)
/   1    - ASCII only (Valid for non LFN cfg.)
*/


#define _USE_LFN    0       /* 0 to 3 */  //LFN ��Long File Name ������
#define _MAX_LFN    255     /* Maximum LFN length to handle (12 to 255) */
/* The _USE_LFN option switches the LFN support.
/
/   0: Disable LFN feature. _MAX_LFN and _LFN_UNICODE have no effect.
/   1: Enable LFN with static working buffer on the BSS. Always NOT reentrant.
/   2: Enable LFN with dynamic working buffer on the STACK.
/   3: Enable LFN with dynamic working buffer on the HEAP.
/
/  The LFN working buffer occupies (_MAX_LFN + 1) * 2 bytes. To enable LFN,
/  Unicode handling functions ff_convert() and ff_wtoupper() must be added
/  to the project. When enable to use heap, memory control functions
/  ff_memalloc() and ff_memfree() must be added to the project. */


#define _LFN_UNICODE    0   /* 0:ANSI/OEM or 1:Unicode */
/* To switch the character code set on FatFs API to Unicode,
/  enable LFN feature and set _LFN_UNICODE to 1. */


#define _FS_RPATH       0   /* 0 to 2 */
//�Ƿ��������·��������ѡ��Ͳ������������߼���ø��Ӳ�˵��������Ҳ�����һЩ��
/* The _FS_RPATH option configures relative path feature.
/
/   0: Disable relative path feature and remove related functions.
/   1: Enable relative path. f_chdrive() and f_chdir() are available.
/   2: f_getcwd() is available in addition to 1.
/
/  Note that output of the f_readdir fnction is affected by this option. */



/*---------------------------------------------------------------------------/
/ Physical Drive Configurations
/----------------------------------------------------------------------------*/

#define _VOLUMES    1
//����(flash)�߼�������Ĭ��Ϊ1���������޸ģ�
/* Number of volumes (logical drives) to be used. */


#define _MAX_SS     512     /* 512, 1024, 2048 or 4096 */
// ������С��Ĭ��512Byte����������4096Byte��
/* Maximum sector size to be handled.
/  Always set 512 for memory card and hard disk but a larger value may be
/  required for on-board flash memory, floppy disk and optical disk.
/  When _MAX_SS is larger than 512, it configures FatFs to variable sector size
/  and GET_SECTOR_SIZE command must be implememted to the disk_ioctl function. */


#define _MULTI_PARTITION    0   /* 0:Single partition, 1/2:Enable multiple partition */
//����ѡ�Ĭ��Ϊ0����һ������������Ҫ��������������ã�
/* When set to 0, each volume is bound to the same physical drive number and
/ it can mount only first primaly partition. When it is set to 1, each volume
/ is tied to the partitions listed in VolToPart[]. */


#define _USE_ERASE  0   /* 0:Disable or 1:Enable */
//�Ƿ���������������Ĭ��Ϊ0����������Ҫ��disk_ioctl��������Ӳ���������룻
/* To enable sector erase feature, set _USE_ERASE to 1. CTRL_ERASE_SECTOR command
/  should be added to the disk_ioctl functio. */



/*---------------------------------------------------------------------------/
/ System Configurations
/----------------------------------------------------------------------------*/

#define _WORD_ACCESS    0   /* 0 or 1 */
//���ݵݽ���ʽ��Ĭ��Ϊ0�������ֽ�Ϊ��λ�ݽ��������Ը�ǿ��
//�����ϵͳ���µ�λΪ��(2Byte)�������Ϊ1��
/* Set 0 first and it is always compatible with all platforms. The _WORD_ACCESS
/  option defines which access method is used to the word data on the FAT volume.
/
/   0: Byte-by-byte access.
/   1: Word access. Do not choose this unless following condition is met.
/
/  When the byte order on the memory is big-endian or address miss-aligned word
/  access results incorrect behavior, the _WORD_ACCESS must be set to 0.
/  If it is not the case, the value can also be set to 1 to improve the
/  performance and code size.
*/


/* A header file that defines sync object types on the O/S, such as
/  windows.h, ucos_ii.h and semphr.h, must be included prior to ff.h. */

#define _FS_REENTRANT   0       /* 0:Disable or 1:Enable */
#define _FS_TIMEOUT     1000    /* Timeout period in unit of time ticks */
#define _SYNC_t         HANDLE  /* O/S dependent type of sync object. e.g. HANDLE, OS_EVENT*, ID and etc.. */
//������ѡ�����ļ�ϵͳ�Ƿ����������йأ�˵ֱ�׵㣬�����ܷ񱻶��߳�ͬʱ���ʣ�
//��RTOS�У�һ�㽨�鿪����_SYNC_t�ɶ���Ϊ��ӦOS�еĲ�������windows��ΪHANDLE��
//uCos��ΪOS_EVENT��vxWorks��ΪSEMAPHORE�����⣬��������Ҫ���ff_req_grant��
//ff_rel_grant��ff_del_syncobj����������ʵ����ʵ�ֵĹ��ܾ������뻥������
//�ͷŻ�������ɾ������������˼�����Զ���OS��װ���ɣ�

/* The _FS_REENTRANT option switches the reentrancy (thread safe) of the FatFs module.
/
/   0: Disable reentrancy. _SYNC_t and _FS_TIMEOUT have no effect.
/   1: Enable reentrancy. Also user provided synchronization handlers,
/      ff_req_grant, ff_rel_grant, ff_del_syncobj and ff_cre_syncobj
/      function must be added to the project. */


#define _FS_SHARE   0   /* 0:Disable or >=1:Enable */
/* To enable file shareing feature, set _FS_SHARE to 1 or greater. The value
   defines how many files can be opened simultaneously. */
// ����������ƣ���ʾ�ļ�ϵͳ�������ͬʱ�򿪶����ļ���Ĭ��Ϊ0����ֻ�ܴ�һ����

#endif /* _FFCONFIG */
