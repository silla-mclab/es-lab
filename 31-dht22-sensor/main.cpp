#include "mbed.h"
#include "DHT22.h"

Serial pc(USBTX, USBRX);
DHT22 dht22(PB_2);

int main() {
	float h = 0.0f, c = 0.0f;
	
	pc.baud(115200);
	
	while(1) {
		wait(1.0);

		dht22.sample();
		c = dht22.getTemperature()/10.0;
		h = dht22.getHumidity()/10.0;

		pc.printf("Temp: %.1f, Humid: %.1f\n", c, h); 
	}
}
