/********************�Ͼ�����ѧ̩�ݿƼ�ѧԺ******************
                 * �ļ���       ��Signal_Process_Algorithm.c
                 * ����         ���㷨��
***************************************************************/

#include "common.h"
#include "MK60_adc.h" 
#include "Signal_Process_Algorithm.h"
#include  "MK60_FTM.h"
#include "MK60_uart.h"
#define N 10 

uint16 Lean_Left_Buffer[N],      Lean_Right_Buffer[N],  
       Outside_Left_Buffer[N],   Outside_Right_Buffer[N],
       Inside_Left_Buffer[N],    Inside_Right_Buffer[N],     
       
       Lean_Left,              Lean_Right, 
       Outside_Left,           Outside_Right,
       Inside_Left,            Inside_Right,
       Filter_Sum;

double  Lean_Left_Max,               Lean_Right_Max, 
         Outside_Left_Max=190,        Outside_Right_Max=190,
         Inside_Left_Max=190,         Inside_Right_Max=190;
              

int16 Filter_Finish_NumericalValue[20]={0};
double Outside_Delta,Inside_Delta,Outside_Sum,Inside_Sum,Lean_Sum,
        Outside_Pre1_Delta,Inside_Pre1_Delta,
        Outside_Pre2_Delta,Inside_Pre2_Delta,
        Outside_Pre3_Delta,Inside_Pre3_Delta,
        Outside_Pre4_Delta,Inside_Pre4_Delta,
        Lean_Delta;
      
//char  coe[N] = {1,2,3,4,5,6,7,8,9,10};  //Ȩ��ϵ����  
//char  sum_coe = 1+2+3+4+5+6+7+8+9+10;   //Ȩ�غ�
//char Outside_Lose_Track_Flag=0,Left_Lose=0,Right_Lose=0,All_Outside_Lose_Track_Flag=0;
double Turn;
double Offset_Buffer[100]; 
//char Turn_Left_Flag_1=0,Turn_Right_Flag_1=0,Turn_Left_Flag_2=0,Turn_Right_Flag_2=0;


struct
{
   double Proportion;                        //����λ��ʽPID��������
 //double Integral;                          //����λ��ʽPID���ֳ���
   double Derivative;                        //����λ��ʽPID΢�ֳ���
   double Last_Error;                        //Error(k-1)  ��һ�β�ֵ
   double Current_Error;                     //Error(k)   ��ǰ��ֵ
}Position_PID; 

/***********��Ȩ����ƽ���˲�*********/
void Filter() 
{ 
   char count;
   
   /*���ݲɼ��洢�������*/
   Lean_Left=0,                 Lean_Right=0, 
   Outside_Left=0,              Outside_Right=0,
   Inside_Left=0,               Inside_Right=0;
   
   /*��ʷ���ݵ��ƴ洢*/
   for(count=4;count>0;count--)
   {
       Outside_Left_Buffer[count]    =   Outside_Left_Buffer[count-1]; 
       Outside_Right_Buffer[count]   =   Outside_Right_Buffer[count-1]; 
       Inside_Left_Buffer[count]    =   Inside_Left_Buffer[count-1]; 
       Inside_Right_Buffer[count]   =   Inside_Right_Buffer[count-1];
       Lean_Left_Buffer[count]    =   Lean_Left_Buffer[count-1]; 
       Lean_Right_Buffer[count]   =   Lean_Right_Buffer[count-1];
   }
   
   /*�����ݲ������*/ 
   Outside_Left_Buffer[0]   =   adc_once(ADC1_SE13, ADC_8bit);  
   Outside_Right_Buffer[0]  =   adc_once(ADC0_SE8, ADC_8bit);   
        
   Inside_Left_Buffer[0]    =   adc_once(ADC1_SE12, ADC_8bit);  
   Inside_Right_Buffer[0]   =   adc_once(ADC1_SE9, ADC_8bit);    
       
   Lean_Left_Buffer[0]      =   adc_once(ADC1_SE11, ADC_8bit);  
   Lean_Right_Buffer[0]     =   adc_once(ADC0_SE12, ADC_8bit);  
   
     
  /*����������Ȩ��λ����*/    
   Outside_Left_Buffer[6]    =  0;
   Outside_Right_Buffer[6]   =  0;
         
   Inside_Left_Buffer[6]     =  0;
   Inside_Right_Buffer[6]    =  0; 
       
   Lean_Left_Buffer[6]    =  0;
   Lean_Right_Buffer[6]   =  0;
   
   /*��Ȩ�ʹ洢*/
   for(count=0;count<5;count++)
   {
     Outside_Left_Buffer[6]+=(5-count)*Outside_Left_Buffer[count];
     Outside_Right_Buffer[6]+=(5-count)*Outside_Right_Buffer[count];
     
     Inside_Left_Buffer[6]+= (5-count)*Inside_Left_Buffer[count]; 
     Inside_Right_Buffer[6]+=(5-count)*Inside_Right_Buffer[count];
     
     Lean_Left_Buffer[6]+= (5-count)*Lean_Left_Buffer[count]; 
     Lean_Right_Buffer[6]+=(5-count)*Lean_Right_Buffer[count]; 
   }
   
   /*ƽ������*/
   Outside_Left_Buffer[6]/=15;
   Outside_Right_Buffer[6]/=15;
     
   Inside_Left_Buffer[6]/=15; 
   Inside_Right_Buffer[6]/=15;
   
   Lean_Left_Buffer[6]/=15; 
   Lean_Right_Buffer[6]/=15;
   
   
    /*ȡ����Ч���ݴ�������������к�������*/
    Outside_Left     =  Outside_Left_Buffer[6];
    Outside_Right    =  Outside_Right_Buffer[6]; 
       
    Inside_Left      =  Inside_Left_Buffer[6]; 
    Inside_Right     =  Inside_Right_Buffer[6];
    
    Lean_Left        =  Lean_Left_Buffer[6]; 
    Lean_Right       =  Lean_Right_Buffer[6];
    
   Filter_Finish_NumericalValue[0] = Outside_Left; 
   
   Filter_Finish_NumericalValue[1] = Outside_Right; 

   Filter_Finish_NumericalValue[2] = Inside_Left; 
   
   Filter_Finish_NumericalValue[3] = Inside_Right;
  
   Filter_Finish_NumericalValue[4] = Lean_Left;  
  
   Filter_Finish_NumericalValue[5] = Lean_Right;
}

//����׼������������������
void Normalization()
{
   /*5��ȡ�����˲�*/
   Filter();
   /*��ȡ�������ֵ����*/
   if(Filter_Finish_NumericalValue[0]>Outside_Left_Max)
     Outside_Left_Max=Filter_Finish_NumericalValue[0];
   if(Filter_Finish_NumericalValue[1]>Outside_Right_Max)
     Outside_Right_Max=Filter_Finish_NumericalValue[1];
   if(Filter_Finish_NumericalValue[2]>Inside_Left_Max)
     Inside_Left_Max=Filter_Finish_NumericalValue[2];
   if(Filter_Finish_NumericalValue[3]>Inside_Right_Max)
     Inside_Right_Max=Filter_Finish_NumericalValue[3];
   if(Filter_Finish_NumericalValue[4]>Lean_Left_Max)
     Lean_Left_Max=Filter_Finish_NumericalValue[4];
   if(Filter_Finish_NumericalValue[5]>Lean_Right_Max)
     Lean_Right_Max=Filter_Finish_NumericalValue[5];
}

extern  float OutData[4];
//ƫ�����
int16 *Offset_Caculate()
{
   /*����׼��*/
   Outside_Left     =   Outside_Left/Outside_Left_Max*100;
   Outside_Right    =   Outside_Right/Outside_Right_Max*100; 
       
   Inside_Left     =   Inside_Left/Inside_Left_Max*100; 
   Inside_Right    =   Inside_Right/Inside_Right_Max*100;
   
   Lean_Left     =   Lean_Left/Lean_Left_Max*100; 
   Lean_Right    =   Lean_Right/Lean_Right_Max*100; 
   
   Filter_Finish_NumericalValue[0] = Outside_Left; 
   //OutData[0]=Filter_Finish_NumericalValue[0];
   
   Filter_Finish_NumericalValue[1] = Outside_Right; 
   //OutData[1]=Filter_Finish_NumericalValue[1];

   Filter_Finish_NumericalValue[2] = Inside_Left; 
   
   Filter_Finish_NumericalValue[3] = Inside_Right;
  
   Filter_Finish_NumericalValue[4] = Lean_Left;  
   //OutData[2]=Filter_Finish_NumericalValue[4];
  
   Filter_Finish_NumericalValue[5] = Lean_Right;
   
   //OutData[3]=Filter_Finish_NumericalValue[5];
   
   
   /*����мӺ�����*/
   Outside_Sum     =  Outside_Left+Outside_Right; 
   Outside_Delta   =  fabs(Outside_Left-Outside_Right); 
   /*�ڲ��мӺ�����*/
   Inside_Sum   =  Inside_Left+Inside_Right;
   Inside_Delta =  Inside_Left-Inside_Right;
   /*���ֵ�мӺ�����*/
   Lean_Sum   = Lean_Left+Lean_Right;
   Lean_Delta = fabs(Lean_Left-Lean_Right);

         /*�����ƫ����㣬�����ţ���λ��ʽPD������������������ͬ*/
         if(Outside_Delta<5&&Outside_Delta>-5) Outside_Delta=0;
         
         /*�ڲ���ƫ�����㣬�����ţ���λ��ʽPD������������������ͬ*/
         if(Inside_Delta<5&&Inside_Delta>-5) Inside_Delta=0;
      
       
         /*����в�Ⱥ����㣬Ŀ����Ϊ�������Ƕ�Ӱ�� ��1.0��ֹ��ĸΪ0*/
         Outside_Delta =100*Outside_Delta/(1.0+Outside_Sum); 
         /*�ڲ��в�Ⱥ����㣬Ŀ����Ϊ�������Ƕ�Ӱ�� ��1.0��ֹ��ĸΪ0*/
         Inside_Delta =100*Inside_Delta/(1.0+Inside_Sum);    
      
         /*�洢���4�������ƫ��*/
         Outside_Pre4_Delta=Outside_Pre3_Delta;
         Outside_Pre3_Delta=Outside_Pre2_Delta; 
         Outside_Pre2_Delta=Outside_Pre1_Delta;
         Outside_Pre1_Delta=Outside_Delta;
       
         /*�洢���4���ڲ���ƫ��*/
         Inside_Pre4_Delta=Inside_Pre3_Delta;
         Inside_Pre3_Delta=Inside_Pre2_Delta;
         Inside_Pre2_Delta=Inside_Pre1_Delta;
         Inside_Pre1_Delta=Inside_Delta;
    return Filter_Finish_NumericalValue;  
}


                           
//λ��ʽPD��������ʼ��
void Position_PID_Init()
{
   Position_PID.Proportion = 1;
   Position_PID.Derivative = 0;
}


//λ��ʽPD������
uint16 Position_PID_Controller(int16 Offset)
{
   /*����λ��ʽPD�������ֲ�����*/
   double P,D,Angle_Value; 
   
   /*����ÿ�εĲ�ֵ*/
   Position_PID.Current_Error=Offset; 
   
   /*λ��ʽ��P��ʽ  Kp*E(k)*/
   P=Position_PID.Proportion*Position_PID.Current_Error;    
   
   /*λ��ʽ��D��ʽ  Kd*[E(k)-E(k-1)]*/
   D=Position_PID.Derivative*(Position_PID.Current_Error-Position_PID.Last_Error);  
   
   /*λ��ʽPD�������MINDTURNΪ�������ֵ*/
   Angle_Value=MIDVALUE+P+D; 
   
   /*ǰ����ת���޷�����*/
   if(Angle_Value>LEFTMAX)   Angle_Value=LEFTMAX; 
   
   /*ǰ����ת���޷�����*/
   if(Angle_Value<RIGHTMAX)  Angle_Value=RIGHTMAX; 
   
   /*����ÿ�εĲ�ֵ*/
   Position_PID.Last_Error=Position_PID.Current_Error; 
   
   /*���ض�����ֵ*/ 
   return Angle_Value;
}


//���δ������̵�λ�ý����㷨  f(x)=0.0033*(x.^3)+0.0272*(x.^2)-4.8804*x-4.8277-offset �������[-20,21]
extern uint32 duty;
double Position()
{ 
  double x=0;
  static double Previous_Angle;
 if(Inside_Left<30&&Inside_Right<30&&Lean_Left<30&&Lean_Right<30)//��ȫ����
 {
    x=Previous_Angle;
    Turn=Position_PID_Controller(x);
 }
 else//����ȫ����
 {
     if(Inside_Sum>80)
     {
       //ֱ��
       if(Inside_Sum>140)
       {
          Position_PID.Proportion = 1;
       }
       else if(Inside_Sum>135)
       {
          Position_PID.Proportion = 2;
       }
       else if(Inside_Sum>130)
       {
          Position_PID.Proportion = 3;
       }
       else if(Inside_Sum>120)
       {
          Position_PID.Proportion = 4;
       }
       else if(Inside_Sum>110)
       {
          Position_PID.Proportion = 5;
       }
       else if(Inside_Sum>100)
       {
          Position_PID.Proportion = 6;
       }
       else if(Inside_Sum>90)
       {
          Position_PID.Proportion = 7;
       }
       else if(Inside_Sum>80)
       {
          Position_PID.Proportion = 8;
       } 
       else
       {
          Position_PID.Proportion = 10;
       }
       Inside_Delta=0.6*Inside_Delta+0.4*(Inside_Pre1_Delta-Inside_Pre4_Delta);
       
       x=2*(0.005*Inside_Delta*Inside_Delta+1.55*Inside_Delta-7.395);
       
       Turn=Position_PID_Controller(x);
       Previous_Angle=x;
     }
     else if(Lean_Sum<105)
     {
       Position_PID.Proportion = 25;
       if(Lean_Left<Lean_Right)
       {
         x=2.8*(0.0019*Inside_Sum*Inside_Sum-0.17*Inside_Sum+11.86)-134;
         //ftm_pwm_duty(FTM0, FTM_CH1,duty-5);
         Previous_Angle=x;
       }
       else
       {
         x=-2.8*(0.0019*Inside_Sum*Inside_Sum-0.17*Inside_Sum+11.86)+100;
         //ftm_pwm_duty(FTM0, FTM_CH0,duty-5);
         Previous_Angle=x;
       }
       Turn=Position_PID_Controller(x);
     }
     //ֱ��
     /*if(Lean_Delta>70 && Outside_Delta>15)
     {
         ftm_pwm_duty(FTM0, FTM_CH1,30);
         ftm_pwm_duty(FTM0, FTM_CH3,30);
     }
     else
     { 
       ftm_pwm_duty(FTM0, FTM_CH1,35);
       ftm_pwm_duty(FTM0, FTM_CH3,35);
     }*/
 }   
     FTM_CnV_REG(FTMN[1], FTM_CH1)=Turn;
       
}















/*����ʽPID�ṹ�壬���ڵ���ջ�*/
struct
{
  int16 Current_Error;  //��ǰ��ֵ   E(0)
  int16 Last_Error;     //�ϴβ�ֵ   E(-1)
  int16 Prev_Error;     //���ϴβ�ֵ E(-2)
}M_PID; //����һ����ΪPID_M �Ľṹ��

float M_Proportion =10,M_Integral =0,M_Derivative =0; //����Kp��Ki��Kd ��������
uint16 PID_m_add ; //PID ���������
void Motor_Control(uint16 Speed)
{
   /*����ֲ�����*/
   int16 P,I,D;
   /*����ÿ�εĲ�ֵ*/
   M_PID.Prev_Error=M_PID.Last_Error;
   /*����ÿ�εĲ�ֵ*/
   M_PID.Last_Error=M_PID.Current_Error;
    /*����ÿ�εĲ�ֵ*/
   M_PID.Current_Error=300-Speed;
   
   P=M_Proportion*(M_PID.Current_Error-M_PID.Last_Error); //����P �����ʽ
   I=M_Integral* M_PID.Current_Error; //����I �����ʽ
   D=M_Derivative*(M_PID.Current_Error+M_PID.Prev_Error-2*M_PID.Last_Error); //΢��D �����ʽ
   
   PID_m_add=P+I+D+PID_m_add; //�����PID ����ֵ���
   
   if(PID_m_add > 5000)// �ٶ�PWM ����2500 �Ͳ�������  2500Ϊ���
   {PID_m_add = 5000 ;}//��ֵ2500
    else if(PID_m_add< -5000){// +2500 ��-2500 �ķ���һ��������ת
    PID_m_add = -5000 ;}
   
   if(PID_m_add > 0)// �ٶ�PWM ������ Ϊһת��
   {

    FTM_CnV_REG(FTMN[0], FTM_CH1) = PID_m_add;  //  ������ת
    FTM_CnV_REG(FTMN[0], FTM_CH3) = 0 ;         //  ���ַ�ת
    FTM_CnV_REG(FTMN[0], FTM_CH2) = PID_m_add*((Turn-5400)*0.0006+1.0038);  // ������ת
    FTM_CnV_REG(FTMN[0], FTM_CH0) = 0 ;         // ���ַ�ת  
   } 
   else                     //�ٶ�PWM С���� Ϊ������ķ���
  {
    FTM_CnV_REG(FTMN[0], FTM_CH1) = 0;           //  ������ת
    FTM_CnV_REG(FTMN[0], FTM_CH3) = PID_m_add ;  //  ���ַ�ת
    FTM_CnV_REG(FTMN[0], FTM_CH2) = 0;           // ������ת
    FTM_CnV_REG(FTMN[0], FTM_CH0) = PID_m_add*((Turn-5400)*0.0006+1.0038) ;  // ���ַ�ת
  }                                         
}




