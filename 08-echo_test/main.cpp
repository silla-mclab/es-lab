#include "mbed.h"

Serial pc(USBTX, USBRX);


int main() {
	pc.printf("Echoes anything you type\r\n");
	while (1) {
		pc.putc(pc.getc());
	}
}
