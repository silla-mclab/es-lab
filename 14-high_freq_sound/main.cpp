#include "mbed.h"

PwmOut sound(PC_9);
DigitalOut myled(PA_13);

int main() {
    sound.period(0.0000625);
    sound = 0.5;
    myled = 0;
    while (1) {
        myled = !myled;
        wait(1);
    }
}
