#include "mbed.h"

DigitalIn button(PC_4);    // the first SW
DigitalOut myled(PA_13);    // Green LED

int main() {
	while (1) {
		if (!button) {
			myled = 1;
			wait(5);
			myled = 0;
		}
	}
}
