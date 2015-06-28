/*
 * (C) Copyright 2014  ShenZhen China
 *  Jundi.chen Software Engineering, <chenjundi0808@gmail.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 CHINA
 */

#include"led.h"
#include"mpu6050.h"
#include"i2c.h"
#include"stdint.h"
#include"uart.h"
#include<stdlib.h>	
#include<stdio.h>
#include<string.h>

static void Delayms(int m)
{
  u32 i;
  
  for(; m != 0; m--)	
       for (i=0; i<50000; i++);
}	

MPU6050_RAM mpu6050_ramdata;			//mpu6050���ڲ���������
MPU6050_RAM mpu6050_zero_offsent;		//mpu6050���������Ư��

/**
  * @brief   write an single Byte data to MPU6050 inside register by I2C  
  * @param   mpu6050_reg: internert register address  write_data
  * @retval  None
**/
void mpu6050_write_reg(unsigned char mpu6050_reg,unsigned char write_data)
{
	i2c_write_byte(MPU6050_DEV_ADDR <<1,mpu6050_reg,write_data);
}

/**
  * @brief   write an buffer data to MPU6050 inside register by I2C  
  * @param   mpu6050_reg
  * @retval  None
**/
void mpu6050_write_buffer(unsigned char mpu6050_reg,unsigned char *buffer , unsigned char length)
{
 	i2c_write_buffer(MPU6050_DEV_ADDR <<1,mpu6050_reg,buffer,length);
}


/**
  * @brief   read an byte data from mpu6050 inside register  
  * @param   mpu6050_reg 
  * @retval  None
**/
unsigned char mpu6050_read_reg(unsigned char mpu6050_reg )
{	 
	return  i2c_read_byte(MPU6050_DEV_ADDR<<1, mpu6050_reg );
}

/**
  * @brief   read length data from reg
  * @param   mpu6050_reg 
  * @retval  None
**/
void mpu6050_read_buffer(unsigned char mpu6050_reg,unsigned char *buffer,unsigned char length)
{
	i2c_read_buffer(MPU6050_DEV_ADDR<<1, mpu6050_reg ,buffer, length);
}

/**
  * @brief   check if connecte to mpu6050  
  * @param   mpu6050_reg 
  * @retval  None
**/
bool mpu6050_check_connection(void )
{
    unsigned char check_counter = 0 ;
    while( check_counter++ < 20 )
    {
        if(mpu6050_read_reg(MPU6050_ID_ADDR) == MPU6050_DEV_DEFAULT)
        return TRUE;
    }
    
    return  FALSE;
}

/**
  * @brief   init mpu6050 work mode power mode
  * @param   None
  * @retval  TRUE  FALSE 
**/
bool mpu6050_init(void )
{
	i2c_init();
 
	Delayms(10);
	if( mpu6050_check_connection() != TRUE )
	{
	    printf("mpu6050_check_connection fail \n");
        return FALSE;
    }        
	mpu6050_write_reg(PWR_CFG_ADDR,0x00);        	  //���õ�Դ����ģʽ �˳����ߺ�ѭ��ģʽ ʱ��ԴΪ�����ǵ�X��

    mpu6050_write_reg(SMPLING_RATE_ADDR, 0x04);  	  //���������ǵĲ����ʷ�Ƶϵ�� Sample Rate = Gyroscope Output Rate / (1 +  SMPLRT_DIV)
	mpu6050_write_reg(FILTER_CFG_ADDR,0x02);          //�������ݺ��ⲿ����ͬ���ϳ� ���õ�ͨ�˲�����Ƶ�ʣ����ֵ�ͨ�˲�����Ϊ100HZ
	mpu6050_write_reg(GYRO_CFG_ADDR ,3<<3);			  //�����ǵ����� Ϊ2000degree/s		 16.4 LSB/degree/s
	mpu6050_write_reg(ACCEL_CFG_ADDR ,2<<3);		  //���ٶȼ����� Ϊ+-8g		         4096 LSB/g
	mpu6050_write_reg(INT_PIN_CFG ,0x32);		      //�ж��ź�Ϊ�ߵ�ƽ�����������ֱ���ж�ȡ�����ж��źŲ���ʧ��ֱͨ����I2C
	mpu6050_write_reg(INT_ENABLE ,0x1)	;             //����׼�������ж�
	mpu6050_write_reg(USER_CTRL,0x00)  ;              //��ʹ�ø���I2C
   	return TRUE;
}

/**
  * @brief   һ�ζ�ȡmpu6050�ڲ���3�������� ������ٶȼ����ݺ��¶�����  
  * @param   mpu6050_reg : MPU6050���ڲ��Ĵ�����ַ��write_data ��һ���ֽڵ�����
  * @retval  None
**/
void mpu6050_read_data(void )
{	   
    unsigned char i=0;
    unsigned char length=sizeof(mpu6050_ramdata);
    unsigned char tmp;
    unsigned char *p= (unsigned char *)&mpu6050_ramdata;
    mpu6050_read_buffer(MPU6050_RAMDATA_START,p,length)	;

    for(i=0;i<length/2;i++)
    {
        tmp = p[2*i];
        p[2*i] = p[2*i + 1];
        p[2*i + 1] = tmp;
    }
}

/**
  * @brief   ��ȡmpu6050�ڲ��¶�����ת����ʵ���¶�  
  * @param   None
  * @retval  None
**/
double mpu6050_temperature(void )
{	   
      return (mpu6050_ramdata.Temp/340+36.53);
}

/**
  * @brief   
  * @param   
  * @retval  
**/
unsigned char mpu6050_read_zero_offsent(void)
{
  static unsigned char check_num=0;
  static unsigned int Acce_offsent_X  ,	 Acce_offsent_Y , Acce_offsent_Z;
  static unsigned int Gyro_offsent_X  ,	 Gyro_offsent_Y , Gyro_offsent_Z;
  if(check_num==0)
  {
	 memset((int8_t *)&mpu6050_zero_offsent,0,14);
   }
   if(check_num<200)
   {
	mpu6050_read_data();						//��һ��mpu6050�ڲ�����
	Acce_offsent_X +=	mpu6050_ramdata.Acce[0];
    Acce_offsent_Y +=	mpu6050_ramdata.Acce[1];
	Acce_offsent_Z +=	mpu6050_ramdata.Acce[2];
	Gyro_offsent_X +=	mpu6050_ramdata.Gyro[0];
    Gyro_offsent_Y +=	mpu6050_ramdata.Gyro[1];
	Gyro_offsent_Z +=	mpu6050_ramdata.Gyro[2];
	check_num++;
	printf("%d	%d	%d	%d	%d	%d\n\r",mpu6050_ramdata.Acce[0],mpu6050_ramdata.Acce[1],mpu6050_ramdata.Acce[2],
	mpu6050_ramdata.Gyro[0] ,mpu6050_ramdata.Gyro[1],mpu6050_ramdata.Gyro[2]);
	return FALSE;
   }
    check_num=0	 ;
	mpu6050_zero_offsent.Acce[0] =Acce_offsent_X /check_num;
    mpu6050_zero_offsent.Acce[1] =Acce_offsent_Y /check_num;
	mpu6050_zero_offsent.Acce[2] =Acce_offsent_Z /check_num;

	mpu6050_zero_offsent.Gyro[0] =Gyro_offsent_X /check_num;
    mpu6050_zero_offsent.Gyro[1] =Gyro_offsent_Y /check_num;
	mpu6050_zero_offsent.Gyro[2] =Gyro_offsent_Z /check_num;
	return TRUE;
}


