#include   "mbed.h"
 
Serial   pc(USBTX, USBRX);
AnalogIn   analog_value(A0);
DigitalOut   led(PA_13);

int main() {
    double  meas;
    printf("\nAnalogIn example\n");
    
    while(1) {
        meas = analog_value.read();
        meas = meas * 1000; 
        printf("measure = %.0f   \r\n", meas);
        if (meas > 250) {
          led = 1;
        }
        else {
          led = 0;
        }
        wait(1); // 1000 ms
    }
}
