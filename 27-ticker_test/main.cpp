#include "mbed.h"

DigitalOut led1(PA_13);
DigitalOut led2(PB_10);

Ticker time_up;

void blinkLed1() {
  led1 = !led1;
}

int main() {
	time_up.attach(&blinkLed1, 0.5);
	
	while(1) {
		led2 = !led2;
		wait(1.0);
	}
}
