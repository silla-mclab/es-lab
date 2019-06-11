#include "mbed.h"
 
#include "mbed.h"

Serial pc(USBTX, USBRX);
Serial esp(PA_9, PA_10);

Timer t; 

#define	BUFFER_SIZE		2048

int count, ended, timeout;
char buf[BUFFER_SIZE]; 		// buffer for terminal message output
char linebuf[512];
char snd[255]; 			// buffer for trasmitting data to ESP8266
char ssid[32] = "SSILLA30"; 	// Wi-Fi AP SSID
char pwd[32] = "silla1234"; 	// Wi-Fi AP Password

// function protypes
void SendCMD(), getreply(), ESPconfig();

int main()
{
	t.start();
	
	pc.baud(115200);
	pc.printf("\f\n\r-------------ESP8266 Hardware Reset ------------- \n\r");
	wait(0.5);

  esp.baud(115200);
  ESPconfig(); 		// ESP8266 module configuration 

  while(1) {
    pc.printf("\n---------- Listing Acces Points ----------\r\n");
    strcpy(snd, "AT+CWLAP\r\n");
    SendCMD();
    timeout=15;
    getreply();
//    pc.printf(buf);
    wait(2);
    
    pc.printf("\n---------- Get IP and MAC ----------\r\n");
    strcpy(snd, "AT+CIFSR\r\n");
    SendCMD();
    timeout=10;
    getreply();
//    pc.printf(buf);
    wait(2);
  }
} 

void SendCMD() {
	char *cp = snd;
	while (*cp != '\0') {
//		pc.putc(*cp);
		esp.putc(*cp++);
	}
}

void getreply() {
/*	
	int cnt = 0;
	char *rp = buf;
	
	t.reset();
	
	while (t.read() < (float)timeout && (cnt < BUFFER_SIZE)) {
		if (esp.readable()) {
			while (esp.readable()) {
				*rp++ = esp.getc();
				if (++cnt >= BUFFER_SIZE)  break;
			}
			t.reset();
		}
	}
	*rp = '\0';	
//	pc.printf("%s", buf);
*/	
	
	int cnt = 0;
	char *rp = buf;
	char ch;
	
	int ok = 0;
	t.reset();
	
	while ((t.read() < (float)timeout) && !ok) {
		if (esp.readable()) {
			while (esp.readable()) {
				ch = esp.getc();
				*rp++ = ch;
				if (ch == '\n') {
					*rp = '\0';
					if (!strcmp(buf, "OK\r\n"))
						ok = 1;
					pc.printf("%s", buf);
					rp = buf;
				}
			}
			t.reset();
		}
	}
	
	if (!ok) {
		pc.printf("Timeout in waiting for replay...\n");
	}
}	

void ESPconfig() {
  wait(5);
  strcpy(snd,"AT\r\n");
  SendCMD();
  wait(1);
  strcpy(snd,"AT\r\n");
  SendCMD();
  wait(1);
  strcpy(snd,"AT\r\n");
  SendCMD();
  timeout=1;
  getreply();
  wait(1);

  pc.printf("\f---------- Starting ESP Config ----------\r\n\n");
  pc.printf("---------- Reset & get Firmware ----------\r\n");
  strcpy(snd,"AT+RST\r\n");
  SendCMD();
  timeout=5;
  getreply();
//  pc.printf(buf);
  wait(2);

  pc.printf("\n---------- Get Version ----------\r\n");
  strcpy(snd,"AT+GMR\r\n"); // ?? ??
  SendCMD();
  timeout=4;
  getreply();
//  pc.printf(buf);
  wait(3);

  // set CWMODE to 1=Station,2=AP,3=BOTH, default mode 1 (Station)
  pc.printf("\n---------- Setting Mode ----------\r\n");
  strcpy(snd, "AT+CWMODE=3\r\n"); // Wi-Fi ?? ??
  SendCMD();
  timeout=4;
  getreply();
//  pc.printf(buf);
  wait(2);

  // set CIPMUX to 0=Single,1=Multi
  pc.printf("\n---------- Setting Connection Mode ----------\r\n");
  strcpy(snd, "AT+CIPMUX=1\r\n");
  SendCMD();
  timeout=4;
  getreply();
//  pc.printf(buf);
  wait(2);

  pc.printf("\n---------- Listing Access Points ----------\r\n");
  strcpy(snd, "AT+CWLAP\r\n");
  SendCMD(); 
  timeout=15;
  getreply();
//  pc.printf(buf);
  wait(2);

  pc.printf("\n---------- Connecting to AP ----------\r\n");
  pc.printf("ssid = %s pwd = %s\r\n",ssid,pwd);
  strcpy(snd, "AT+CWJAP=\"");
  strcat(snd, ssid);
  strcat(snd, "\",\"");
  strcat(snd, pwd);
  strcat(snd, "\"\r\n");
  SendCMD();
  timeout=10;
  getreply();
//  pc.printf(buf);
  wait(5);

  pc.printf("\n---------- Get IP's ----------\r\n");
  strcpy(snd, "AT+CIFSR\r\n");
  SendCMD();

  timeout=3;
  getreply();
//  pc.printf(buf);
  wait(1);

  pc.printf("\n---------- Get Connection Status ----------\r\n");
  strcpy(snd, "AT+CIPSTATUS\r\n"); // ???? ??
  SendCMD();
  timeout=5;
  getreply();
  pc.printf(buf);
  pc.printf("\n\n\n If you get a valid (non zero) IP, ESP8266 has been set up.\r\n");
  pc.printf(" Run this if you want to reconfig the ESP8266 at any time.\r\n");
  pc.printf(" It saves the SSID and password settings internally\r\n");
  wait(10);
}
