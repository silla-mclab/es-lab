#include   "mbed.h"
#include   "motordriver.h"

Motor A(D11, PC_8);   // pwm, dir

int main() {
    while (1) {
        // For speed test.
        for (double s= 0; s < 1.0 ; s += 0.1) {
            A.forward(s); 
            wait(1);
	}

        A.stop();
        wait(3);

        for (double s= 0; s < 1.0 ; s += 0.1) {
            A.backward(s);
            wait(1);
       }
    }
}
	