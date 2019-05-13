/*
 * I2C_GYRO.c
 *
 * Created: 2018-12-25 14:31:30
 *  Author: Jan
 */ 
#ifndef I2C_GYRO_CPP
#define I2C_GYRO_CPP
#include "I2C_GYRO.h"

void I2C_Init(){
	TWCR &= ~(1<<TWIE);
	TWBR = 1;
	TWSR &= ~(1<<TWPS1) | (1<<TWPS0);
}

void I2C_Start(){
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTA);
	while ( !( TWCR & ( 1<<TWINT ) ) );
}
void I2C_Stop(){
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	while (!(TWCR & (1 << TWSTO)));
}

void I2C_Write(uint8_t byte){
	TWDR = byte;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while(!(TWCR&(1<<TWINT)));
}

uint8_t I2C_Read(uint8_t ack){ //ack = 1: MAK; ack = 0:NMAK;
    TWCR = (1<<TWINT)|(1<<TWEN)|(ack << TWEA);
    while(!(TWCR&(1<<TWINT)));
	return TWDR;
}

uint8_t I2C_Read_Single_Byte(uint8_t reg , uint8_t devAddr){
	uint8_t data;
	
	I2C_Start();	
	I2C_Write(devAddr); //SDA+W
	I2C_Write(reg);	
	I2C_Start();
	I2C_Write(devAddr+1); // SDA+R 
	data = I2C_Read(0); 
	I2C_Stop();
	
	return data;
}

void I2C_Write_Single_Byte (uint8_t reg, uint8_t value, uint8_t devAddr){
	I2C_Start();
	I2C_Write(devAddr);
	I2C_Write(reg);
	I2C_Write(value);
	I2C_Stop();
}
void I2C_Read_Acc(){
	I2C_Read_Acc_X();
	I2C_Read_Acc_Y();
	I2C_Read_Acc_Z();
}
void I2C_Read_Acc_X(){
	uint8_t high, low;
	high = I2C_Read_Single_Byte(OUTX_H_XL, GYRO_ADDRESS);
	low = I2C_Read_Single_Byte(OUTX_L_XL, GYRO_ADDRESS);
	AccX = (high << 8) | low;
}

void I2C_Read_Acc_Y(){
	uint8_t high, low;
	high = I2C_Read_Single_Byte(OUTY_H_XL, GYRO_ADDRESS);
	low = I2C_Read_Single_Byte(OUTY_L_XL, GYRO_ADDRESS);
	AccY = (high << 8) | low;
}

void I2C_Read_Acc_Z(){
	uint8_t high, low;
	high = I2C_Read_Single_Byte(OUTZ_H_XL, GYRO_ADDRESS);
	low = I2C_Read_Single_Byte(OUTZ_L_XL, GYRO_ADDRESS);
	AccZ = (high << 8) | low;
}

void I2C_Read_Gyro(){
	I2C_Read_Gyro_X();
	I2C_Read_Gyro_Y();
	I2C_Read_Gyro_Z();
}

void I2C_Read_Gyro_X(){
	uint8_t high, low;
	high = I2C_Read_Single_Byte(OUTX_H_G, GYRO_ADDRESS);
	low = I2C_Read_Single_Byte(OUTX_L_G, GYRO_ADDRESS);
	GyroX = (high << 8) | low;
}

void I2C_Read_Gyro_Y(){
	uint8_t high, low;
	high = I2C_Read_Single_Byte(OUTY_H_G, GYRO_ADDRESS);
	low = I2C_Read_Single_Byte(OUTY_L_G, GYRO_ADDRESS);
	GyroY = (high << 8) | low;
}

void I2C_Read_Gyro_Z(){
	uint8_t high, low;
	high = I2C_Read_Single_Byte(OUTZ_H_G, GYRO_ADDRESS);
	low = I2C_Read_Single_Byte(OUTZ_L_G, GYRO_ADDRESS);
	GyroZ = (high << 8) | low;
}


#endif
