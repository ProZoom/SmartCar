#ifndef _SIGNAL_PROCESS_ALGORITHM_H_
#define _SIGNAL_PROCESS_ALGORITHM_H_ 

#define MIDVALUE  5000
#define LEFTMAX   6700
#define RIGHTMAX  3300

void Filter();
void Normalization();
double Position();
uint16 Position_PID_Controller(int16 Offset);
void Position_PID_Init();
void Filter();
int16 *Offset_Caculate();
void Motor_Control(uint16 Speed);

#endif