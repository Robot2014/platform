/**
  ******************************************************************************
  * @file    sensor.c
  * @author  Chenjy   QQ:251534538  mail:chenjunyu@hodmct.com
  * @version V1.0
  * @date    30-March-2013
  * @brief   ����ļ�����Ŵ�������ת�������������
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

SENSOR_DAT		Acce_Dat,Gyro_Dat;				//���ٶȺ������ǵ�ת�����ֵ
SENSOR_DAT		Gyro_inte;						//�����ǻ��ֺ������
SENSOR_DAT		Acce_avg;						//�����ں�ʱ��Ҫ�õ����ݵļ��ٶȼ�ƽ��ֵ

SENSOR_DAT		Acce_avg_buffer[FILTER_NUM];	//�����˲�����ֵ����


/**
  * @brief   ��ȡmpu6050���ٶ����ݼ�ȥ���ݵ����Ư��ֵ���� ���ݵ�ת������ 
  * @param   None
  * @retval  None
**/
void Convert_Acce(void)
{
		Acce_Dat.x=(mpu6050_ramdata.Acce[0]-mpu6050_zero_offsent.Acce[0])*A_GAIN;	//ת���ɵ�Ԫ��ʽ
		Acce_Dat.y=(mpu6050_ramdata.Acce[1]-mpu6050_zero_offsent.Acce[1])*A_GAIN;
		Acce_Dat.z=(mpu6050_ramdata.Acce[2]-mpu6050_zero_offsent.Acce[2])*A_GAIN;
}

/**
  * @brief   ��ȡmpu6050���������ݼ�ȥ���ݵ����Ư��ֵ���� ���ݵ�ת������ 
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
  * @brief   ��ȡmpu6050���ٶ����ݽ��д����˲�
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
  * @brief   ��ȡmpu6050���ٶ����ݽ��л��������˲�
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
  * @brief   ��ȡ����������ֵ
  * @param   None
  * @retval  None
**/
void read_sensor(void)
{
   time_interval=CONVERT_SEC(Get_CurrentTime(&CurrentTick));			//��ȡ����ʱ��
   mpu6050_Read_data();													//��ȡmpu6050��6������
   Convert_Acce();														//ת�����ٶȼ�����
   Convert_Gyro();														//ת������������
   Gyro_integral(&Gyro_inte,time_interval);								//���������ݻ���

}








 






