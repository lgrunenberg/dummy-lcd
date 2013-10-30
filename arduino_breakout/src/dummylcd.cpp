#include "dummylcd.h"

extern "C" {


#include <inttypes.h>

#include <avr/pgmspace.h>

}



int cursorX, cursorY, cursorFg, cursorBg;

#include "font.h"

dummylcd::dummylcd() {

}

// Function to mirror a Byte, used for Arduino breakout board

byte dummylcd::swapBits(byte b) {
  b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
  b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
  b = (b & 0xAA) >> 1 | (b & 0x55) << 1;

  return b;
}

// Write a Command to the Display Port

void dummylcd::writeCommand(byte data) {
  PORTG &= ~(1<<2);

  PORTA = data;

  PORTD |= (1<<7);

  PORTD &= ~(1<<7);
}

// Write a Data Word to the Display Port

void dummylcd::writeData(int data) {
  PORTG |= (1<<2);

  PORTA = data;
  PORTC = swapBits(data >> 8);

  PORTD |= (1<<7);
 
  PORTD &= ~(1<<7);
}



void dummylcd::initLcd(void) {

  PORTA = 0x00;
  PORTC = 0x00;
  DDRC = 0xFF;
  DDRA = 0xFF;
  
  // set CS pin LOW
  
  PORTG &= ~(1<<0);
  DDRG |= (1<<0);
  
  // set RS pin LOW
  
  PORTG &= ~(1<<2);
  DDRG |= (1<<2);
  
  // set RESET pin LOW
  
  PORTG &= ~(1<<1);
  DDRG |= (1<<1);
  
  // set WR pin LOW
  
  PORTD &= ~(1<<7);
  DDRD |= (1<<7);
  
  PORTG |= (1<<1);
  delay(5);
  PORTG &= ~(1<<1);
  delay(15);
  PORTG |= (1<<1);
  delay(100);



  writeCommand(0xE9);       // DBI Dummy Command 
  writeData(0x20);

  writeCommand(0x11);        //  Exit Sleep
  delay(100);

  writeCommand(0xD0);        //  Power Setting
  writeData(0x04);
  writeData(0x07);
  writeData(0x00);

  writeCommand(0xD1);        //  VCOM Control
  writeData(0x00);
  writeData(0x71);
  writeData(0x19);

  writeCommand(0x36);        //  Address Mode
  writeData(0x28);
  
  writeCommand(0x3A);        //  Pixel Format
  writeData(0x55);
  
  writeCommand(0xC1);        //  Display Timing
  writeData(0x10);
  writeData(0x10);
  writeData(0x02);
  writeData(0x02);
  
  writeCommand(0xC0);        //  Set Default Gamma
  writeData(0x00);
  writeData(0x35);
  writeData(0x00);
  writeData(0x00);
  writeData(0x01);
  writeData(0x02);
  
  writeCommand(0xC5);         //  Set frame rate
  writeData(0x04);
  
  writeCommand(0xD2);         //  power setting
  writeData(0x01);
  writeData(0x44);
  
  writeCommand(0xC8);         //  Set Gamma
  writeData(0x04);
  writeData(0x67);
  writeData(0x35);
  writeData(0x04);
  writeData(0x08);
  writeData(0x06);
  writeData(0x24);
  writeData(0x01);
  writeData(0x37);
  writeData(0x40);
  writeData(0x03);
  writeData(0x10);
  writeData(0x08);
  writeData(0x80);
  writeData(0x00);

  writeCommand(0xB3);
  writeData( 0x02);
  writeData( 0x00);
  writeData( 0x00);
  writeData( 0x20);

  writeCommand(0x2A);         //  Set Column Address
  writeData(0x00);
  writeData(0x00);
  writeData(0x01);
  writeData(0x8F);

  writeCommand(0x2B);         //  Set Page Address
  writeData(0x00); 
  writeData(0x00);
  writeData(0x00);
  writeData(0xEF);   

  writeCommand(0x29);         // display on 

  writeCommand(0x20);         // display on 

  writeCommand(0x2C);         // start GRAM display


  // Clean the display to black
  
  for(int i = 0; i < 400; i++) {
    for(int j = 0; j < 240; j++) {
      writeData( 0x00);
    }
  }

// TODO: first clean display then activate power!

}



void dummylcd::setWindow(int x, int y, int width, int height) {
  if((width > 0) && (height > 0)) {    

    int x_end = x + width - 1;
    int y_end = y + height - 1;

    writeCommand(0x2A);         //Set Column Address
    writeData((x >> 8) & 0x01);
    writeData(x & 0xFF);
    writeData((x_end >> 8) & 0x01);
    writeData(x_end & 0xFF);
    writeCommand(0x2B);         //Set Page Address
    writeData((y >> 8) & 0x01); 
    writeData(y & 0xFF);
    writeData((y_end >> 8) & 0x01);
    writeData(y_end & 0xFF);
    writeCommand(0x2C);         //display on

  }
}

// fill a box with the specified color

void dummylcd::fillRect(int x, int y, int width, int height, int color) {

  setWindow(x, y, width, height);

  for(int i = x; i < x + width; i++) {
    for(int j = y; j <= y + width; j++) {
      writeData(color);
    }
  }
}    

// set a pixel to the specified color

void dummylcd::setPixel(int color) {
  
  writeData(color);
  
}

// set the text only cursor to desired values

void dummylcd::setCursor(int x, int y, int fgcolor, int bgcolor) {
  cursorX = x;
  cursorY = y;
  cursorFg = fgcolor;
  cursorBg = bgcolor;
}

// print a string, null terminated

void dummylcd::print(char *string) {
  
  // first we estimate the overall string length
  
  int stringlength;
  while(*string != '\0') {
    stringlength++;
    string++;
  }
  string -= stringlength;
  
  // then set the window to the the strings width
  
  setWindow(cursorX, cursorY, stringlength*8, 8);
  
  // draw the font
  
  for(int i = 0; i < 8; i++) {
    for (int j = 0; j < stringlength; j++) {
      char myChar = font[*string][i];
      for(int k = 0; k < 8; k++) {
        if((myChar & (0x80 >> k)) > 0) {
          setPixel(cursorFg);
        } else {
          setPixel(cursorBg);
        }
      }
      string++;
    }
    string -= stringlength;
  }
}



