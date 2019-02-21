#include "mbed.h"

DigitalIn button(PA_14);    // the first SW
DigitalOut myled(PA_13);    // Green LED

int main() {
	while (1) {
		if (!button) {
			myled = !myled;
			while (!button)  wait(0.3);
		}
	}
}
