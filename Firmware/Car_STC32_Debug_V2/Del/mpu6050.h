#ifndef __MPU6050_H
#define __MPU6050_H

#include "stc32g.h"
#include "intrins.h"
#include "stc.h"
#include "i2c.h"

#define MPU6050_ADDR 0x68

typedef struct
{
	short int acc_x;
	short int acc_y;
	short int acc_z;
	short int temperature;
	short int gyro_roll;
	short int gyro_pitch;
	short int gyro_yaw;
}mpu6050_struct;

void mpu6050_init(void);
void mpu6050_read(mpu6050_struct *mpu6050_data);

#endif  