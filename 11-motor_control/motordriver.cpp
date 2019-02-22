#include   "mbed.h"
#include   "motordriver.h"
 
Motor::Motor(PinName pwm, PinName dir):
        _pwm(pwm), _dir(dir) 
{
    // Set initial condition of PWM
    _pwm.period(0.001);		// 1000Hz PWM
    _pwm = 0;
 
    // Initial condition of output enables
    _dir = 0;
 
    sign = 0;   // i.e nothing.
}
 
void   Motor::forward(double speed) {
	float temp = 0;

	if (sign == -1) {
	   _pwm = 0;     
	   wait (0.2);
	}
	_dir = 1;
	temp = abs(speed);
	_pwm = temp;
	sign = 1;
}

void Motor::backward(double speed) {
	float temp = 0;

	if (sign == 1) {
	   _pwm = 0;  
	   wait (0.2);
	}
	_dir = 0;
	temp = abs(speed);
	_pwm = temp;
	sign = -1;
}
 
void   Motor::stop(void) {
	_pwm = 0;
  sign = 0;
}
