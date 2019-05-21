#include "mbed.h"

Serial pc(USBTX, USBRX);
Serial bt(PA_11, PA_12);

DigitalOut myled(PA_13);

int main() {
	pc.baud(115200);
	bt.baud(38400);
	
	//prints data on mobile
	bt.printf("Connection Established\n");
	//print data on pc terminal
	pc.printf("Connection Established\n");	
	
	while(1) {
		// For reading and writing data from/to bluetooth HC-06
		// check if bluetooth is readable and execute commands // to toggle LED
		if (bt.readable()) {
			 char input_key=  bt.putc(bt.getc());

				//tutn on LED if "y" is entered
				if(input_key == 'y') {
						myled = 1;
						bt.printf("LED is ON\n");
				}

				//tutn on LED if "n" is entered
				if(input_key == 'n') {
						myled = 0;
						bt.printf("LED is OFF\n");
				}
		}

		//For reading and writing data from/to pc terminal
		//check if pc is readable and execute commands to toggle LED
		if (pc.readable()) {
				char input_key=  pc.putc(pc.getc());
				if(input_key == 'y') {
						myled = 1;
						pc.printf("LED is ON\n");
				}
				if(input_key == 'n') {
						myled = 0;
						pc.printf("LED is OFF\n");
				}
		}
	}
}
