/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2015,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       VCAN_computer.h
 * @brief      ɽ��๦�ܵ���������λ����������
 * @author     ɽ��Ƽ�
 * @version    v5.2.2
 * @date       2015-03-24
 */
#ifndef _VCAN_COMPUTER_H_
#define _VCAN_COMPUTER_H_

/*
 * ����ͷ�ļ�
 */
#include "common.h"
#include "MK60_uart.h"


//��λ�������ص�ַ��
//��ɽ��๦�ܵ�������V1.1.1�����ڡ�����ͷ��GSM... - ���ܳ������� - http://vcan123.com/forum.php?mod=viewthread&tid=5988&ctid=27

extern void vcan_sendimg(void *imgaddr, uint32_t imgsize);
extern void vcan_sendccd(void *ccdaddr, uint32_t ccdsize);
extern void vcan_sendware(void *wareaddr, uint32_t waresize);


#endif  //_VCAN_COMPUTER_H_