#include "mbed.h"

DigitalOut myled1(PA_13);
DigitalOut myled2(PB_10);
DigitalOut myled3(PA_4);

int main() {
		// initialize all LEDs to be off
	  myled1 = 0;
	  myled2 = 0;
	  myled3 = 0;
	
    while(1) {
        myled1 = 1; 	// LED1 is ON
        wait(1.0); 		// delay 1000 ms
			  myled1 = 0;		// LED1 is OFF

			  myled2 = 1;		// LED2 is On
        wait(1.0); 		// delay 1000 ms
			  myled2 = 0;		// LED2 is OFF
			  
			  myled3 = 1;		// LED3 is On
        wait(1.0); 		// delay 1000 ms
			  myled3 = 0;		// LED3 is OFF
    }
}
