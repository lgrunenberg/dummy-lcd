
#pragma once
#include <avr/pgmspace.h>
#include <inttypes.h>
#include <Arduino.h>



typedef unsigned char byte;


class dummylcd {



public:

dummylcd();

byte swapBits(byte b);
void writeCommand(byte data);
void writeData(int data);
void initLcd(void);
void setWindow(int x, int y, int width, int height);
void fillRect(int x, int y, int width, int height, int color);
void setPixel(int color);
void setCursor(int x, int y, int fgcolor, int bgcolor);
void print(char *string);


}; 




