/*
  ��ŵ�������㷨pid����

*/


/*
typedef struct{
double kp;			//��������
double ki;			//���ֲ���
double kd			//΢��ϵ��
double iLinmit;		//����������������ֵ
double currError;	//��ǰ�ĽǶ����
double prevError;	//ǰһ�νǶ����
double integValue;	//������ֵ
double deriv;		//΢����ֵ
}PID;

*/
#include"pid.h"

PID quadcopter;
void pid_init(void)
{


}



double  pid_cal(PID *pid ,double expectCorner,double currCorner)
{
	 pid->currError=currCorner-expectCorner;		//���㵱ǰ�Ƕ�ƫ��
	 pid->integValue+=pid->currError;
 	if (pid->integValue> pid->iLimit)
	  	{
  	  		pid->integValue = pid->iLimit;
 	 	}
 	 else if (pid->integValue < -pid->iLimit)
  	 	{
   			 pid->integValue = -pid->iLimit;
 	  	}
		pid->deriv = (pid->currError - pid->prevError) ;//���΢����
		pid->prevError=	pid->currError;

   return (pid->kp * pid->currError) +
          (pid->ki * pid->integValue) +
          (pid->kd * pid->deriv);
}












