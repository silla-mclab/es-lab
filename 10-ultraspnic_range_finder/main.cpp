#include "mbed.h"

Serial pc(USBTX, USBRX);

DigitalOut Trig(D10);
DigitalIn Echo(D7);
Timer t;
float i;

int main() {
  t.start();
  pc.printf("\x1B\x48\x1B\x4A");
  pc.printf("---====[ Ultrsonic Range   (SR04) ]===---");
  
  while(1) {     
      Trig = 1;
      wait(0.00002);
      Trig = 0;

      while(!Echo) ;

      t.reset();
      while(Echo);
      i = t.read_us();

      pc.printf("\x1B\x48");
      pc.printf("\n\n\rPulselength %6.0f   uS", i);
      i = i/58;
      pc.printf("\n\n\rDistance %4.0f   cm", i);
      wait(2);
   }
}
