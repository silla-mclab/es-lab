#include "mbed.h"
 
Serial pc(USBTX, USBRX);
AnalogIn loudness(A5);     // Sound Sensor
DigitalOut myled(PA_13);   // Green LED

double   i;

int   main() {
    while(1) {
        i = loudness;

        if (i>0.80) {
             myled = 1;
             pc.printf("Loudness: %f %\r\n", i*100);
             wait(1);
        }
        else myled = 0;
    }
}
