#include"pid.h"
#include"quadcopter.h"
#include"motor.h"
#include"time_pwm.h"


QUADCOPTER	QuadCopter;	 

void quadcopter_conctorl(double PID_pitch,double PID_roll,double PID_yaw)
{

// angle_convert(&PID_pitch,&PID_roll,&PID_yaw);	//角度倾斜按照机体坐标转换
// int motor1,motor2,motor3,motor4;
/*
1号电机=悬停油门-前倾量+右倾量+旋转量；
2号电机=悬停油门-前倾量-右倾量-旋转量；
3号电机=悬停油门+前倾量+右倾量-旋转量；
4号电机=悬停油门+前倾量-右倾量+旋转量。
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

