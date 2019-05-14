#include "mbed.h"

Serial pc(USBTX, USBRX);

SPI spiAccel(PB_15, PB_14, PB_13);		// MOSI, MISO, SCL
DigitalOut csAccel(PB_12);						// CS

char buffer[6];
int16_t data[3];
float x, y, z;

void initAccelSensor() {
	csAccel = 1;									// initially deactivate ADXL345 dev.
  spiAccel.format(8, 3);				// b-bit data, mode 3
  spiAccel.frequency(2000000);	// 2MHz clock rate

  csAccel = 0;						// select ADXL345 dev.
	spiAccel.write(0x31);		// send Data format control register address
 	spiAccel.write(0x01);		// send data format control data
	csAccel = 1;
	
  csAccel = 0;						// select ADXL345 dev.
	spiAccel.write(0x2D);		// send Power control register address
 	spiAccel.write(0x08);		// setting bit-3 causes ADXL345 dev to enter Measure mode
	csAccel = 1;						
}

int main() {
	pc.baud(115200);
  initAccelSensor();
  wait(0.2);
	
	while(1) {
		csAccel = 0;													// select ADXL345 dev.
		spiAccel.write(0x80|0x40|0x32);				// RW-bit high, MB-bit high, 5~0-bit address bits
		for (int i=0; i<6; i++) {
			buffer[i] = spiAccel.write(0x00);		// read back 6 data bytes
		}
		csAccel = 1;						

		data[0] = buffer[1] << 8 | buffer[0];	// combine upper & lower bytes to a 16-bit value
		data[1] = buffer[3] << 8 | buffer[2];
		data[2] = buffer[5] << 8 | buffer[4];

		x = 0.0078 * data[0];									// convert to actual g values
		y = 0.0078 * data[1];
		z = 0.0078 * data[2];
		
		pc.printf("x = %+1.2fg\t y = %+1.2fg\t z = %+1.2fg\n", x, y, z);
		
		wait(0.2);
	}
}
