#include "mpu6050.h"
I2C_HandleTypeDef MPU6050_hi2c;
uint16_t ACCEL_X_RAW=0;
uint16_t ACCEL_Y_RAW=0;
uint16_t ACCEL_Z_RAW=0;

uint16_t Gyro_X_RAW = 0;
uint16_t Gyro_Y_RAW = 0;
uint16_t Gyro_Z_RAW = 0;

uint16_t Temp_RAW = 0;
float Temp;
float A[3],G[3];

void MPU6050_init(I2C_HandleTypeDef Hi2c)
{
	HAL_Delay(1000);
	MPU6050_hi2c = Hi2c;
	uint8_t check,data;
	HAL_I2C_Mem_Read(&MPU6050_hi2c,MPU6050_ADDR,WHO_AM_I_REG,1,&check,1,1000);
	if (check==104)
	{
		//写入0来唤醒所有传感器
		data =0;
		HAL_I2C_Mem_Write(&MPU6050_hi2c,MPU6050_ADDR,PWR_MGMT_1_REG,1,&data,1,1000);
		//设置数据速率
		data=0x17;
		HAL_I2C_Mem_Write(&MPU6050_hi2c,MPU6050_ADDR,SMPLRT_DIV_REG,1,&data,1,1000);
		//在ACCEL_CONFIG配置寄存器中设置加速度计配置
		//XA_ST=0,YA_ST=0,ZA_ST=0,FS_SEL=0 ->+2g;
		data = 0x00;
		HAL_I2C_Mem_Write(&MPU6050_hi2c,MPU6050_ADDR,ACCEL_CONFIG_REG,1,&data,1,1000);
		//配置陀螺仪设置寄存器
		data = 0x00;
		HAL_I2C_Mem_Write(&MPU6050_hi2c,MPU6050_ADDR,GYRO_CONFIG_REG,1,&data,1,1000);
	}
}
//加速度读取函数
float * MPU6050_Read_Accel(void)
{
	uint8_t Rec_Data[6];
	HAL_I2C_Mem_Read(&MPU6050_hi2c,MPU6050_ADDR,ACCEL_XOUT_H_REG,1,Rec_Data,6,1000);
	ACCEL_X_RAW = (uint16_t) (Rec_Data[0]<<8 | Rec_Data[1]);
	ACCEL_Y_RAW = (uint16_t) (Rec_Data[2]<<8 | Rec_Data[3]);
	ACCEL_Z_RAW = (uint16_t) (Rec_Data[4]<<8 | Rec_Data[5]);
	A[0] = ACCEL_X_RAW/16384.0;
	A[1] = ACCEL_Y_RAW/16384.0;
	A[2] = ACCEL_Z_RAW/16384.0;
	return A;
}
//角速度读取函数
float * MPU6050_Read_Gyro(void)
{
	uint8_t Rec_Data[6];
	HAL_I2C_Mem_Read(&MPU6050_hi2c,MPU6050_ADDR,GYRO_XOUT_H_REG,1,Rec_Data,6,1000);
	Gyro_X_RAW = (uint16_t) (Rec_Data[0]<<8 | Rec_Data[1]);
	Gyro_Y_RAW = (uint16_t) (Rec_Data[2]<<8 | Rec_Data[3]);
	Gyro_Z_RAW = (uint16_t) (Rec_Data[4]<<8 | Rec_Data[5]);
	G[0] = Gyro_X_RAW/131.0;
	G[1] = Gyro_Y_RAW/131.0;
	G[2] = Gyro_Z_RAW/131.0;
	return G;
}
//温度读取函数
float MPU6050_Read_Temp(void)
{
	uint8_t Rec_Data[6];
	HAL_I2C_Mem_Read(&MPU6050_hi2c,MPU6050_ADDR,TEMP_OUT_H_REG,1,Rec_Data,6,1000);
	Temp_RAW = (uint16_t) (Rec_Data[0]<<8 | Rec_Data[1]);
	Temp = 36.53 + (Temp_RAW )/340.0;
	return Temp;
}