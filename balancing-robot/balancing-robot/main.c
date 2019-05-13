#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <math.h>
#include "MOTOR_DRIVER.h"
#include "I2C_GYRO.h"

#define DT 0.05
#define Kp  100
#define Kd  0
#define Ki  0
#define targetAngle 0
void accAndGyroInit(){
	// Accelerometr initialization
	I2C_Write_Single_Byte(CTRL1_XL, 0x60, GYRO_ADDRESS); //Acc ODR 416 Hz (high performance)
	I2C_Write_Single_Byte(CTRL9_XL, 0x38, GYRO_ADDRESS); // Enable axis X,Y,Z
	
	//Gyro initialization
	I2C_Write_Single_Byte(CTRL10_C, 0x38, GYRO_ADDRESS);
	I2C_Write_Single_Byte(CTRL2_G, 0x60, GYRO_ADDRESS);
}

float gyroAngle = 0;
float accAngle = 0;
float previousAngle = 0;
float currentAngle = 0;
float error = 0, errorSum =0;
int16_t motorPower = 0;
int16_t rangeConstraint(int16_t val, int16_t min, int16_t max){
	if (val > max ) return max;
	if( val < min) return min;
	return val;
}

void calcAngle(){
	I2C_Read_Acc_X();
	I2C_Read_Acc_Z();
	I2C_Read_Gyro_Y();
	
	accAngle = atan2f(AccZ,AccX) - M_PI_2;
	accAngle *= 180.0f/M_PI;
	
	gyroAngle = GyroY/128;
	gyroAngle *= DT;
	
	currentAngle = 0.9934*(previousAngle + gyroAngle) + 0.0066*(accAngle);
	error = currentAngle - targetAngle;
	errorSum = errorSum + error;
	errorSum = rangeConstraint(errorSum, -300,300);
	
	motorPower = Kp*(error) + Ki*(errorSum)*DT - Kd*(currentAngle-previousAngle)/DT;
	motorPower = rangeConstraint(motorPower, -255,255);
	previousAngle = currentAngle;
}

int main(void)
{
	void zeroTable(); // set 0 to all elements of table 'tab'
	
	/* ACC & GYRO INITIALIZATION*/
	I2C_Init();
	accAndGyroInit();


	//**** MOTOR CONTROL ****//
	//set up Fast PWM Mode on pin PD6
	DDRD |= (1 << DDD6) | ( 1 << DDD5);	// set PD6 as output
	OCR0A = 0;				// set duty cycle max values is 256 (100%)
	OCR0B = 0;
	TCCR0A |= ((1 << COM0A1) | (1 << COM0B1)); //set OC0A and OC0B
	TCCR0A |= (1 << WGM01) | (1 << WGM00); // set fast PWM Mode
	TCCR0B |= (TCCR0B & ~((1 << CS01) | (1 << CS02)))| (1<<CS00); // set prescaler

	DDRB |= MOTOR_LEFT  | MOTOR_MODE; // set as output
	DDRD |= MOTOR_RIGHT; //set as output
	MOTOR_MODE_EN;
	while (1)
	{
		calcAngle();
		if(motorPower <0)
			moveForward(motorPower);
		else
			moveBackward(motorPower);
		
		_delay_ms(DT*1000);
	}
}
