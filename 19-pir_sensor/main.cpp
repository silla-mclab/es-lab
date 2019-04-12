#include "mbed.h"

Serial pc(USBTX, USBRX);
DigitalIn motion(D5);
DigitalOut light(PA_13);

volatile int Sensor = 0;

int main(void)
{
//	pc.baud(115200);
		
	while(1) {
		Sensor = motion;
		if (Sensor) {
			light = 1;
			pc.printf("IR Detect: LED On \r\n");
			wait(3);
		}
		else light = 0;
	}
}
