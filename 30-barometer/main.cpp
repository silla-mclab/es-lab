#include "mbed.h"
#include "BMP180.h"

Serial pc(USBTX, USBRX);
I2C i2c(I2C_SDA, I2C_SCL);
BMP180 bmp180(&i2c);

int main() {
	pc.baud(115200);
	while (1) {
		if (bmp180.init() != 0) {
			pc.printf("Error in communicating with BMP180\n");
		}
		else {
			pc.printf("BMP180 initialized...\n");
			break;
		}
		wait(1);
	}
	
	while (1) {
		bmp180.startTemperature();
		wait_ms(5);
		float temp;
		if (bmp180.getTemperature(&temp) != 0) {
			pc.printf("Error in getting temperature...\n");
			continue;
		}
		
		bmp180.startPressure(BMP180::ULTRA_LOW_POWER);
		wait_ms(10);
		int pressure;
		if (bmp180.getPressure(&pressure) != 0) {
			pc.printf("Error in getting pressure...\n");
			continue;
		}
		
		pc.printf("Pressure = %d Pa, Temperature = %.1f C\n", pressure, temp);
		
		// Calculate altitude in meter
		float altitude = 44330.0f * (1.0f - pow((pressure/102710.0f), (1.0f/5.255f)));
		pc.printf("Altitude = %.1f m\n\n", altitude);
		
		wait(1);
	}
}
