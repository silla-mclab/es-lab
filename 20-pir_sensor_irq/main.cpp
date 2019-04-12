#include "mbed.h"

Serial pc(USBTX, USBRX);
DigitalOut light(PA_13);
InterruptIn motion(D5);

int cnt	= 0;

void rising_handler(void) {
    cnt++;        // increment count variable
    light = 1;    // turn on Green LED
}

void falling_handler(void) {
    light = 0;    // turn off Green LED
}

int main(void)
{
    int last_cnt = 0;

    motion.rise(&rising_handler);
    motion.fall(&falling_handler);
    cnt = 0;
    light = 0;

    while(1) {
        if(cnt != last_cnt) {
            pc.printf("Hello! I've detected %d times since reset\r\n", cnt);
           last_cnt = cnt; 
        }
    }
}
