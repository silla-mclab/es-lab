#include "mbed.h"

Serial pc(USBTX, USBRX);
Serial bt(PA_11, PA_12);

int main() {
	pc.baud(115200);
	bt.baud(38400);
	
	while(1) {
		while (pc.readable()) {
			int c = pc.getc();
			bt.putc(c);
//			pc.putc(c);			// for echo to PC 
		}
		
		while (bt.readable()) {
			pc.putc(bt.getc());
		}
	}
}
