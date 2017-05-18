/********************�Ͼ�����ѧ̩�ݿƼ�ѧԺ******************
                 * �ļ���       ��test.c
                 * ����         ��ģ�����
***************************************************************/
#include "common.h"
#include "include.h"
/*******����������ģ��********/
#if 0

void  main(void)
{
   //-------------------------------���ڳ�ʼ��---------------------------------------
    uart_init(UART5,9600); 
    lptmr_pulse_init(LPT0_ALT1, 0xffff, LPT_Rising); 
    ftm_pwm_init(FTM0, FTM_CH0,10*1000,0);
    ftm_pwm_init(FTM0, FTM_CH1,10*1000,0);
    //---------------------------------------��ʱ���жϳ�ʼ��--------------------------------------------
    pit_init_ms(PIT1,20);                                    //��ʼ��PIT1����ʱʱ��Ϊ�� 20ms
    set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler);      //����PIT1���жϷ�����Ϊ PIT1_IRQHandler
    enable_irq (PIT1_IRQn); //���ٶȲɼ��жϺ�����20ms,���ڱջ�����
   //��ѭ���������˻��������棬ͨ�ţ�ͣ��
   while(1)
    {
      FTM_CnV_REG(FTMN[0], FTM_CH1) = 0;                   
      FTM_CnV_REG(FTMN[0], FTM_CH0) = 2000;
    }
}
#endif
/*******NRF����ģ��********/
/*******����*******/
#if 0
   void  main(void)
{
    uart_init(UART5,9600); 
    uint32 i=0;
    uint8 buff[DATA_PACKET];      //�������ݻ�����
    uint8 *str = "i love you";
    printf("\n\n\n***********����ģ��NRF24L01+����************");
    while(!nrf_init())                  //��ʼ��NRF24L01+ ,�ȴ���ʼ���ɹ�Ϊֹ
    {
        printf("\n  NRF��MCU����ʧ�ܣ������¼����ߡ�\n");
    }
    set_vector_handler(PORTD_VECTORn ,PORTD_IRQHandler);    			//���� PORTE ���жϷ�����Ϊ PORTE_VECTORn
    enable_irq(PORTD_IRQn);
    while(1)
    {
        sprintf((char *)buff,"%s%d",str,i);         //��str��i�ϲ���һ���ַ�����buff��ٽ��з���
        if(nrf_tx(buff,DATA_PACKET) == 1 )          //����һ�����ݰ���buff����Ϊ32�ֽڣ�
        {
            //�ȴ����͹����У��˴����Լ��봦������
            while(nrf_tx_state() == NRF_TXING);         //�ȴ��������
            if( NRF_TX_OK == nrf_tx_state () )
            {
                printf("\n���ͳɹ�:%d",i);
                i++;                                    //���ͳɹ����1������֤�Ƿ�©��
            }
            else 
            {
               printf("\n����ʧ��:%d",i);
            }
        }
        else
        {
             printf("\n����ʧ��:%d",i);
        }
    } 
}
#endif
/*******����*******/
#if 0
   void main(void)
{
    uint8 buff[DATA_PACKET];                        //������ջ�����
    uint8 relen;

    printf("\n\n\n***********����ģ��NRF24L01+����************");

    while(!nrf_init())        //��ʼ��NRF24L01+ ,�ȴ���ʼ���ɹ�Ϊֹ
    {
        printf("\n  NRF��MCU����ʧ�ܣ������¼����ߡ�\n");
    }
    //�����жϷ�����
    set_vector_handler(PORTD_VECTORn ,PORTD_IRQHandler);                //���� PORTE ���жϷ�����Ϊ PORTE_VECTORn
    enable_irq(PORTD_IRQn);

    printf("\n      NRF��MCU���ӳɹ���\n"); 

    while(1)
    {            
        relen = nrf_rx(buff,DATA_PACKET);               //�ȴ�����һ�����ݰ������ݴ洢��buff��
        if(relen != 0)
        {
            printf("\n���յ�����:%s",buff);             //��ӡ���յ�������
        }
    }
}
#endif
/*******��λ������ģ��********/
#if 0
extern uint16 PID_m_add ; 
extern float Speed_Set_Goal;
void main()
{
    //��λ�� ����ʾ������Ҫ���ó� ������Ϊ6����������Ϊ uint8_t
    
    uint8 var[6];
    while(1)
    {
        var[0] =0;
        var[1] = 0;
        var[2] =0;
        var[3] = 10;
        var[4] = 15;
        var[5] = 20;

        vcan_sendware(var, sizeof(var));
        //DELAY_MS(10);
    }

}
#endif