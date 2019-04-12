#include "mbed.h"
#include "doorlock_sim.h"

Serial pc(USBTX, USBRX);
BusIn buttons(PA_14, PB_7, PC_4);
BusOut leds(PA_13, PB_10, PA_4);
DigitalOut buzzer(PC_9);

Timer t;
Timer btnTm;
Ticker ticker;

int state;
uint8_t passwd[4] = {1, 1, 1, 1 };

void playNote(char note, double play_time) {
	static int notePeriods[] = {
		1136, 1014, 1915, 1700, 1519, 1432, 1275, 956
	};

  int half_period = (note == 'C') ? notePeriods[7] : notePeriods[note-'a'];

	t.reset();
	while (t.read() < play_time) {
		buzzer = 1;
		wait_us(half_period);
		buzzer = 0;
		wait_us(half_period);
	}  
}	

uint8_t readButtonEdge() {
	static int prevState = 0b00000111;
	uint8_t edge = NO_EDGE;
  int currState = buttons;
	
	int edges = currState ^ prevState;
	if (edges != 0) {
		wait(0.1);			// debouncing
		if (edges & 0x01) {
			edge = (currState & 0x01) ? BTN1_RISING : BTN1_FALLING;
		}
		else if (edges & 0x02) {
			edge = (currState & 0x02) ? BTN2_RISING : BTN2_FALLING;
		}
		else if (edges & 0x04) {
			edge = (currState & 0x04) ? BTN3_RISING : BTN3_FALLING;
		}
		prevState = currState;
	}
	
	return edge;
}

uint8_t getButtonInput() {
	uint8_t edge = readButtonEdge();
	if (edge & 0x10) {
		BUTTON_BEEP();
	}
	return edge;
}

void displayLeds(int ledValue) {
	leds = ledValue;
}

void blink_led1() {
	leds = leds ^ LED1_ON;
}

void blink_led3() {
	leds = leds ^ LED3_ON;
}

int processPasswdInput() {
	static int piState = FIRST_INPUT;
	static uint8_t pw_in[4];
	
	int status = CONTINUE;
	uint8_t edge;
	int i;

	if (btnTm.read() > BUTTON_INPUT_TIMEOUT) {
	  piState = PW_FAILURE;
	}
	
	switch (piState) {
		case FIRST_INPUT:
			edge = getButtonInput();
			if (edge & 0x10) {
				edge &= 0x07;
				pw_in[0] = (edge == 4) ? 3 : edge;
				piState = SECOND_INPUT;
				btnTm.reset();
			}
			break;
		case SECOND_INPUT:
			edge = getButtonInput();
		  if (edge & 0x10) {
				edge &= 0x07;
				pw_in[1] = (edge == 4) ? 3 : edge;
				piState = THIRD_INPUT;
				btnTm.reset();
			}
			break;		
		case THIRD_INPUT:
			edge = getButtonInput();
		  if (edge & 0x10) {
				edge &= 0x07;
				pw_in[2] = (edge == 4) ? 3 : edge;
				piState = FOURTH_INPUT;
				btnTm.reset();
			}
			break;		
		case FOURTH_INPUT:
			edge = getButtonInput();
		  if (edge & 0x10) {
				edge &= 0x07;
				pw_in[3] = (edge == 4) ? 3 : edge;

				// password comparison
				for (i=0; i<4; i++) {					
					pc.printf("%d\n", (int)pw_in[i]);
					if (passwd[i] != pw_in[i])
						break;
				}
				if (i >= 4)  piState = PW_SUCCESS;
				else piState = PW_FAILURE;
			}
			break;		
		case PW_SUCCESS:
			status = SUCCEED;
		  piState = FIRST_INPUT;
			break;		
		case PW_FAILURE:
			status = FAILED;
		  piState = FIRST_INPUT;
			break;
  }		

  return status;	
}

int processCloseButtonInput() {
	int status = CONTINUE;
  
  if (btnTm.read() > BUTTON_INPUT_TIMEOUT) {
    status = FAILED;
  }
  else {
		int edge = getButtonInput();
    if (edge == BTN1_RISING) {
		  if (btnTm.read() > MODE_CHANGE_TIMEOUT) {
			  status = SUCCEED;
				BUTTON_BEEP();
		  }
		  else {
			  status = FAILED;
		  }
		}
		else if (edge == BTN1_FALLING) {
			btnTm.reset();
		}	
	}

  return status;
}	
	

void setup() {
	pc.baud(115200);
	t.start();
	btnTm.start();

	state = DOOR_CLOSED;
  DISPLAY_DOOR_CLOSED();	
}

int main() {
	setup();
	
	while (1) {
		switch (state) {
			case DOOR_CLOSED:
				DISPLAY_DOOR_CLOSED();
			  if (getButtonInput() == BTN3_FALLING) {
					ticker.attach(&blink_led3, 0.5);
					btnTm.reset();
					state = PASSWD_INPUT;
				}
				break;
			case PASSWD_INPUT:
				switch (processPasswdInput()) {
					case FAILED:
						FAILURE_BEEP();
					  ticker.detach();
					  state = DOOR_CLOSED;
					  break;
					case SUCCEED:
						SUCCESS_BEEP();
					  ticker.detach();
					  state = DOOR_OPEN;
					  break;
				  default:
					  break;
				}
				break;
			case DOOR_OPEN:
				DISPLAY_DOOR_OPEN();
			  if (getButtonInput() == BTN1_FALLING) {
					state = CLOSE_BUTTON_INPUT;
					ticker.attach(&blink_led1, 0.5);
					btnTm.reset();
				}
				break;
			case CLOSE_BUTTON_INPUT:
				switch (processCloseButtonInput()) {
					case FAILED:
						FAILURE_BEEP();
					  ticker.detach();
					  state = DOOR_OPEN;
					  break;
					case SUCCEED:
						SUCCESS_BEEP();
					  ticker.detach();
					  state = DOOR_CLOSED;
					  break;
				  default:
					  break;
				}
				break;
		}
	}	
}
