#include "mbed.h"

#define	WIFI_COMM

Serial pc(USBTX, USBRX);
#ifdef	WIFI_COMM
Serial esp8266(PA_9, PA_10);	// ESP-8266 WiFi module
#else
Serial bt(PA_11, PA_12);			// HC-06 Bletooth module
#endif

int main() {
	pc.baud(115200);
#ifdef	WIFI_COMM
	esp8266.baud(115200);
#else
	bt.baud(38400);
#endif
	
	while(1) {
#ifdef	WIFI_COMM
		while (pc.readable()) {
			int c = pc.getc();
			esp8266.putc(c);
		}
		
		while (esp8266.readable()) {
			pc.putc(esp8266.getc());
		}
#else
		while (pc.readable()) {
			int c = pc.getc();
			bt.putc(c);
//			pc.putc(c);			// for echo to PC 
		}
		
		while (bt.readable()) {
			pc.putc(bt.getc());
		}
#endif		
	}
}
