#include "mbed.h"

Serial pc(USBTX, USBRX);
DigitalIn motion(D5);
DigitalOut light(PA_13);
Timer t;

volatile int Sensor = 0;
volatile int prevState = 0;

int main(void)
{
	pc.baud(115200);
	pc.printf("Start...\n");
  t.start();
	
	while(1) {
		Sensor = motion;
		if (Sensor != prevState) {
			if (Sensor) {
				t.reset();
				pc.printf("IR Detect: LED On \r\n");
			}
			else {
				pc.printf("No motion: LED Off, Duration = %4.2f sec \r\n", t.read());
			}
			light = Sensor;
			prevState = Sensor;
		}
	}
}
