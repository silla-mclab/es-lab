#include "mbed.h"
#include "doorlock_sim.h"
#include "utils.h"

Serial pc(USBTX, USBRX);
Timer btnTm;

int state;
uint8_t passwd[4] = {1, 1, 1, 1};

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
//					pc.printf("%d\n", (int)pw_in[i]);
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
	btnTm.start();
	
	initUtils();

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
					startLed3Blinking();
					btnTm.reset();
					state = PASSWD_INPUT;
				}
				break;
			case PASSWD_INPUT:
				switch (processPasswdInput()) {
					case FAILED:
						FAILURE_BEEP();
					  stopLed3Blinking();
					  state = DOOR_CLOSED;
					  break;
					case SUCCEED:
						SUCCESS_BEEP();
					  stopLed3Blinking();
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
					startLed1Blinking();
					btnTm.reset();
				}
				break;
			case CLOSE_BUTTON_INPUT:
				switch (processCloseButtonInput()) {
					case FAILED:
						FAILURE_BEEP();
					  stopLed1Blinking();
					  state = DOOR_OPEN;
					  break;
					case SUCCEED:
						SUCCESS_BEEP();
					  stopLed1Blinking();
					  state = DOOR_CLOSED;
					  break;
				  default:
					  break;
				}
				break;
		}
	}	
}
