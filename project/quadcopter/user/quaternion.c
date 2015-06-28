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
float exInt = 0, eyInt = 0, ezInt = 0;      // 按比例缩小积分误差
float yaw,pitch,roll; 						//偏航角，俯仰角，翻滚角
void IMUupdate(float gx,float gy,float gz,float ax,float ay,float az)
{
	static float q0=1,q1=0,q2=0,q3=0;
	float norm;
	float vx,vy,vz;			/*方向余弦参数 */
	float ex,ey,ez;			/* 错误领域和方向传感器测量参考方向的交叉积*/
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

/*@! 将加速度计三维向量转成单位向量  */
	norm= 1.0f/sqrt(ax*ax+ay*ay+az*az);
	ax=ax*norm;
	ay=ay*norm;
	az=az*norm;  
/**
**@!这是把四元数换算成方向余弦矩阵中的第三列的三个元素。
**@!根据余弦矩阵和欧拉角的定义，地理坐标系的重力向量，转到机体坐标系，正好是这三个元素。
**@!所以这里的vx\y\z，其实就是当前的欧拉角（即四元数）的机体坐标参照系上，换算出来的重力单位向量。
**/
	vx=2*(q1q3-q0q2);
	vy=2*(q2q3+q0q1);
	vz=q0q0-q1q1-q2q2+q3q3;

/* 计算叉积误差  */	
	ex=ay*vz-az*vy;
	ey=ax*vz-az*vx;
	ez=ax*vx-ay*vy;

/*  叉积误差做PI修正陀螺仪漂移  */
	exInt = exInt + ex * Ki;								 
    eyInt = eyInt + ey * Ki;
    ezInt = ezInt + ez * Ki;

 	gx = gx + Kp*ex + exInt;					   							
  	gy = gy + Kp*ey + eyInt;
  	gz = gz + Kp*ez + ezInt;				   							

/*  四元数微分方程 */					   
  	q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
  	q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
  	q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
  	q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

/*	四元数量化  */
  	norm = sqrt(q0q0 + q1q1 + q2q2 + q3q3);
  	q0 = q0 / norm;
  	q1 = q1 / norm;
  	q2 = q2 / norm;
  	q3 = q3 / norm;
	pitch = asin(-2*q1q3+2*q0q2)*57.3; // pitch
 	roll  = atan2(2*q2q3+2*q0*q1,-2*q1q1-2*q2q2+1)*57.3; // roll
}


/*        将陀螺仪和加速度计数据互补滤波          */
void Complementary_Filter(void)
{




}





