/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_i2c.c
 * @brief      i2c��������
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-07-12
 */

#include "common.h"
#include "MK60_port.h"
#include "MK60_i2c.h"

unsigned char MasterTransmission;
unsigned char SlaveID;

I2C_MemMapPtr I2CN[2] = {I2C0_BASE_PTR, I2C1_BASE_PTR}; //��������ָ�����鱣�� I2CN �ĵ�ַ


/*
 *  ��I2Cͨ�ŵ�ÿ��С���趼�ú궨����ʵ�֣������д���㺯��
 *  �˺궨��ο���˼������˾�����޸�����
 */
//�����ź�
#define i2c_Start(I2Cn)             I2C_C1_REG(I2CN[I2Cn]) |= (I2C_C1_TX_MASK | I2C_C1_MST_MASK)    //MST ��0��1��������ʼ�źţ�TX = 1 ���뷢��ģʽ

//ֹͣ�ź�
#define i2c_Stop(I2Cn)              I2C_C1_REG(I2CN[I2Cn]) &= ~(I2C_C1_MST_MASK | I2C_C1_TX_MASK)   //MST ��1��0������ֹͣ�źţ�TX = 0 �������ģʽ

//�ظ�����
#define i2c_RepeatedStart(I2Cn)     I2C_C1_REG(I2CN[I2Cn]) |= I2C_C1_RSTA_MASK

//�������ģʽ(Ӧ��,��Ҫ���ն�����ݣ��������һ���ֽ�ǰ��Ҫ����Ӧ��i2c_DisableAck)
#define i2c_EnterRxMode(I2Cn)       I2C_C1_REG(I2CN[I2Cn]) &= ~(I2C_C1_TX_MASK | I2C_C1_TXAK_MASK)  //

//�������ģʽ(��Ӧ��,ֻ����һ���ֽ�)
#define i2c_PutinRxMode(I2Cn)       I2C_C1_REG(I2CN[I2Cn]) &= ~I2C_C1_TX_MASK;I2C_C1_REG(I2CN[I2Cn]) |= I2C_C1_TXAK_MASK

//����Ӧ��(�������һ���ֽ�)
#define i2c_DisableAck(I2Cn)        I2C_C1_REG(I2CN[I2Cn]) |= I2C_C1_TXAK_MASK

//�ȴ� I2C_S
#define i2c_Wait(I2Cn)              while(( I2C_S_REG(I2CN[I2Cn]) & I2C_S_IICIF_MASK)==0) {} \
                                    I2C_S_REG(I2CN[I2Cn]) |= I2C_S_IICIF_MASK;

//дһ���ֽ�
#define i2c_write_byte(I2Cn,data)   (I2C_D_REG(I2CN[I2Cn]) = (data));i2c_Wait(I2Cn)

/*!
 *  @brief      I2C��ʼ�������ò�����
 *  @param      I2Cn_e      I2Cģ��(I2C0��I2C1)
 *  @param      baud        �ڴ��Ĳ�����
 *  @return                 ʵ�ʵĲ�����
 *  @since      v5.0
 *  Sample usage:       i2c_init(I2C0,400*1000);     // ��ʼ��I2C0���ڴ��Ĳ�����Ϊ400k
 */
uint32 i2c_init(I2Cn_e i2cn, uint32 baud)
{
    if(i2cn == I2C0)
    {
        /* ����ʱ�� */
#if defined(MK60DZ10)
        SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;           //���� I2C0ʱ��
#elif defined( MK60F15)
        SIM_SCGC4 |= SIM_SCGC4_IIC0_MASK;           //���� I2C0ʱ��
#endif

        /* ���� I2C0���ܵ� GPIO �ӿ� */
        if((I2C0_SCL_PIN == PTB0) || (I2C0_SCL_PIN == PTB2) || (I2C0_SCL_PIN == PTD8) )
        {
            port_init (I2C0_SCL_PIN, ALT2 | ODO | PULLUP );
        }
        else
            ASSERT(0);                              //���������������㣬ֱ�Ӷ���ʧ���ˣ����ùܽ�����

        if((I2C0_SDA_PIN == PTB1) || (I2C0_SDA_PIN == PTB3) || (I2C0_SDA_PIN == PTD9) )
            port_init (I2C0_SDA_PIN, ALT2 | ODO | PULLUP );
        else
            ASSERT(0);                              //���������������㣬ֱ�Ӷ���ʧ���ˣ����ùܽ�����
    }
    else
    {
        /* ����ʱ�� */
#if defined(MK60DZ10)
        SIM_SCGC4 |= SIM_SCGC4_I2C1_MASK;           //���� I2C1ʱ��
#elif defined(MK60F15)
        SIM_SCGC4 |= SIM_SCGC4_IIC1_MASK;           //���� I2C1ʱ��
#endif
        /* ���� I2C1���ܵ� GPIO �ӿ� */
        if(I2C1_SCL_PIN == PTE1)
            port_init (I2C1_SCL_PIN, ALT6 | ODO | PULLUP );
        else if(I2C1_SCL_PIN == PTC10)
            port_init (I2C1_SCL_PIN, ALT2 | ODO | PULLUP );
        else
            ASSERT(0);                          //���������������㣬ֱ�Ӷ���ʧ���ˣ����ùܽ�����

        if(I2C1_SDA_PIN == PTE0)
            port_init (I2C1_SDA_PIN, ALT6 | ODO | PULLUP );
        else if (I2C1_SDA_PIN == PTC11)
            port_init (I2C1_SDA_PIN, ALT2 | ODO | PULLUP );
        else
            ASSERT(0);                          //���������������㣬ֱ�Ӷ���ʧ���ˣ����ùܽ�����
    }

    /* ����Ƶ�� */

    // I2C baud rate = bus speed (Hz)/(mul �� SCL divider)  ������ 50MHz/(1 ��128)=390.625kHz
    // SDA hold time = bus period (s) �� mul �� SDA hold value
    // SCL start hold time = bus period (s) �� mul �� SCL start hold value
    // SCL stop hold time = bus period (s) �� mul �� SCL stop hold value

    //��� ICR ��Ӧ��  SCL_divider ���� ��K60P144M100SF2RM.pdf����1468ҳ�� I2C Divider and Hold Values
    uint16 ICR_2_SCL_divider[0x40]  =
    {
        20, 22, 24, 26, 28, 30, 34, 40, 28, 32, 36, 40, 44, 48, 56, 68,
        48, 56, 64, 72, 80, 88, 104, 128, 80, 96, 112, 128, 144, 160, 192, 240,
        160, 192, 224, 256, 288, 320, 384, 480, 320, 384, 448, 512, 576, 640, 768, 960,
        640, 768, 896, 1024, 1152, 1280, 1536, 1920, 1280, 1536, 1792, 2048, 2304, 2560, 3072, 3840
    };

    uint8 mult;
    if(bus_clk_khz <= 50000)mult = 0;         //bus һ��Ƶ
    else  if(bus_clk_khz <= 100000)mult = 1;  //bus ����Ƶ
    else      mult = 2;                       //bus �ķ�Ƶ

    uint16 scldiv =  bus_clk_khz * 1000 / ( (1<<mult) * baud );  //��ѵķ�Ƶϵ��

    //��Ҫ�� ICR_2_SCL_divider ���ҵ� ����ѷ�Ƶϵ��scldiv������� ��Ƶϵ��
    uint8 icr, n = 0x40;
    uint16 min_Dvalue = ~0, Dvalue;

    while(n)                                            //ѭ�������ɨ�裬�ҳ���ӽ��� ��Ƶϵ��
    {
        n--;
        Dvalue = abs(scldiv - ICR_2_SCL_divider[n]);
        if(Dvalue == 0)
        {
            icr = n;
            break;                                      //�˳�whileѭ��
        }

        if(Dvalue < min_Dvalue)
        {
            icr = n;
            min_Dvalue = Dvalue;
        }
    }

    I2C_F_REG(I2CN[i2cn])  = ( 0                        // I2C Frequency Divider register (I2Cx_F)  I2C��Ƶ�Ĵ���   I2C�������Ϊ 400k
                               | I2C_F_MULT(mult)        // �������� mul =  1<<MULT
                               | I2C_F_ICR(icr)          // ʱ������ = ICR_2_SCL_divider[ICR] ������� ICR �� SCL_divider ӳ���ϵ
                             );

    /* ʹ�� I2C */
    I2C_C1_REG(I2CN[i2cn]) = ( 0
                               | I2C_C1_IICEN_MASK       //ʹ��I2C
                               //| I2C_C1_IICIE_MASK       //ʹ���ж�
                             );

    return (  bus_clk_khz * 1000 / ( (1<<mult) * ICR_2_SCL_divider[icr])  );
}


/*!
 *  @brief      I2Cͨ�Ž�������Ҫ���õĺ�������
 *  @since      v5.0
 *  @note       ���ͨ��ʧ�ܣ��ɳ����������ʱֵ��ȷ���Ƿ���ʱ���µ�
 */
void i2c_delay(void)
{
    volatile uint16 n = 80;     //ע�⣬�������̫С���ᵼ�¶�ȡ����

    while(n--)
    {
        asm("nop");
    }
}

/*!
 *  @brief      ��ȡI2C�豸ָ����ַ�Ĵ���������
 *  @param      I2Cn_e        I2Cģ��(I2C0��I2C1)
 *  @param      SlaveID     �ӻ���ַ(7λ��ַ)
 *  @param      reg         �ӻ��Ĵ�����ַ
 *  @return                 ��ȡ�ļĴ���ֵ
 *  @since      v5.0
 *  Sample usage:       uint8 value = i2c_read_reg(I2C0, 0x1D, 1);
 */
uint8 i2c_read_reg(I2Cn_e i2cn, uint8 SlaveID, uint8 reg)
{

    //��д��Ĵ�����ַ,�ٶ�ȡ����,��˴˹����� I2C �ĸ��ϸ�ʽ,�ı����ݷ���ʱ��Ҫ��������
    uint8 result;

    ASSERT((SlaveID & 0x80) == 0);                      //���ԣ�����Ҫ���7λ��ַ��ֵ������7bit,����ͨ��ʱҪ��ĸ�7λ
    //��Щ�ֲᣬ������7λ��ַָ����8bit��ĸ�7λ
    //��Щ�ֲᣬ������7λ��ַָ����7bit
    //������ȷ�ϣ����Գ����Ƿ�ͨ��������ȷ��

    i2c_Start(i2cn);                                    //���������ź�

    i2c_write_byte(i2cn, ( SlaveID << 1 ) | MWSR);      //���ʹӻ���ַ��дλ

    i2c_write_byte(i2cn, reg);                          //���ʹӻ���ļĴ�����ַ

    i2c_RepeatedStart(i2cn);                            //���ϸ�ʽ���������������ź�

    i2c_write_byte(i2cn, ( SlaveID << 1) | MRSW );      //���ʹӻ���ַ�Ͷ�λ

    i2c_PutinRxMode(i2cn);                              //�������ģʽ(��Ӧ��,ֻ����һ���ֽ�)
    result = I2C_D_REG(I2CN[i2cn]);                     //��ٶ�ȡһ�Σ�������������
    i2c_Wait(i2cn);                                     //�ȴ��������

    i2c_Stop(i2cn);                                     //����ֹͣ�ź�

    result = I2C_D_REG(I2CN[i2cn]);                     //��ȡ����

    i2c_delay();                                        //������ʱһ�£��������

    return result;
}


/*!
 *  @brief      д��һ���ֽ����ݵ�I2C�豸ָ���Ĵ�����ַ
 *  @param      I2Cn_e        I2Cģ��(I2C0��I2C1)
 *  @param      SlaveID     �ӻ���ַ(7λ��ַ)
 *  @param      reg         �ӻ��Ĵ�����ַ
 *  @param      Data        ����
 *  @since      v5.0
 *  Sample usage:       i2c_write_reg(I2C0, 0x1D, 1,2);     //��ӻ�0x1D �ļĴ��� 1 д������ 2
 */

void i2c_write_reg(I2Cn_e i2cn, uint8 SlaveID, uint8 reg, uint8 Data)
{

    i2c_Start(i2cn);                                    //���������ź�

    i2c_write_byte(i2cn, ( SlaveID << 1 ) | MWSR);      //���ʹӻ���ַ��дλ

    i2c_write_byte(i2cn, reg);                          //���ʹӻ���ļĴ�����ַ

    i2c_write_byte(i2cn, Data);                         //������Ҫд�������

    i2c_Stop(i2cn);

    i2c_delay();                                        //��ʱ̫�̵Ļ�������д����
}

