#include "mbed.h"
#include "PulseSensor.h"

Serial pc(USBTX, USBRX);
DigitalOut led(PA_13);
     
/** Print the data in a format that can be parsed by the 
 *  Processing application available here: http://pulsesensor.myshopify.com/pages/code-and-guide
 */
void sendDataToProcessing(char symbol, int data)
{
    pc.printf("%c%d\r\n", symbol, data);
//	if (symbol == 'B')
//		pc.printf("BMP = %d\n", data);
}
  
int main() {
    pc.baud(115200);
	
    PulseSensor sensor(PB_1, sendDataToProcessing); 
    sensor.start();
 
    while(1) {
    }
}
