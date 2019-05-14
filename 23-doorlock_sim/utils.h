#ifndef UTILS_H
#define UTILS_H

#include "mbed.h"

void initUtils();
void playNote(char note, double play_time);
uint8_t readButtonEdge();
uint8_t getButtonInput();
void displayLeds(int ledValue);
void startLed1Blinking();
void stopLed1Blinking();
void startLed3Blinking();
void stopLed3Blinking();

#endif // UTILS_H
