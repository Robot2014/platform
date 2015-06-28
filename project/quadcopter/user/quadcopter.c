#include"pid.h"
#include"quadcopter.h"
#include"motor.h"
#include"time_pwm.h"


QUADCOPTER	QuadCopter;	 

void quadcopter_conctorl(double PID_pitch,double PID_roll,double PID_yaw)
{

// angle_convert(&PID_pitch,&PID_roll,&PID_yaw);	//�Ƕ���б���ջ�������ת��
// int motor1,motor2,motor3,motor4;
/*
1�ŵ��=��ͣ����-ǰ����+������+��ת����
2�ŵ��=��ͣ����-ǰ����-������-��ת����
3�ŵ��=��ͣ����+ǰ����+������-��ת����
4�ŵ��=��ͣ����+ǰ����-������+��ת����
*/
/*motor1=basicSpeed-roll+pitch+yaw;
 motor2=basicSpeed-roll-pitch-yaw;
 motor3=basicSpeed+roll+pitch-yaw;
 motor4=basicSpeed+roll-pitch+yaw;
 set_pwm(motor1,motor2,motor3,motor4);	  */

}

void QuadCopter_init(QUADCOPTER *Q)
{
 Q->Status=Q_OFF;
 Q->BaseSpeed=300;
 Q->Scale=100;
 Q->MaxSpeed=TIME2_PERIOD;

}

void	QuadCopter_up(QUADCOPTER*	QuadCopter)
{
 	if(QuadCopter->Status==Q_OFF)
	return;
	QuadCopter->BaseSpeed+=QuadCopter->Scale;
	if(	QuadCopter->BaseSpeed>QuadCopter->MaxSpeed)
	{
	   	QuadCopter->BaseSpeed=QuadCopter->MaxSpeed;

	}
	motor_speed(QuadCopter->BaseSpeed,QuadCopter->BaseSpeed,QuadCopter->BaseSpeed,QuadCopter->BaseSpeed ) ;

}

void	QuadCopter_down(QUADCOPTER*	QuadCopter)
{

	 if(QuadCopter->Status==Q_OFF)
	return;
	if(	QuadCopter->BaseSpeed<QuadCopter->Scale)
	{
			QuadCopter->BaseSpeed=0;
	} else
	{
	QuadCopter->BaseSpeed-=QuadCopter->Scale;
	}
	motor_speed(QuadCopter->BaseSpeed,QuadCopter->BaseSpeed,QuadCopter->BaseSpeed,QuadCopter->BaseSpeed ) ;
}

