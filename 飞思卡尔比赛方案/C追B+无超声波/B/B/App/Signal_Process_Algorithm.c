/********************�Ͼ�����ѧ̩�ݿƼ�ѧԺ******************
                 * �ļ���       ��Signal_Process_Algorithm.c
                 * ����         ���㷨��
***************************************************************/

#include "common.h"
#include "MK60_adc.h" 
#include "Signal_Process_Algorithm.h"
#include  "MK60_FTM.h"
#include "MK60_uart.h"
#include "include.h"

#define N 10 
uint16 Lean_Left_Buffer[N],      Lean_Right_Buffer[N],  
       Outside_Left_Buffer[N],   Outside_Right_Buffer[N],
       Inside_Left_Buffer[N],    Inside_Right_Buffer[N],
       
       Lean_Left,              Lean_Right, 
       Outside_Left,           Outside_Right,
       Inside_Left,            Inside_Right,
       Filter_Sum;

double  Lean_Left_Max,           Lean_Right_Max, 
        Outside_Left_Max=190,        Outside_Right_Max=190,
        Inside_Left_Max=190,         Inside_Right_Max=190;
              
int16 Filter_Finish_NumericalValue[20]={0};
double Outside_Delta,Inside_Delta,Outside_Sum,Inside_Sum,Lean_Sum,
        Outside_Pre1_Delta,Inside_Pre1_Delta,
        Outside_Pre2_Delta,Inside_Pre2_Delta,
        Outside_Pre3_Delta,Inside_Pre3_Delta,
        Outside_Pre4_Delta,Inside_Pre4_Delta,
        Lean_Delta;
      
char Outside_Lose_Track_Flag=0,Left_Lose=0,Right_Lose=0,All_Outside_Lose_Track_Flag=0;
double Last_Angel,Turn;
double Offset_Buffer[100]; 
char Turn_Left_Flag_1=0,Turn_Right_Flag_1=0,Turn_Left_Flag_2=0,Turn_Right_Flag_2=0;
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
   int i,j,temp; 
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
   Outside_Left_Buffer[0]    =   adc_once(ADC0_SE13, ADC_8bit);  //b3
   Outside_Right_Buffer[0]   =   adc_once(ADC1_SE10, ADC_8bit);   //b4
        
   Inside_Left_Buffer[0]    =   adc_once(ADC0_SE9, ADC_8bit);    //b1
   Inside_Right_Buffer[0]   =   adc_once(ADC1_SE12, ADC_8bit);      //b6
                   
   Lean_Left_Buffer[0]  =   adc_once(ADC0_SE8, ADC_8bit);         //b0
   Lean_Right_Buffer[0] =   adc_once(ADC1_SE13, ADC_8bit);        //b7
   
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
       
    Inside_Left     =  Inside_Left_Buffer[6]; 
    Inside_Right    =  Inside_Right_Buffer[6];
    
    Lean_Left     =  Lean_Left_Buffer[6]; 
    Lean_Right    =  Lean_Right_Buffer[6];
    //AD����ֵ�洢
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
//ƫ�����
extern float OutData[4] ;
int16 *Offset_Caculate()
{
   /*����׼��*/
   Outside_Left     =   Outside_Left/Outside_Left_Max*100;
   Outside_Right    =   Outside_Right/Outside_Right_Max*100; 
       
   Inside_Left     =   Inside_Left/Inside_Left_Max*100; 
   Inside_Right    =   Inside_Right/Inside_Right_Max*100;
   
   Lean_Left     =   Lean_Left/Lean_Left_Max*100; 
   Lean_Right    =   Lean_Right/Lean_Right_Max*100; 
   //��һ�����ֵ�洢
   Filter_Finish_NumericalValue[0] = Outside_Left; 
   Filter_Finish_NumericalValue[1] = Outside_Right; 
   Filter_Finish_NumericalValue[2] = Inside_Left; 
   Filter_Finish_NumericalValue[3] = Inside_Right;
   Filter_Finish_NumericalValue[4] = Lean_Left;  
   Filter_Finish_NumericalValue[5] = Lean_Right;
   /*����мӺ�����*/
   Outside_Sum   =  Outside_Left+Outside_Right;
   //OutData[0] =Outside_Sum;
   Outside_Delta   =  fabs(Outside_Left-Outside_Right);
   //OutData[1] =Outside_Delta;
  /*�ڲ��мӺ�����*/
   Inside_Sum = Inside_Left+Inside_Right;
   Inside_Delta =  Inside_Left-Inside_Right;
   /*���ֵ�мӺͲ�����*/
   Lean_Sum = Lean_Left+Lean_Right;
   //OutData[2] = Lean_Sum;
   Lean_Delta=Lean_Left-Lean_Right;
   //OutData[3] = Lean_Delta;
  /*�ڲ���ƫ�����㣬�����ţ���λ��ʽPD������������������ͬ*/
     if(Inside_Delta<5&&Inside_Delta>-5) Inside_Delta=0;
  /*�ڲ��в�Ⱥ����㣬Ŀ����Ϊ�������Ƕ�Ӱ�� ��1.0��ֹ��ĸΪ0*/
     Inside_Delta =100*Inside_Delta/(1.0+Inside_Sum);    
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
   Position_PID.Derivative = 1;
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
double Position()
{
  double x=0;
  extern uint16 Speed;
  extern uint8 buff[DATA_PACKET]; 
  static double Previous_Angle;
  //��ȫ����
 if(Inside_Left<40&&Inside_Right<40&&Lean_Left<40&&Lean_Right<40)
 {
    x=Previous_Angle;
    Turn=Position_PID_Controller(x);
    if(Turn-5300>=0)
       Turn=6700;
    else
      if(Turn-4700<=0)
       Turn=3300;
 }
 //����ȫ����
 else
 {
   //ȫ��
   if(Inside_Sum>70)
     {
       if(Inside_Sum>110)
       {
          Position_PID.Proportion = 1;
       }
       else if(Inside_Sum>105)
       {
          Position_PID.Proportion = 1.2;
       }
       else if(Inside_Sum>100)
       {
          Position_PID.Proportion = 1.6;
       }
       else if(Inside_Sum>95)
       {
          Position_PID.Proportion = 2;
       }
       else if(Inside_Sum>90)
       {
          Position_PID.Proportion = 2.4;
       }
       else if(Inside_Sum>85)
       {
          Position_PID.Proportion = 2.8;
       }
       else if(Inside_Sum>80)
       {
          Position_PID.Proportion = 3.2;
       }
       else if(Inside_Sum>75)
       {
          Position_PID.Proportion = 3.6;
       }
        else if(Inside_Sum>70)
       {
          Position_PID.Proportion = 4.0;
       } else if(Inside_Sum>65)
       {
          Position_PID.Proportion = 4.4;
       } else if(Inside_Sum>60)
       {
          Position_PID.Proportion = 4.8;
       } else if(Inside_Sum>55)
       {
          Position_PID.Proportion = 5.2;
       }
        else if(Inside_Sum>50)
       {
          Position_PID.Proportion = 5.6;
       }
        else if(Inside_Sum>45)
       {
          Position_PID.Proportion = 5.6;
       }
        else if(Inside_Sum>40)
       {
          Position_PID.Proportion = 6.0;
       }
        else if(Inside_Sum>35)
       {
          Position_PID.Proportion = 7;
       }
       else if(Inside_Sum>30)
       {
          Position_PID.Proportion = 9;
       }
       else
       {
          Position_PID.Proportion = 10;
       }
       Inside_Delta=0.6*Inside_Delta+0.4*(Inside_Pre1_Delta-Inside_Pre4_Delta);
       //4.091e-005     -0.1012     -0.252
       x=100*(0.00004091*Inside_Delta*Inside_Delta-0.1012*Inside_Delta-0.252);
       //printf("x:%d\n",x);
       Previous_Angle=x;
       Turn=Position_PID_Controller(x);
       }
   //��ת���� ��ת����  ��ֵ��Լ��5000  ���¸�������6700
   else if(Lean_Sum<110)
     {  
        Position_PID.Proportion = 20;
     //  -0.00016196    -0.058079        10.31
       if(Lean_Left<Lean_Right)
       {
         x=-400*(-0.00016196*Inside_Sum*Inside_Sum-0.058079*Inside_Sum-10.31);
         Previous_Angle=x;
       }
       else
       {
         x=400*(-0.00016196*Inside_Sum*Inside_Sum-0.058079*Inside_Sum-10.31);
         Previous_Angle=x;
       }
         Turn=Position_PID_Controller(x);
       }
 }
     FTM_CnV_REG(FTMN[1], FTM_CH1)=Turn;  
}




/*����ʽPID�ṹ��*/
//����һ����ΪPID_M �Ľṹ��
struct
{
  int16 Current_Error;  //��ǰ��ֵ   E(0)
  int16 Last_Error;     //�ϴβ�ֵ   E(-1)
  int16 Prev_Error;     //���ϴβ�ֵ E(-2)
}M_PID; 

//����Kp��Ki��Kd ��������
float   
        M_Proportion =5,
        M_Integral =0,
        M_Derivative =0; 
//PID ���������
uint16 PID_m_add ; 
extern float Speed_Set_Goal;
extern float OutData[4];
void Motor_Control(uint16 Speed)
{
   /*����ֲ�����*/
   int16 P,I,D;
   /*����ÿ�εĲ�ֵ*/
   M_PID.Prev_Error=M_PID.Last_Error;
   /*����ÿ�εĲ�ֵ*/
   M_PID.Last_Error=M_PID.Current_Error;
    /*����ÿ�εĲ�ֵ*/
   M_PID.Current_Error=Speed_Set_Goal-Speed; 
   
   P=M_Proportion*(M_PID.Current_Error-M_PID.Last_Error); //����P �����ʽ                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
   I=M_Integral* M_PID.Current_Error; //����I �����ʽ
   D=M_Derivative*(M_PID.Current_Error+M_PID.Prev_Error-2*M_PID.Last_Error); //΢��D �����ʽ
   PID_m_add=P+I+D+PID_m_add; //�����PID ����ֵ���
   
   //OutData[1]=PID_m_add;
   if(PID_m_add>3000) PID_m_add=3000;
   //CH0>CH1 ,��ת
   FTM_CnV_REG(FTMN[0], FTM_CH1) = 0;                   
   FTM_CnV_REG(FTMN[0], FTM_CH0) = PID_m_add*5*((Turn-5400)*0.0006+1.0038);
    
}
//printf("PID_m_add:%d\n",PID_m_add);


