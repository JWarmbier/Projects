/*
 * MOTOR_DRIVER.h
 *
 * Created: 2018-12-25 16:10:23
 *  Author: Jan
 */ 
#include <avr/io.h>

#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#define MOTOR_LEFT (1 << DDB0)
#define MOTOR_LEFT_FORWARD PORTB |= MOTOR_LEFT
#define MOTOR_LEFT_BACKWARD PORTB &= ~MOTOR_LEFT

#define MOTOR_RIGHT (1 << DDD7)
#define MOTOR_RIGHT_FORWARD PORTD |= MOTOR_RIGHT
#define MOTOR_RIGHT_BACKWARD PORTD &= ~MOTOR_RIGHT

#define MOTOR_MODE (1 << DDB2)
#define MOTOR_MODE_EN PORTB|=MOTOR_MODE
#define MOTOR_MODE_DIS PORTB &= ~MOTOR_MODE

void moveBackward(uint8_t);

void moveForward(uint8_t);



#endif /* MOTOR_DRIVER_H_ */