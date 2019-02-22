#ifndef   MBED_MOTOR_H
#define   MBED_MOTOR_H
 
#include "mbed.h"
 
class Motor {
public:
	Motor(PinName pwm, PinName dir);
	void forward(double speed);
	void backward(double speed);
	void stop(void);
        
protected:
	PwmOut _pwm;       // PWM pin
	DigitalOut _dir;   // motor diection pin  
	int sign;          // current motor direction
};

#endif
