#include "mbed.h"
#include "Adafruit_GFX/Adafruit_GFX.h"
#include "Adafruit_GFX/Adafruit_SSD1306.h"
 
DigitalOut myled(PA_13);
 
// an I2C sub-class that provides a constructed default
class I2CPreInit : public I2C
{
public:
    I2CPreInit(PinName sda, PinName scl) : I2C(sda, scl)
    {
        frequency(400000);
        start();
    };
};
 
//I2CPreInit gI2C(I2C_SDA, I2C_SCL);
I2C	 gI2C(I2C_SDA, I2C_SCL);
Adafruit_SSD1306_I2c display(gI2C, D13, 0x78, 64, 128);

void testdrawcircle(void);

int main()
{   
		uint16_t x=0;
 
    display.printf("%ux%u OLED Display\r\n", display.width(), display.height());
    
    while(1)
    {
        myled = !myled;
 
        display.printf("%u\r",x);
        display.display();
        x++;
        wait(1.0);
				testdrawcircle();			
    }
}

void testdrawcircle(void) {
  display.clearDisplay();

  for(int16_t i=0; i<max(display.width(),display.height())/2; i+=2) {
    display.drawCircle(display.width()/2, display.height()/2, i, WHITE);
    display.display();
    wait_ms(1);
  }

  wait(2.0);
}