#ifndef _BMX055_H_
#define _BMX055_H_
#include "main.h"

// BMX055 Gyroscope Registers
#define GYRO_WHOAMI           0x00  // should return 0x0F
// #define GYRO_Reserved       0x01
#define GYRO_RATE_Z_LSB       0x06
#define GYRO_RATE_Z_MSB       0x07
#define GYRO_RANGE            0x0F

// Seven-bit device addresses are ACC = 0x19, GYRO = 0x69
#define GYRO_ADDRESS 0x69  // Address of BMX055 gyroscope

// Set initial input parameters
#define GFS_125DPS 0x04
#define GYRO_FACTOR 262.144  // 32768 / 125

uint8_t read_byte(uint8_t reg);
void write_byte(uint8_t reg, uint8_t data);

void GyroInit();
void GyroOffsetCalc();
void UpdateGyroData();

#endif // _BMX055_H_
