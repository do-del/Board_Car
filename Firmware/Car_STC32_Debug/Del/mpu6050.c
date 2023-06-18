#include "mpu6050.h"

void mpu6050_init(void)
{
	unsigned char dat[1];
	
	dat[0] = 0x00;
	I2C_Write(MPU6050_ADDR, 0x6B, dat, 1);
	
	dat[0] = 0x08;
	I2C_Write(MPU6050_ADDR, 0x1B, dat, 1);
	
	dat[0] = 0x10;
	I2C_Write(MPU6050_ADDR, 0x1C, dat, 1);
	
	dat[0] = 0x03;
	I2C_Write(MPU6050_ADDR, 0x1A, dat, 1);
}

void mpu6050_read(mpu6050_struct *mpu6050_data)
{
	unsigned char buf[14];
	
	I2C_Read(MPU6050_ADDR, 0x3B, buf, 14);
	mpu6050_data->acc_x = (buf[0]<<8) | (buf[1]);
	mpu6050_data->acc_y = (buf[2]<<8) | (buf[3]);
	mpu6050_data->acc_z = (buf[4]<<8) | (buf[5]);
	mpu6050_data->temperature = (buf[6]<<8) | (buf[7]);
	mpu6050_data->gyro_roll = (buf[8]<<8) | (buf[9]);
	mpu6050_data->gyro_pitch = (buf[10]<<8) | (buf[11]);
	mpu6050_data->gyro_yaw = (buf[12]<<8) | (buf[13]);
}