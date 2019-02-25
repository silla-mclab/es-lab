#include   "mbed.h"              

DigitalOut sound(PC_9);

int main()
{
    while(1) {
        sound = 1;
			  wait(0.001);
        sound = 0;
			  wait(0.001);
    }
} 
