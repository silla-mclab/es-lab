#include "mbed.h"

AnalogIn sensorUV(A4);

int main() {
    double value;
    while (true) {
        value = sensorUV;
        printf("\rUV Value = %3.2f%%", value*100);
        wait(0.1);
    }
}
