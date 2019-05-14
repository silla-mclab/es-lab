#include "mbed.h"

Serial pc(USBTX, USBRX);

AnalogIn x_axis(PC_2);
AnalogIn y_axis(PC_3);

DigitalOut led1(PA_13);

Ticker joystick;

void joystickHandler() {
  int x = x_axis.read() * 1000;
  int y = y_axis.read() * 1000;
	printf("\rX=%3d, Y=%3d\n", x, y);
}

int main() {
	pc.baud(115200);
	
	joystick.attach(&joystickHandler, 0.5);
	
	while(1) {
		led1 = !led1;
		wait(1.0);
	}
}
