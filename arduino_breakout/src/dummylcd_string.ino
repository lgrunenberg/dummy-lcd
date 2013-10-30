

#include "dummylcd.h"

dummylcd LCDA;

void setup() {

  
  // Initialize the LCD Screen
  
  LCDA.initLcd();
  
  // Set the Text only cursor to desired values
  
  LCDA.setCursor(10,20, 0x05E0, 0x0000);
  
  // Let's print some text, null terminated
  
  LCDA.print("Lorem ipsum" + '\0');

}


void loop() {

}


