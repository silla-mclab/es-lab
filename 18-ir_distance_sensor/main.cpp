#include "mbed.h"

Serial pc(USBTX, USBRX);
AnalogIn IRSensor(PC_5);

int dist = 0;
double a, volt;

double map(double x, double in_min, double in_max, double out_min, double out_max)
{
  double scale = (x - in_min) / (in_max - in_min);
  return scale * (out_max - out_min) + out_min;
}

int main() {
    pc.printf("IR Distance Sensor Test!!\r\n");

    while(1) {
       a = IRSensor;
       pc.printf("IR sensor reads %2.2f\r\n", a);
       volt = map(IRSensor, 0, 1.0, 0, 3200);
       dist = (int)((27.61f / (volt - 0.1696f)) * 1000.0);
       pc.printf("distance %d cm\r\n\n", dist);      
       wait(2);
    }
}