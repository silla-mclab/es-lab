#include "mbed.h"

Serial pc(USBTX, USBRX);

InterruptIn button(PA_14);		//define and name the interrupt input
DigitalOut led(PA_13);
DigitalOut redLed(PA_4);

void ISR1() { 	// this is the response to interrupt, i.e. the ISR
  led = !led;
}

int main() {
	pc.baud(115200);
	
  button.fall(&ISR1); 	// attach the address of the ISR function
												// to interrupt rising edge
  while (1) {
    if (!button) {
      pc.printf ("\r\n Button Pressed");
    }

		redLed = !redLed;
		wait(0.5);
  }
}
