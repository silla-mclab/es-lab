#include "mbed.h"

Serial pc(USBTX, USBRX);
int i = 0;

int main() {
	while (1) {
		pc.printf("\rCounter %d    ", i);
		i++;
		wait(1.0);
	}
}
