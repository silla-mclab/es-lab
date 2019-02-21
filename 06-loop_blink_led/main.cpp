#include "mbed.h"

DigitalIn button(PA_14);    // the first SW
DigitalOut myled(PA_13);    // Green LED

int i;

int main() {
	while (1) {
		if (!button) {
			for (i=0; i<3; i++) {
				myled = 1;
				wait(0.3);
				myled = 0;
				wait(0.7);
			}
		}
	}
}
