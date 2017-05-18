/********************南京理工大学泰州科技学院******************
                 * 文件名       ：Signal_Process_Algorithm.c
                 * 描述         ：算法包
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
   double Proportion;                        //定义位置式PID比例常数
 //double Integral;                          //定义位置式PID积分常数
   double Derivative;                        //定义位置式PID微分常数
   double Last_Error;                        //Error(k-1)  上一次差值
   double Current_Error;                     //Error(k)   当前差值
}Position_PID; 

/***********加权递推平均滤波*********/
void Filter() 
{ 
   char count;
   int i,j,temp; 
   /*数据采集存储变量清空*/
   Lean_Left=0,                 Lean_Right=0, 
   Outside_Left=0,              Outside_Right=0,
   Inside_Left=0,               Inside_Right=0;
   /*历史数据递推存储*/
   for(count=4;count>0;count--)
   {
       Outside_Left_Buffer[count]    =   Outside_Left_Buffer[count-1]; 
       Outside_Right_Buffer[count]   =   Outside_Right_Buffer[count-1]; 
       Inside_Left_Buffer[count]    =   Inside_Left_Buffer[count-1]; 
       Inside_Right_Buffer[count]   =   Inside_Right_Buffer[count-1];
       Lean_Left_Buffer[count]    =   Lean_Left_Buffer[count-1]; 
       Lean_Right_Buffer[count]   =   Lean_Right_Buffer[count-1];
   }
   /*新数据采样入队*/ 
   Outside_Left_Buffer[0]    =   adc_once(ADC0_SE13, ADC_8bit);  //b3
   Outside_Right_Buffer[0]   =   adc_once(ADC1_SE10, ADC_8bit);   //b4
        
   Inside_Left_Buffer[0]    =   adc_once(ADC0_SE9, ADC_8bit);    //b1
   Inside_Right_Buffer[0]   =   adc_once(ADC1_SE12, ADC_8bit);      //b6
                   
   Lean_Left_Buffer[0]  =   adc_once(ADC0_SE8, ADC_8bit);         //b0
   Lean_Right_Buffer[0] =   adc_once(ADC1_SE13, ADC_8bit);        //b7
   
  /*各缓冲区加权和位清零*/    
   Outside_Left_Buffer[6]    =  0;
   Outside_Right_Buffer[6]   =  0;
         
   Inside_Left_Buffer[6]     =  0;
   Inside_Right_Buffer[6]    =  0; 
       
   Lean_Left_Buffer[6]    =  0;
   Lean_Right_Buffer[6]   =  0;
   /*加权和存储*/
   for(count=0;count<5;count++)
   {
     Outside_Left_Buffer[6]+=(5-count)*Outside_Left_Buffer[count];
     Outside_Right_Buffer[6]+=(5-count)*Outside_Right_Buffer[count];
     
     Inside_Left_Buffer[6]+= (5-count)*Inside_Left_Buffer[count]; 
     Inside_Right_Buffer[6]+=(5-count)*Inside_Right_Buffer[count];
     
     Lean_Left_Buffer[6]+= (5-count)*Lean_Left_Buffer[count]; 
     Lean_Right_Buffer[6]+=(5-count)*Lean_Right_Buffer[count]; 
   }
   
   /*平均运算*/
   Outside_Left_Buffer[6]/=15;
   Outside_Right_Buffer[6]/=15;
     
   Inside_Left_Buffer[6]/=15; 
   Inside_Right_Buffer[6]/=15;
   
   Lean_Left_Buffer[6]/=15; 
   Lean_Right_Buffer[6]/=15;
   /*取出有效数据存入运算变量进行后续运算*/
    Outside_Left     =  Outside_Left_Buffer[6];
    Outside_Right    =  Outside_Right_Buffer[6]; 
       
    Inside_Left     =  Inside_Left_Buffer[6]; 
    Inside_Right    =  Inside_Right_Buffer[6];
    
    Lean_Left     =  Lean_Left_Buffer[6]; 
    Lean_Right    =  Lean_Right_Buffer[6];
    //AD采样值存储
    Filter_Finish_NumericalValue[0] = Outside_Left; 
    Filter_Finish_NumericalValue[1] = Outside_Right; 
    Filter_Finish_NumericalValue[2] = Inside_Left; 
    Filter_Finish_NumericalValue[3] = Inside_Right;
    Filter_Finish_NumericalValue[4] = Lean_Left;  
    Filter_Finish_NumericalValue[5] = Lean_Right;
}
//离差标准化样本采样函数运算
void Normalization()
{
   /*5次取样并滤波*/
   Filter();
   /*获取样本最大值数据*/
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
//偏差计算
extern float OutData[4] ;
int16 *Offset_Caculate()
{
   /*离差标准化*/
   Outside_Left     =   Outside_Left/Outside_Left_Max*100;
   Outside_Right    =   Outside_Right/Outside_Right_Max*100; 
       
   Inside_Left     =   Inside_Left/Inside_Left_Max*100; 
   Inside_Right    =   Inside_Right/Inside_Right_Max*100;
   
   Lean_Left     =   Lean_Left/Lean_Left_Max*100; 
   Lean_Right    =   Lean_Right/Lean_Right_Max*100; 
   //归一化后的值存储
   Filter_Finish_NumericalValue[0] = Outside_Left; 
   Filter_Finish_NumericalValue[1] = Outside_Right; 
   Filter_Finish_NumericalValue[2] = Inside_Left; 
   Filter_Finish_NumericalValue[3] = Inside_Right;
   Filter_Finish_NumericalValue[4] = Lean_Left;  
   Filter_Finish_NumericalValue[5] = Lean_Right;
   /*外侧电感加和运算*/
   Outside_Sum   =  Outside_Left+Outside_Right;
   //OutData[0] =Outside_Sum;
   Outside_Delta   =  fabs(Outside_Left-Outside_Right);
   //OutData[1] =Outside_Delta;
  /*内侧电感加和运算*/
   Inside_Sum = Inside_Left+Inside_Right;
   Inside_Delta =  Inside_Left-Inside_Right;
   /*八字电感加和差运算*/
   Lean_Sum = Lean_Left+Lean_Right;
   //OutData[2] = Lean_Sum;
   Lean_Delta=Lean_Left-Lean_Right;
   //OutData[3] = Lean_Delta;
  /*内侧电感偏差运算，带符号，与位置式PD代码正负物理意义相同*/
     if(Inside_Delta<5&&Inside_Delta>-5) Inside_Delta=0;
  /*内侧电感差比和运算，目的是为了消除角度影响 加1.0防止分母为0*/
     Inside_Delta =100*Inside_Delta/(1.0+Inside_Sum);    
  /*存储最近4次内侧电感偏差*/
         Inside_Pre4_Delta=Inside_Pre3_Delta;
         Inside_Pre3_Delta=Inside_Pre2_Delta;
         Inside_Pre2_Delta=Inside_Pre1_Delta;
         Inside_Pre1_Delta=Inside_Delta;
   return Filter_Finish_NumericalValue;  
} 


                           
//位置式PD控制器初始化
void Position_PID_Init()
{
   Position_PID.Proportion = 1;
   Position_PID.Derivative = 1;
}


//位置式PD控制器
uint16 Position_PID_Controller(int16 Offset)
{
   /*定义位置式PD控制器局部变量*/
   double P,D,Angle_Value; 
   
   /*更新每次的差值*/
   Position_PID.Current_Error=Offset; 
   
   /*位置式的P公式  Kp*E(k)*/
   P=Position_PID.Proportion*Position_PID.Current_Error;    
   
   /*位置式的D公式  Kd*[E(k)-E(k-1)]*/
   D=Position_PID.Derivative*(Position_PID.Current_Error-Position_PID.Last_Error);  
   
   /*位置式PD输出量，MINDTURN为舵机中心值*/
   Angle_Value=MIDVALUE+P+D; 
   
   /*前轮左转向限幅处理*/
   if(Angle_Value>LEFTMAX)   Angle_Value=LEFTMAX; 
   
   /*前轮右转向限幅处理*/
   if(Angle_Value<RIGHTMAX)  Angle_Value=RIGHTMAX; 
   
   /*更新每次的差值*/
   Position_PID.Last_Error=Position_PID.Current_Error; 
   
   /*返回舵机打角值*/ 
   return Angle_Value;
}


//三次代数方程的位置解算算法  f(x)=0.0033*(x.^3)+0.0272*(x.^2)-4.8804*x-4.8277-offset 求根区间[-20,21]
double Position()
{
  double x=0;
  extern uint16 Speed;
  extern uint8 buff[DATA_PACKET]; 
  static double Previous_Angle;
  //完全丢线
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
 //不完全丢线
 else
 {
   //全程
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
   //左转增加 右转减少  中值大约在5000  上下浮动不能6700
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




/*增量式PID结构体*/
//定义一个名为PID_M 的结构体
struct
{
  int16 Current_Error;  //当前差值   E(0)
  int16 Last_Error;     //上次差值   E(-1)
  int16 Prev_Error;     //上上次差值 E(-2)
}M_PID; 

//定义Kp、Ki、Kd 三个参数
float   
        M_Proportion =5,
        M_Integral =0,
        M_Derivative =0; 
//PID 的增量输出
uint16 PID_m_add ; 
extern float Speed_Set_Goal;
extern float OutData[4];
void Motor_Control(uint16 Speed)
{
   /*定义局部变量*/
   int16 P,I,D;
   /*更新每次的差值*/
   M_PID.Prev_Error=M_PID.Last_Error;
   /*更新每次的差值*/
   M_PID.Last_Error=M_PID.Current_Error;
    /*更新每次的差值*/
   M_PID.Current_Error=Speed_Set_Goal-Speed; 
   
   P=M_Proportion*(M_PID.Current_Error-M_PID.Last_Error); //比例P 输出公式                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
   I=M_Integral* M_PID.Current_Error; //积分I 输出公式
   D=M_Derivative*(M_PID.Current_Error+M_PID.Prev_Error-2*M_PID.Last_Error); //微分D 输出公式
   PID_m_add=P+I+D+PID_m_add; //电机的PID 增量值输出
   
   //OutData[1]=PID_m_add;
   if(PID_m_add>3000) PID_m_add=3000;
   //CH0>CH1 ,正转
   FTM_CnV_REG(FTMN[0], FTM_CH1) = 0;                   
   FTM_CnV_REG(FTMN[0], FTM_CH0) = PID_m_add*5*((Turn-5400)*0.0006+1.0038);
    
}
//printf("PID_m_add:%d\n",PID_m_add);


