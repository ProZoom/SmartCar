/********************南京理工大学泰州科技学院******************
                 * 文件名       ：Dial_switch.c
                 * 描述         ：拨码开关
***************************************************************/
#include "include.h"


//初始化拨码开关
void Dial_switch_Init(void)
{
 gpio_init(PTA4 ,GPI,1);
 gpio_init(PTA5 ,GPI,1);
 gpio_init(PTA6 ,GPI,1);
 gpio_init(PTA7 ,GPI,1);
}
//读取拨码开关值
uint8_t Read_Coder_Key(void)
{
	uint8_t key_value=0;
	if(READ_Coder_1) 
                       key_value |= 0x01u;    
	else 
                       key_value &= ~0x01u;   //0xfe
	if(READ_Coder_2) 
                       key_value |= 0x02u;    
	else 
                       key_value &= ~0x02u;     //0xfd
	if(READ_Coder_3) 
                       key_value |= 0x04u;   
	else 
                       key_value &= ~0x04u;     //0xfc
	//if(READ_Coder_4) key_value |= 0x08u;
	//else key_value &= ~0x08u;  //0xfb
	return key_value; 
}



extern float   M_Proportion,M_Integral,M_Derivative; 
extern float Speed_Set_Goal;
void Coder_ctrl_speed(void)
{
  switch((Read_Coder_Key() & 0x0f))//xxxx xxxx & 0000 1111
  {
      case 0x00:
                Speed_Set_Goal = 1000;
                M_Proportion=0;
                M_Integral=0;
                M_Derivative=0; 
                break;
      case 0x01:
	 Speed_Set_Goal = 1500;
                M_Proportion=0;
                M_Integral=0;
                M_Derivative=0;
                break;
      case 0x02:
                Speed_Set_Goal = 2000;
                M_Proportion=0;
                M_Integral=0;
                M_Derivative=0;
                break;
      case 0x03:
  	 Speed_Set_Goal = 2500;
                M_Proportion=0;
                M_Integral=0;
                M_Derivative=0;
                break;
      case 0x04:
	 Speed_Set_Goal = 3000;
                M_Proportion=0;
                M_Integral=0;
                M_Derivative=0;
                break;
      case 0x05:
	 Speed_Set_Goal = 3500;
                M_Proportion=0;
                M_Integral=0;
                M_Derivative=0;
                break;
      case 0x06:
                Speed_Set_Goal = 4000;
                M_Proportion=0;
                M_Integral=0;
                M_Derivative=0;
                break;
      case 0x07:
	 Speed_Set_Goal = 4500;
                M_Proportion=0;
                M_Integral=0;
                M_Derivative=0;
                break;
      default :
                break;                   
    }   
       // Speed_Set_Goal=Speed_Set_Goal+3;
}


//KEY4控制状态标志位
uint8 state_flag;
void KEY_Cont_flag(void)
{
  if(READ_Coder_4) state_flag=1;
  else state_flag=0;
}
