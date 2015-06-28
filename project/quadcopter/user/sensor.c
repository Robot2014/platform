/**
  ******************************************************************************
  * @file    sensor.c
  * @author  Chenjy   QQ:251534538  mail:chenjunyu@hodmct.com
  * @version V1.0
  * @date    30-March-2013
  * @brief   这个文件将存放传感器的转换后的最终数据
             The interface is SPI. 
  * @dagram	  
             stm32-----------------mpu6050
  ******************************************************************************
  * @attention
  * this file is free for all who is crazy of quadcopter 
  * if you find and bug you can communication to me by QQ or email.
  * Thanks
  ******************************************************************************
**/

#include"sensor.h"
#include"mpu6050.h"
#include"systick.h"
#define FILTER_NUM		10

SENSOR_DAT		Acce_Dat,Gyro_Dat;				//加速度和陀螺仪的转换后的值
SENSOR_DAT		Gyro_inte;						//陀螺仪积分后的数据
SENSOR_DAT		Acce_avg;						//数据融合时需要得到数据的加速度计平均值

SENSOR_DAT		Acce_avg_buffer[FILTER_NUM];	//滑动滤波窗口值缓存


/**
  * @brief   读取mpu6050加速度数据减去数据的零点漂移值乘以 数据的转换倍率 
  * @param   None
  * @retval  None
**/
void Convert_Acce(void)
{
		Acce_Dat.x=(mpu6050_ramdata.Acce[0]-mpu6050_zero_offsent.Acce[0])*A_GAIN;	//转换成单元格式
		Acce_Dat.y=(mpu6050_ramdata.Acce[1]-mpu6050_zero_offsent.Acce[1])*A_GAIN;
		Acce_Dat.z=(mpu6050_ramdata.Acce[2]-mpu6050_zero_offsent.Acce[2])*A_GAIN;
}

/**
  * @brief   读取mpu6050陀螺仪数据减去数据的零点漂移值乘以 数据的转换倍率 
  * @param   None
  * @retval  None
**/
void Convert_Gyro(void)
{
		Gyro_Dat.x=(mpu6050_ramdata.Gyro[0]-mpu6050_zero_offsent.Gyro[0])*G_GAIN;	
		Gyro_Dat.y=(mpu6050_ramdata.Gyro[1]-mpu6050_zero_offsent.Gyro[1])*G_GAIN;
		Gyro_Dat.z=(mpu6050_ramdata.Gyro[2]-mpu6050_zero_offsent.Gyro[2])*G_GAIN;
}

/**
  * @brief   读取mpu6050加速度数据进行窗口滤波
  * @param   None
  * @retval  None
**/
void Gyro_integral(SENSOR_DAT*	Gyro_inte, double integral_time)
{
		Gyro_inte->x=Gyro_inte->x+Gyro_Dat.x*integral_time;
		Gyro_inte->y=Gyro_inte->y+Gyro_Dat.y*integral_time;
		Gyro_inte->z=Gyro_inte->z+Gyro_Dat.z*integral_time;

}


/**
  * @brief   读取mpu6050加速度数据进行滑动窗口滤波
  * @param   None
  * @retval  None
**/
void Acce_avg_filter(void)
{
	 	static uint8_t filter_num=0;
		 uint8_t i=0;
		 SENSOR_DAT SUM;
		 Acce_avg_buffer[filter_num].x=Acce_Dat.x;
		 Acce_avg_buffer[filter_num].y=Acce_Dat.y;
		 Acce_avg_buffer[filter_num].z=Acce_Dat.z;
		 for(i=0;i<FILTER_NUM;i++)
		 	{
          	 	SUM.x=Acce_avg_buffer[i].x;
		   		SUM.y=Acce_avg_buffer[i].y;
		   		SUM.z=Acce_avg_buffer[i].z;
			 }
			Acce_avg.x=SUM.x/FILTER_NUM;
			Acce_avg.y=SUM.y/FILTER_NUM;
			Acce_avg.z=SUM.z/FILTER_NUM;
			filter_num++;
			if(filter_num>FILTER_NUM)
			filter_num=0;
}

/**
  * @brief   读取传感器器的值
  * @param   None
  * @retval  None
**/
void read_sensor(void)
{
   time_interval=CONVERT_SEC(Get_CurrentTime(&CurrentTick));			//获取积分时间
   mpu6050_Read_data();													//获取mpu6050的6轴数据
   Convert_Acce();														//转换加速度计数据
   Convert_Gyro();														//转换陀螺仪数据
   Gyro_integral(&Gyro_inte,time_interval);								//陀螺仪数据积分

}








 






