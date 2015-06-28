/*
  存放电机控制算法pid部分

*/


/*
typedef struct{
double kp;			//比例参数
double ki;			//积分参数
double kd			//微分系数
double iLinmit;		//积分项的最大积累误差值
double currError;	//当前的角度误差
double prevError;	//前一次角度误差
double integValue;	//积分项值
double deriv;		//微分项值
}PID;

*/
#include"pid.h"

PID quadcopter;
void pid_init(void)
{


}



double  pid_cal(PID *pid ,double expectCorner,double currCorner)
{
	 pid->currError=currCorner-expectCorner;		//计算当前角度偏差
	 pid->integValue+=pid->currError;
 	if (pid->integValue> pid->iLimit)
	  	{
  	  		pid->integValue = pid->iLimit;
 	 	}
 	 else if (pid->integValue < -pid->iLimit)
  	 	{
   			 pid->integValue = -pid->iLimit;
 	  	}
		pid->deriv = (pid->currError - pid->prevError) ;//求出微分项
		pid->prevError=	pid->currError;

   return (pid->kp * pid->currError) +
          (pid->ki * pid->integValue) +
          (pid->kd * pid->deriv);
}












