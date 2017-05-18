
/********************�Ͼ�����ѧ̩�ݿƼ�ѧԺ******************
                 * �ļ���       ��Visual_Scope.c
                 * ����         ��Serial_Digital_Scope ��λ��ģ��
 1.  //������Ҫ���͵����� real_angle �� g_fCarAngle �� ENC03 ��Gyro_Now   
 2.  OutData[0] = real_angle;   
 3.  OutData[1] = g_fCarAngle;   
 4.  OutData[2] = ENC03 ;   
 5.  OutData[3] = Gyro_Now;   
 6.   
 7.  //ִ�з��ͺ���   
 8.  OutPut_Data();  

***************************************************************/
#include "include.h"
float OutData[4] = { 0 };
unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
 {
     unsigned short CRC_Temp;
     unsigned char i,j;
     CRC_Temp = 0xffff;

     for (i=0;i<CRC_CNT; i++){      
         CRC_Temp ^= Buf[i];
         for (j=0;j<8;j++) {
             if (CRC_Temp & 0x01)
                 CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
             else
                 CRC_Temp = CRC_Temp >> 1;
         }
     }
     return(CRC_Temp);
 }

 
void OutPut_Data(void)
 {
   int temp[4] = {0};
   unsigned int temp1[4] = {0};
   unsigned char databuf[10] = {0};
   unsigned char i;
   unsigned short CRC16 = 0;
   for(i=0;i<4;i++)
    {
     temp[i]  = (int)OutData[i];
     temp1[i] = (unsigned int)temp[i];
    }
   for(i=0;i<4;i++) 
   {
     databuf[i*2]   = (unsigned char)(temp1[i]%256);
     databuf[i*2+1] = (unsigned char)(temp1[i]/256);
   }
   CRC16 = CRC_CHECK(databuf,8);
   databuf[8] = CRC16%256;
   databuf[9] = CRC16/256;
   for(i=0;i<10;i++)
   {
     uart_putchar(UART5,(char)databuf[i]);
   }
 }
 