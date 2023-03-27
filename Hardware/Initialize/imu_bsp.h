#ifndef _IMU_BSP_H_
#define _IMU_BSP_H_

#include "main.h"

#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

#define MPU_DELAY(x) delay_ms(x)

//2G
#define ACCEL_SEN 0.00478515625f * 2
//1000
#define GYRO_SEN 0.0005326322180158476492076f

#define MAG_SEN 0.3f //×ª»»³É uT

//#define MAG_SEN 1

typedef struct
{
	int16_t ax;
	int16_t ay;
	int16_t az;

	int16_t mx;
	int16_t my;
	int16_t mz;

	int16_t temp;

	int16_t gx;
	int16_t gy;
	int16_t gz;
	
	int16_t ax_offset;
	int16_t ay_offset;
	int16_t az_offset;

	int16_t gx_offset;
	int16_t gy_offset;
	int16_t gz_offset;
} mpu_data_t;

typedef struct
{
	float ax;
	float ay;
	float az;
	
	float mx;
	float my;
	float mz;	
	
	float gx;
	float gy;
	float gz;	
	
} mpu_data_get_t;


typedef struct
{
	
	uint32_t accel[3];
	uint32_t gyro[3];	
	uint32_t mag[3];		
	
} Mpu6500_Ist8310_Data_t;




typedef struct
{
	int16_t ax;
	int16_t ay;
	int16_t az;

	int16_t mx;
	int16_t my;
	int16_t mz;

	float temp;

	float wx; /*!< omiga, +- 2000dps => +-32768  so gx/16.384/57.3 =	rad/s */
	float wy;
	float wz;

	float vx;
	float vy;
	float vz;

	float rol;
	float pit;
	float yaw;
} imu_t;

extern mpu_data_t mpu_data;
extern imu_t      imu;

uint8_t   mpu_device_init(void);
uint8_t ist83100_initt(void);
void init_quaternion(void);
void mpu_get_data(void);
void imu_ahrs_update(void);
void imu_attitude_update(void);
void mpu_offset_call(void);






#endif

