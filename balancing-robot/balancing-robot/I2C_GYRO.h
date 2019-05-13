/*
 * I2C_GYRO.h
 *
 * Created: 2018-12-25 14:20:05
 *  Author: Jan
 */ 

#include <avr/io.h>

#ifndef I2C_GYRO_H_
#define I2C_GYRO_H_
/* GYRO Pololu L3GD20H
#define GYRO_ADDRESS 0xD6

#define CTRL1 0x20
#define CTRL2 0x21
#define CTRL3 0x22
#define CTRL4 0x23
#define CTRL5 0x24
#define REFERENCE 0x25
#define OUT_TEMP 0x26
#define STATUS 0x27
#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D
#define FIFO_CTRL 0x2E
#define FIFO_SCR 0x2F
#define IG_CFG 0x30
#define IG_SRC 0x31
#define IG_THS_XH 0x32
#define IG_THS_XL 0x33
#define IG_THS_YH 0x34
#define IG_THS_YL 0x35 
#define IG_THS_ZH 0x36
#define IG_THS_ZL 0x37
#define IG_DURATION 0x38
#define LOW_ODR 0x39
*/

// LSM6DS33 GYRO AND ACC
#define GYRO_ADDRESS 0xD6

#define FUNC_CFG_ACCESS 0x01
#define FIFO_CTRL1 0x06
#define FIFO_CTRL2 0x07
#define FIFO_CTRL3 0x08
#define FIFO_CTRL4 0x09
#define FIFO_CTRL5 0x0A
#define ORIENT_CFG_G 0x0B
#define INT1_CTRL 0x0D
#define INT2_CTRL 0x0E
#define WHO_AM_I 0x0F
#define CTRL1_XL 0x10
#define CTRL2_G 0x11
#define CTRL3_C 0x12
#define CTRL4_C 0x13
#define CTRL5_C 0x14
#define CTRL6_C 0x15
#define CTRL7_G 0x16
#define CTRL8_XL 0x17
#define CTRL9_XL 0x18
#define CTRL10_C 0X19
#define WAKE_UP_SRC 0x1B
#define TAP_SCR 0x1C
#define D6D_SRC 0x1D
#define STATUS_REG 0x1E
#define OUT_TEMP_L 0x20
#define OUT_TEMP 0x21
#define OUTX_L_G 0x22
#define OUTX_H_G 0x23
#define OUTY_L_G 0x24
#define OUTY_H_G 0x25
#define OUTZ_L_G 0x26
#define OUTZ_H_G 0x27
#define OUTX_L_XL 0x28
#define OUTX_H_XL 0x29
#define OUTY_L_XL 0x2A
#define OUTY_H_XL 0x2B
#define OUTZ_L_XL 0x2C
#define OUTZ_H_XL 0x2D
#define FIFO_STATUS1 0x3A
#define FIFO_STATUS2 0x3B
#define FIFO_STATUS3 0x3C
#define FIFO_STATUS4 0x4D
#define FIFO_DATA_OUT_L 0x3E
#define FIFO_DATA_OUT_H 0x3F
#define TIMESTAMP0_REG 0x40
#define TIMESTAMP1_REG 0x41
#define TIMESTAMP2_REG 0x42
#define STEP_TIMESTAMP_L 0x49h
#define STEP_TIMESTAMP_H 0x4A
#define STEP_COUNTER_L 0x4B
#define STEP_COUNTER_H 0x4C
#define FUNC_SRC 0x53
#define TAP_CFG 0x58
#define TAP_THS_6D 0x59
#define INT_DUR2 0x5A
#define WAKE_UP_THS 0x5B
#define WAKE_UP_DUR 0x5C
#define FREE_FALL 0x5D
#define MD1_CFG 0x5E
#define MD2_CFG 0x5F


int16_t AccX, AccY, AccZ;
int16_t GyroX, GyroY, GyroZ;

void I2C_Init();

void I2C_Start();	// transmisja START z oczekiwaniem na potwierdzenie
void I2C_Stop();	// transmisja STOP z oczekiwanie na potwierdzenie

void I2C_Write(uint8_t);
uint8_t I2C_Read(uint8_t);

uint8_t I2C_Read_Single_Byte(uint8_t reg, uint8_t devAddr);
void I2C_Write_Single_Byte (uint8_t reg, uint8_t value, uint8_t devAddr);

void I2C_Read_Gyro();
void I2C_Read_Gyro_X();
void I2C_Read_Gyro_Y();
void I2C_Read_Gyro_Z();

void I2C_Read_Acc();
void I2C_Read_Acc_X();
void I2C_Read_Acc_Y();
void I2C_Read_Acc_Z();

#endif /* I2C_GYRO_H_ */