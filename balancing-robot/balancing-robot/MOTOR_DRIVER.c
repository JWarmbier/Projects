
#ifndef MOTOR_DRIVER_C
#define MOTOR_DRIVER_C
#include "MOTOR_DRIVER.h"

void moveBackward(uint8_t duty){
	MOTOR_RIGHT_BACKWARD;
	MOTOR_LEFT_FORWARD;
	OCR0A = duty;				// set duty cycle max values is 256 (100%)
	OCR0B = duty;
}

void moveForward(uint8_t duty){
	MOTOR_RIGHT_FORWARD;
	MOTOR_LEFT_BACKWARD;
	OCR0A = duty;				// set duty cycle max values is 256 (100%)
	OCR0B = duty;
	
}

#endif /* MOTOR_DRIVER_C */