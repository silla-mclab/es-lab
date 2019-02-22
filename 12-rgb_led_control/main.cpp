#include   "mbed.h"              

PwmOut r(A1);
PwmOut g(PC_6);
PwmOut b(A3);

int main()
{
    r.period(0.001);
    g.period(0.001);
    b.period(0.001);
    
    while(1) {
        for(double i = 0.0; i < 1.0 ; i +=   0.001) {
            double p = 3 * i;
            g = 1.0 - ((p < 1.0) ? 1.0 - p   : (p > 2.0) ? p - 2.0 : 0.0);
            r = 1.0 - ((p < 1.0) ? p : (p   < 2.0) ? 1.0 : 2.0 - p);
            b = 1.0 - ((p < 1.0) ? 0.0 :   (p > 2.0) ? p - 2.0 : 2.0 - p);   
            wait (0.01);
        }
    }
} 
