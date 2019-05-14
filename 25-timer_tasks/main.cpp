#include "mbed.h"

Serial pc(USBTX, USBRX);

DigitalOut led1(PA_13);
DigitalOut led2(PB_10);

Timer timer_fast;
Timer timer_slow;

void task_fast(void);
void task_slow(void);

#define FAST_TASK_INTERVAL		0.2
#define	SLOW_TASK_INTERVAL		1.0

int main() {
	pc.baud(115200);

  timer_fast.start();
	timer_slow.start();
	
  while (1) {
		if (timer_fast.read() > (float)FAST_TASK_INTERVAL) {
			task_fast();
			timer_fast.reset();
		}
		
		if (timer_slow.read() > (float)SLOW_TASK_INTERVAL) {
			task_slow();
			timer_slow.reset();
		}
  }
}

void task_fast(void) {
	led1 = !led1;
}

void task_slow(void) {
	led2 = !led2;
}
