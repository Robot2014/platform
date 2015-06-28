			  /**
  ******************************************************************************
  * @file    quaternion.c
  * @author  Corner Boy   QQ:251534538  mail:chenjunyu@hodmct.com
  * @version V1.0
  * @date    14-April-2013
  * @brief   This C file is 
  * 
  * @dagram	  
               
  ******************************************************************************
  * @attention
  * This file is free for all who is crazy of quadcopter_X4 
  * if you find and bug you can communication to me by QQ or email.
  * Thanks
  ******************************************************************************
**/
 #include<math.h>

#define Kp 10.0f                        
#define Ki 0.008f                         
#define halfT 0.5f
float exInt = 0, eyInt = 0, ezInt = 0;      // ��������С�������
float yaw,pitch,roll; 						//ƫ���ǣ������ǣ�������
void IMUupdate(float gx,float gy,float gz,float ax,float ay,float az)
{
	static float q0=1,q1=0,q2=0,q3=0;
	float norm;
	float vx,vy,vz;			/*�������Ҳ��� */
	float ex,ey,ez;			/* ��������ͷ��򴫸��������ο�����Ľ����*/
	float q0q0=q0*q0;
	float q0q1=q1*q1;
	float q0q2=q0*q2;
//	float q0q3=q0*q3;
	float q1q1=q1*q1;
//	float q1q2=q1*q2;
	float q1q3=q1*q3;
	float q2q2=q2*q2;
	float q2q3=q2*q3;
	float q3q3=q3*q3;

/*@! �����ٶȼ���ά����ת�ɵ�λ����  */
	norm= 1.0f/sqrt(ax*ax+ay*ay+az*az);
	ax=ax*norm;
	ay=ay*norm;
	az=az*norm;  
/**
**@!���ǰ���Ԫ������ɷ������Ҿ����еĵ����е�����Ԫ�ء�
**@!�������Ҿ����ŷ���ǵĶ��壬��������ϵ������������ת����������ϵ��������������Ԫ�ء�
**@!���������vx\y\z����ʵ���ǵ�ǰ��ŷ���ǣ�����Ԫ�����Ļ����������ϵ�ϣ����������������λ������
**/
	vx=2*(q1q3-q0q2);
	vy=2*(q2q3+q0q1);
	vz=q0q0-q1q1-q2q2+q3q3;

/* ���������  */	
	ex=ay*vz-az*vy;
	ey=ax*vz-az*vx;
	ez=ax*vx-ay*vy;

/*  ��������PI����������Ư��  */
	exInt = exInt + ex * Ki;								 
    eyInt = eyInt + ey * Ki;
    ezInt = ezInt + ez * Ki;

 	gx = gx + Kp*ex + exInt;					   							
  	gy = gy + Kp*ey + eyInt;
  	gz = gz + Kp*ez + ezInt;				   							

/*  ��Ԫ��΢�ַ��� */					   
  	q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
  	q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
  	q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
  	q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

/*	��Ԫ������  */
  	norm = sqrt(q0q0 + q1q1 + q2q2 + q3q3);
  	q0 = q0 / norm;
  	q1 = q1 / norm;
  	q2 = q2 / norm;
  	q3 = q3 / norm;
	pitch = asin(-2*q1q3+2*q0q2)*57.3; // pitch
 	roll  = atan2(2*q2q3+2*q0*q1,-2*q1q1-2*q2q2+1)*57.3; // roll
}


/*        �������Ǻͼ��ٶȼ����ݻ����˲�          */
void Complementary_Filter(void)
{




}





