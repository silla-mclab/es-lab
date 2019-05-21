#include "mbed.h"

Serial pc(USBTX, USBRX);

AnalogIn photoresistor(PB_1);
Timer t; //running timer
Ticker interrupt; //recurring 2 ms interrupt 

volatile float beat;          // raw analog signal data from photoresistor
volatile int BPM = 60;        // beats per minute
volatile int beatCount = 0;   // number of beats found
volatile float currIBI = 0;   // current InterBeat Interval: the time since the last beat
volatile float lastIBI = 1;   // last InterBeat Interval: the time between beats
volatile float pastIBI[10];   // past ten IBI values
volatile float sumIBI = 0;    // sum of past ten IBI values
volatile unsigned long lastBeatTime = 0;    //time of last beat 
volatile float peak = 50;     // peak of heartbeat wave; initialized to 20 to 
															// avoid noise at startup
volatile float trough = 50;   // trough of heartbeat wave
volatile float lowerThresh = 30;  // used to reduce noise; change these values 
																	// based on amount of light in room
volatile float upperThresh = 50;  // used to reduce noise; change these values 
																	// based on amount of light in room
volatile float amp = 3;           // amplitude of heartbeat wave
volatile bool firstBeat = true;   // keeps track of first beat found
volatile bool beatFound = false;  // true when a beat is found, false in between beats
volatile bool BPMfound = false;   // true when BPM is found, false otherwise.

void findBPM(){   
    beat = photoresistor * 100;   // read the analog data from the photoresistor and multiply by 100 (amplifies small values)          
    currIBI = t.read() - lastBeatTime;  

    // avoid noise by waiting 3/5 of last IBI     
    if((beat < lowerThresh) && (currIBI > (lastIBI*3)/5)){       
        if (beat < trough){                       
            trough = beat;                         
        }
    }

    if((beat > upperThresh) && (beat > peak)){          
        peak = beat;                            
    }                                      

    // photoresistor signal surges when there is a heartbeat
    if ((currIBI > 0.25) && (beat > upperThresh) && (beatFound == false)){ 
        beatFound = true;  
        beatCount++;
        lowerThresh = beat - 30;   // adjust threshold to account for any change in lighting and avoid noise                          
        lastIBI = t.read() - lastBeatTime;  
        lastBeatTime = t.read();                

				// if this is  the first beat found, fill pastIBI values with this value 
				// until more beats are found
        if(firstBeat){                        
						firstBeat = false;                
            for(int i=0; i< 10; i++){       
                pastIBI[i] = lastIBI;                      
            }
        }
        sumIBI = lastIBI;       
        
				// shift data over one value in pastIBI array to make room for next IBI 
				// value and discard oldest value             
        for(int i=0; i< 9; i++){               
            pastIBI[i] = pastIBI[i+1];              
            sumIBI += pastIBI[i];          
        }
        pastIBI[9] = lastIBI;   // add most recent IBI value to array                          
        sumIBI = sumIBI/10;  		// average the last 10 IBI values 
        BPM = 60/sumIBI;     		// BPM = 1 minute / average IBI

        if(beatCount == 5){  		// Only report BPM after at least 5 beats are found
            BPMfound = true;      
            beatCount = 0;
        }                                             
    }

    //when the signal is less than the threshold, the beat is over 
    if (((beat < lowerThresh) && beatFound) || (currIBI > 2)){     
        beatFound = false;    	// reset the beatFound flag to find the next beat
        amp = peak - trough;  	// calculate amplitude of heartbeat wave
        upperThresh = amp*2/3 + trough;   // set upper threshold to 50% of the amplitude
        peak = upperThresh;                         
        trough = lowerThresh;
    }
		
//    __enable_irq();   //enable interrupts
} 

int main()
{
	  pc.baud(115200);    
    pc.printf("\nHeart Beat Monitor\r\n");
    wait(1);

    t.start();

		// throws interrupt every 2ms to read sensor data and calculate BPM
    interrupt.attach(&findBPM, 0.002); 
    while(1){
        if(t.read() > 10){        // wait 10 seconds to get an accurate average 
            if(BPMfound){ 
                BPMfound = false; // start calculating next BPM
                pc.printf("\nYour BPM is %3d:\r\n", BPM);
                wait(3);
                lastBeatTime = 0;
                beatCount = 0;
                t.start();        // restart timer
            }

            //No heartbeat detected wtihin 10 seconds
            else{
                pc.printf("No heartbeat detected!\r\n");
                wait(3);
                lastBeatTime = 0;
                beatCount = 0;
                t.start();       // restart timer
            }
        }
        else{
            pc.printf("Calculating...\r");
						wait(.1);
       }
    }
}
