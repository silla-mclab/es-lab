#include "mbed.h"

DigitalIn button(PA_14);
DigitalOut led1(PA_13);
DigitalOut led2(PB_10);

Timeout response;

void blink() {
  led2 = 1;
	wait(1.0);
	led2 = 0;
}

int isButtonPressed() {
	static int prevState = 1;
	
	int pressed = 0;
	int currState = button;
	if (currState != prevState) {
		if (currState == 0)  pressed = 1;
		prevState = currState;
		wait(0.1);		// for debouncing
	}
  return pressed;
}

int main() {
	while(1) {
		if (isButtonPressed()) {
			response.attach(&blink, 2.0);
			led1 = 1;
			wait(1.0);
		}
		else {
			led1 = 0;
		}
	}
}
