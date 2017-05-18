/********************�Ͼ�����ѧ̩�ݿƼ�ѧԺ******************
                 * �ļ���       ��Communition.c
                 * ����         ��NRF2401ͨ��ģ��
***************************************************************/  
#include "include.h"

void Communication_send(uint8 *str) 
{
    uint32 i=0;
    uint8 buff[DATA_PACKET];      //�������ݻ�����
    *buff=*str;
    set_vector_handler(PORTD_VECTORn ,PORTD_IRQHandler);    			//���� PORTE ���жϷ�����Ϊ PORTE_VECTORn
    enable_irq(PORTD_IRQn);
    while(1)
    {
        //sprintf((char *)buff,"%s%d",str,i);         //��str��i�ϲ���һ���ַ�����buff��ٽ��з���
        if(nrf_tx(buff,DATA_PACKET) == 1 )          //����һ�����ݰ���buff����Ϊ32�ֽڣ�
        {
            //�ȴ����͹����У��˴����Լ��봦������
            while(nrf_tx_state() == NRF_TXING);         //�ȴ��������
        }
    } 
}

/*******����*******/

void Commution_receive() 
{
    uint8 buff[DATA_PACKET];                 //������ջ�����
    uint8 relen;
    //�����жϷ�����
    set_vector_handler(PORTD_VECTORn ,PORTD_IRQHandler); //���� PORTE ���жϷ�����Ϊ PORTE_VECTORn
    enable_irq(PORTD_IRQn);
    while(1)
    {            
        relen = nrf_rx(buff,DATA_PACKET);       //�ȴ�����һ�����ݰ������ݴ洢��buff��
        if(relen != 0)
        {
           // printf("\n���յ�����:%s",buff);    //��ӡ���յ�������
        }
    }
}