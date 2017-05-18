/********************南京理工大学泰州科技学院******************
                 * 文件名       ：Communition.c
                 * 描述         ：NRF2401通信模块
***************************************************************/  
#include "include.h"

void Communication_send(uint8 *str) 
{
    uint32 i=0;
    uint8 buff[DATA_PACKET];      //发送数据缓存区
    *buff=*str;
    set_vector_handler(PORTD_VECTORn ,PORTD_IRQHandler);    			//设置 PORTE 的中断服务函数为 PORTE_VECTORn
    enable_irq(PORTD_IRQn);
    while(1)
    {
        //sprintf((char *)buff,"%s%d",str,i);         //把str和i合并成一个字符串到buff里，再进行发送
        if(nrf_tx(buff,DATA_PACKET) == 1 )          //发送一个数据包：buff（包为32字节）
        {
            //等待发送过程中，此处可以加入处理任务
            while(nrf_tx_state() == NRF_TXING);         //等待发送完成
        }
    } 
}

/*******接收*******/

void Commution_receive() 
{
    uint8 buff[DATA_PACKET];                 //定义接收缓冲区
    uint8 relen;
    //配置中断服务函数
    set_vector_handler(PORTD_VECTORn ,PORTD_IRQHandler); //设置 PORTE 的中断服务函数为 PORTE_VECTORn
    enable_irq(PORTD_IRQn);
    while(1)
    {            
        relen = nrf_rx(buff,DATA_PACKET);       //等待接收一个数据包，数据存储在buff里
        if(relen != 0)
        {
           // printf("\n接收到数据:%s",buff);    //打印接收到的数据
        }
    }
}