/********************南京理工大学泰州科技学院******************
                 * 文件名       ：Buzzer.c
                 * 描述         ：人机交互模块
***************************************************************/
#include "include.h"
uint8 Sensor_Display_Buffer[8]={'\0'};
extern int16 System_RunTime_Count;
uint8 Distance_Display_Buffer[8]={'\0'};
extern uint16 distance;
void Show(void)
{ 
        Sensor_Display_Buffer[0]=(buffer[0]%10000)/1000+0x30;
        Sensor_Display_Buffer[1]=(buffer[0]%1000)/100+0x30;
        Sensor_Display_Buffer[2]=(buffer[0]%100)/10+0x30;
        Sensor_Display_Buffer[3]=buffer[0]%10+0x30;
        LCD_P6x8Str(0,2,Sensor_Display_Buffer);
        
        Sensor_Display_Buffer[0]=(buffer[1]%10000)/1000+0x30;
        Sensor_Display_Buffer[1]=(buffer[1]%1000)/100+0x30;
        Sensor_Display_Buffer[2]=(buffer[1]%100)/10+0x30;
        Sensor_Display_Buffer[3]=buffer[1]%10+0x30;
        LCD_P6x8Str(0,3,Sensor_Display_Buffer);
        Sensor_Display_Buffer[0]=(buffer[2]%10000)/1000+0x30;
        Sensor_Display_Buffer[1]=(buffer[2]%1000)/100+0x30;
        Sensor_Display_Buffer[2]=(buffer[2]%100)/10+0x30;
        Sensor_Display_Buffer[3]=buffer[2]%10+0x30;
        LCD_P6x8Str(0,4,Sensor_Display_Buffer);
        
        Sensor_Display_Buffer[0]=(buffer[3]%10000)/1000+0x30;
        Sensor_Display_Buffer[1]=(buffer[3]%1000)/100+0x30;
        Sensor_Display_Buffer[2]=(buffer[3]%100)/10+0x30;
        Sensor_Display_Buffer[3]=buffer[3]%10+0x30;
        LCD_P6x8Str(0,5,Sensor_Display_Buffer);
        
        Sensor_Display_Buffer[0]=(buffer[4]%10000)/1000+0x30;
        Sensor_Display_Buffer[1]=(buffer[4]%1000)/100+0x30;
        Sensor_Display_Buffer[2]=(buffer[4]%100)/10+0x30;
        Sensor_Display_Buffer[3]=buffer[4]%10+0x30;
        LCD_P6x8Str(0,6,Sensor_Display_Buffer);
        
        Sensor_Display_Buffer[0]=(buffer[5]%10000)/1000+0x30;
        Sensor_Display_Buffer[1]=(buffer[5]%1000)/100+0x30;
        Sensor_Display_Buffer[2]=(buffer[5]%100)/10+0x30;
        Sensor_Display_Buffer[3]=buffer[5]%10+0x30;
        LCD_P6x8Str(0,7,Sensor_Display_Buffer);
        
        Display(System_RunTime_Count/50,0,0);
        LCD_P8x16Str(60,0,"S");
        
        Distance_Display_Buffer[0]=(distance%1000)/100+0x30;
        Distance_Display_Buffer[1]=(distance%100)/10+0x30;
        Distance_Display_Buffer[2]=(distance%10)+0x30;
        LCD_P8x16Str(60,3,Distance_Display_Buffer);
        LCD_P8x16Str(90,3,"mm");

        
}