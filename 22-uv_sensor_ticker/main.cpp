#include "mbed.h"
 
AnalogIn sensorUV(A0);
Ticker sensor_ticker;
 
void ticker_handler(void){ 
		// get the value from the sensor and print it out
    float value;
    value = sensorUV;
    printf("\rUV Value = %3.2f%%",value*100);
}
 
int main() {
    // set up ticker_handler() to be called every 0.1 second
    sensor_ticker.attach(&ticker_handler, 0.1); 
 
    while (true) {
        // do something else ...
    }
}
