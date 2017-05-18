#ifndef _SIGNAL_PROCESS_ALGORITHM_H_
#define _SIGNAL_PROCESS_ALGORITHM_H_ 

#define MIDVALUE  5400
#define LEFTMAX   6000
#define RIGHTMAX  4800

void Filter();
void Normalization();
double Position();
uint16 Position_PID_Controller(int16 Offset);
void Position_PID_Init();
void Filter();
int16 *Offset_Caculate();
void Motor_Control(uint16 Speed);

#endif