/*
 * PID_CONTROLLER.c
 *
 * Created: 2019-05-12 18:44:36
 *  Author: Jan
 */ 
#ifndef PID_CONTROLLER_C
#define PID_CONTROLLER_C

float pid_compute(float dane, float x)
{
	error = dane - x;
	integral = integral + error * dt;
	derivative = (error - previous_error) / dt;
	output = kp * error + ki * integral + kd * derivative;
	previous_error = error;
	if(output>100)output=100;
	if(output<-100)output=-100;
	return output;

}

#endif