/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_it.h
 * @brief      山外K60 平台中断服务重定向头文件
 * @author     山外科技
 * @version    v5.0
 * @date       2013-06-26
 */


#ifndef __MK60_IT_H__
#define __MK60_IT_H__

/*                          重新定义中断向量表
 *  先取消默认的中断向量元素宏定义        #undef  VECTOR_xxx
 *  在重新定义到自己编写的中断函数      #define VECTOR_xxx    xxx_IRQHandler
 *  例如：
 *       #undef  VECTOR_003                         先取消映射到中断向量表里的中断函数地址宏定义
 *       #define VECTOR_003    HardFault_Handler    重新定义硬件上访中断服务函数
 */
#include    "include.h"
#include    "common.h"
#define AMP1     ADC0_SE8        //PTB0
#define AMP2     ADC0_SE9        //PTB1
#define AMP3     ADC0_SE12       //PTB2
#define AMP4     ADC0_SE13       //PTB3
#define AMP5     ADC1_SE10       //PTB4
#define AMP6     ADC1_SE11       //PTB5
extern int16 *buffer;
extern uint16 Speed;
extern uint16 distance;
extern void Distance_Control(uint16 distance);

extern void PIT0_IRQHandler(void);
extern void PIT1_IRQHandler(void);
extern void PORTD_IRQHandler(void);
extern void PORTE_IRQHandler(void);
#endif  //__MK60_IT_H__
