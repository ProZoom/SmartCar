/********************南京理工大学泰州科技学院******************
                 * 文件名       ：Dial_switch.c
                 * 描述         ：拨码开关
***************************************************************/
#include "include.h"

uint8_t key_value=0;
//初始化拨码开关
void Dial_switch_Init(void)
{
  gpio_init(PTA4 ,GPI,0);
  gpio_init(PTA5 ,GPI,0);
  gpio_init(PTA6 ,GPI,0);
  gpio_init(PTA7 ,GPI,0);
}
//读取拨码开关值
uint8_t Read_Coder_Key(void)
{
	if(READ_Coder_2==0&&READ_Coder_3==0) 
                       key_value = 0;   
               if(READ_Coder_2==0&&READ_Coder_3==1) 
                       key_value = 1; 
               if(READ_Coder_2==1&&READ_Coder_3==0) 
                       key_value = 2;
               if(READ_Coder_2==1&&READ_Coder_3==1) 
                       key_value = 3; 
	return key_value; 
}



extern uint32 duty;
extern uint8 buff[DATA_PACKET]; 
extern uint8 Key_jitter;
extern uint32 stop_duty;
uint32 Distance_Set_Goal;
void Coder_ctrl_speed(void)
{
  switch(Read_Coder_Key())//xxxx xxxx & 0000 1111
  {
      case 0:
                duty=30;      //本(后)车占空比
                buff[0]=20;   //前车占空比
                Key_jitter=18; //干簧管停车（抖动）检测到的脉冲数
                stop_duty=20;         //刹车速度
                Distance_Set_Goal=250; //双车设定距离
                break;
      case 1:
	 duty=28;
                buff[0]=18;
                Key_jitter=8;
                stop_duty=25;
                Distance_Set_Goal=300;
                break;
      case 2:
                duty=35;
                buff[0]=19;
                Key_jitter=10;
                stop_duty=25;
                Distance_Set_Goal=300;
                break;
      case 3:
  	 duty=35;
                buff[0]=25;
                Key_jitter=10;
                stop_duty=25;
                Distance_Set_Goal=300;
                break;
      default :
                break;                   
    }   
}
