/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       system_MK60DZ10.c
 * @brief      ϵͳ������غ���
 * @author     ɽ��Ƽ�
 * @version    v5.2
 * @date       2014-10-09
 */

#include "common.h"
#include "system_MK60DZ10.h"
#include "MK60_uart.h"
#include "MK60_wdog.h"
#include "MK60_gpio.h"



int core_clk_khz;
int core_clk_mhz;
int bus_clk_khz;

extern void main(void);

/*!
 *  @brief      kinetis ��������
 *  @since      v5.0
 *  @author     ��˼������˾
 *  @note       �˺�����ϵͳ���������ɻ�ຯ�����ã�Ȼ��ִ��main����
 */
void start(void)
{
    wdog_disable();     // �رտ��Ź�

    common_startup();   // �����ж������� �� ��Ҫ�����ݵ� RAM��

    sysinit();          // ϵͳ��ʼ��������ϵͳƵ�ʣ���ʼ��printf�˿�

    //Ϊ�˷�ֹ main ���� �� ������ ���ؿڣ����� ����ʧ�ܣ��˴����� ���� KEY_A �ͽ�����ѭ��
    SRART_CHECK();

    gpio_init(PTA4,GPO,1);  //��ʼ��Ϊ���1���� ������ NMI �ж�

    main();             // ִ���û�������

    while(1);           // ��ѭ��
}

/*!
 *  @brief      ϵͳ��ʼ��������ϵͳƵ�ʣ���ʼ��printf�˿�
 *  @since      v5.0
 *  @note       �˺�����ϵͳ���������ɻ�ຯ�����ã�Ȼ��ִ��main����
 */
void sysinit (void)
{
    core_clk_mhz = pll_init(CORE_CLK);     //����ϵͳʱ��

    core_clk_khz = core_clk_mhz * 1000;     //�����úõ�ʱ�Ӷ�������ȫ�ֱ�����Ա����ʹ��
    bus_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24) + 1);

    uart_init (VCAN_PORT, VCAN_BAUD);   //��ʼ��printf�õ��Ĺܽ�
}

/*!
 *  @brief      trace ʱ����� ��ʼ��
 *  @since      v5.0
 *  @note       ������У��ʱ������Ƿ���ȷ
 *  @author     ��˼������˾
 */
void trace_clk_init(void)
{
    /* Set the trace clock to the core clock frequency */
    SIM_SOPT2 |= SIM_SOPT2_TRACECLKSEL_MASK;

    /* Enable the TRACE_CLKOUT pin function on PTA6 (alt7 function) */
    PORTA_PCR6 = ( PORT_PCR_MUX(0x7));
}

/*!
 *  @brief      fb ʱ����� ��ʼ��
 *  @since      v5.0
 *  @note       ������У��ʱ������Ƿ���ȷ
 *  @author     ��˼������˾
 */
void fb_clk_init(void)
{
    /* Enable the clock to the FlexBus module */
    SIM_SCGC7 |= SIM_SCGC7_FLEXBUS_MASK;

    /* Enable the FB_CLKOUT function on PTC3 (alt5 function) */
    PORTC_PCR3 = ( PORT_PCR_MUX(0x5));
}


/*!
 *  @brief      �����ж������� �� ��Ҫ�����ݵ� RAM��
 *  @since      v5.0
 *  @author     ��˼������˾
 */
#pragma section = ".data"
#pragma section = ".data_init"
#pragma section = ".bss"
#pragma section = "CodeRelocate"
#pragma section = "CodeRelocateRam"

void common_startup(void)
{
    /* Declare a counter we'll use in all of the copy loops */
    uint32 n;

    /* Declare pointers for various data sections. These pointers
     * are initialized using values pulled in from the linker file
     */
    uint8 *data_ram, * data_rom, * data_rom_end;
    uint8 *bss_start, * bss_end;


    /*  VECTOR_TABLE �� VECTOR_RAM �ĵ�ַ��  linker �ļ����� ��*.icf�� */
    extern uint32 __VECTOR_TABLE[];
    extern uint32 __VECTOR_RAM[];

    /* ����ROM����ж�������RAM�� */
    if (__VECTOR_RAM != __VECTOR_TABLE)             //�������RAM����������Ҫ�����ж�������
    {
        for (n = 0; n < 0x410; n++)                 //�������
            __VECTOR_RAM[n] = __VECTOR_TABLE[n];
    }
    /* ָ���µ��ж��������ַΪ __VECTOR_RAM */
    write_vtor((uint32)__VECTOR_RAM);

    /* ���Ѹ���ֵ�ı�����ROM�︴�����ݵ�RAM�� */
    data_ram = __section_begin(".data");            //�Ѹ���ֵ�ı����ĵ�ַ��RAM��
    data_rom = __section_begin(".data_init");       //�Ѹ���ֵ�ı��������ݴ����ROM���Ҫ��ֵ��RAM��
    data_rom_end = __section_end(".data_init");
    n = data_rom_end - data_rom;

    /* ���Ƴ�ʼ�����ݵ�RAM�� */
    while (n--)
        *data_ram++ = *data_rom++;

    /* û����ֵ���߳�ֵΪ0�ı�������Ҫ�����RAM������ݣ�ȷ��ֵΪ0 */
    bss_start = __section_begin(".bss");
    bss_end = __section_end(".bss");

    /* ���û����ֵ���߳�ֵΪ0�ı�������ֵ */
    n = bss_end - bss_start;
    while(n--)
        *bss_start++ = 0;

    /* ��ֵ�� __ramfunc �����ĺ����ĵĴ���ε� RAM�����Լӿ���������        */
    uint8 *code_relocate_ram = __section_begin("CodeRelocateRam");
    uint8 *code_relocate = __section_begin("CodeRelocate");
    uint8 *code_relocate_end = __section_end("CodeRelocate");

    /* ��ROM�︴�ƺ������뵽RAM�� */
    n = code_relocate_end - code_relocate;
    while (n--)
        *code_relocate_ram++ = *code_relocate++;

}
