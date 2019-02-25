#include "mbed.h"

#define Threshold   80       

Serial pc(USBTX, USBRX);     // UART Serial Comm.
DigitalOut myled (PA_13);    // Green LED
PwmOut buzz(PC_9);       // Buzzer
AnalogIn loudness(A5);       // Sound sensor

int main()
{
    int   loud_level;

	  buzz.period(0.002);
    while (true)   {
			loud_level   = (int)(100*loudness.read());
			if (loud_level > Threshold) {
					pc.printf("Loudness:   %d\r\n", loud_level);
					buzz   = 0.5;
					myled   = 1;
					wait(10);
					buzz   = 0;
					myled   = 0;
			}
    }
}
