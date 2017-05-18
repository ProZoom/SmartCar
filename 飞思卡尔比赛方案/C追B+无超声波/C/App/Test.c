/********************南京理工大学泰州科技学院******************
                 * 文件名       ：test.c
                 * 描述         ：模块测试
***************************************************************/
#include "common.h"
#include "include.h"
/*******编码器测试模块********/
#if 0

void  main(void)
{
   //-------------------------------串口初始化---------------------------------------
    uart_init(UART5,9600); 
    lptmr_pulse_init(LPT0_ALT1, 0xffff, LPT_Rising); 
    ftm_pwm_init(FTM0, FTM_CH0,10*1000,0);
    ftm_pwm_init(FTM0, FTM_CH1,10*1000,0);
    //---------------------------------------定时器中断初始化--------------------------------------------
    pit_init_ms(PIT1,20);                                    //初始化PIT1，定时时间为： 20ms
    set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler);      //设置PIT1的中断服务函数为 PIT1_IRQHandler
    enable_irq (PIT1_IRQn); //开速度采集中断函数，20ms,用于闭环控制
   //主循环，用于人机交互界面，通信，停车
   while(1)
    {
      FTM_CnV_REG(FTMN[0], FTM_CH1) = 0;                   
      FTM_CnV_REG(FTMN[0], FTM_CH0) = 2000;
    }
}
#endif
/*******NRF测试模块********/
/*******发送*******/
#if 0
   void  main(void)
{
    uart_init(UART5,9600); 
    uint32 i=0;
    uint8 buff[DATA_PACKET];      //发送数据缓存区
    uint8 *str = "i love you";
    printf("\n\n\n***********无线模块NRF24L01+测试************");
    while(!nrf_init())                  //初始化NRF24L01+ ,等待初始化成功为止
    {
        printf("\n  NRF与MCU连接失败，请重新检查接线。\n");
    }
    set_vector_handler(PORTD_VECTORn ,PORTD_IRQHandler);    			//设置 PORTE 的中断服务函数为 PORTE_VECTORn
    enable_irq(PORTD_IRQn);
    while(1)
    {
        sprintf((char *)buff,"%s%d",str,i);         //把str和i合并成一个字符串到buff里，再进行发送
        if(nrf_tx(buff,DATA_PACKET) == 1 )          //发送一个数据包：buff（包为32字节）
        {
            //等待发送过程中，此处可以加入处理任务
            while(nrf_tx_state() == NRF_TXING);         //等待发送完成
            if( NRF_TX_OK == nrf_tx_state () )
            {
                printf("\n发送成功:%d",i);
                i++;                                    //发送成功则加1，可验证是否漏包
            }
            else 
            {
               printf("\n发送失败:%d",i);
            }
        }
        else
        {
             printf("\n发送失败:%d",i);
        }
    } 
}
#endif
/*******接收*******/
#if 0
   void main(void)
{
    uint8 buff[DATA_PACKET];                        //定义接收缓冲区
    uint8 relen;

    printf("\n\n\n***********无线模块NRF24L01+测试************");

    while(!nrf_init())        //初始化NRF24L01+ ,等待初始化成功为止
    {
        printf("\n  NRF与MCU连接失败，请重新检查接线。\n");
    }
    //配置中断服务函数
    set_vector_handler(PORTD_VECTORn ,PORTD_IRQHandler);                //设置 PORTE 的中断服务函数为 PORTE_VECTORn
    enable_irq(PORTD_IRQn);

    printf("\n      NRF与MCU连接成功！\n"); 

    while(1)
    {            
        relen = nrf_rx(buff,DATA_PACKET);               //等待接收一个数据包，数据存储在buff里
        if(relen != 0)
        {
            printf("\n接收到数据:%s",buff);             //打印接收到的数据
        }
    }
}
#endif
/*******上位机调试模块********/
#if 0
extern uint16 PID_m_add ; 
extern float Speed_Set_Goal;
void main()
{
    //上位机 虚拟示波器需要配置成 波形数为6，数据类型为 uint8_t
    
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