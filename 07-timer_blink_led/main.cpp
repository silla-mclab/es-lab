#include "mbed.h"

DigitalIn button(PA_14);    // the first SW
DigitalOut myled(PA_13);    // Green LED
Timer t;
int i;

int main() {
	t.start();
	while (1) {
		if (!button) {
			t.reset();
			while (!button) wait(0.3);
   		i = t.read_ms() - 300;
			
      t.reset();
      while (t.read_ms() < i) {			
				myled = 1;
				wait(0.5);
				myled = 0;
				wait(0.5);
			}
		}
	}
}
