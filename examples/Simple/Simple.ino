// SA van der Wulp    | March 26, 2025
// Copyright (c) 2025 | MIT License
// https://github.com/vdwulp/iButtonTag

#include <iButtonTag.h>                     // Include the library

#define PIN_PROBE 2                         // Data line connected to pin 2

iButtonTag ibutton( PIN_PROBE );            // Setup iButtonTag on the pin

void setup(void) {
  Serial.begin(9600);
}

void loop(void) {
  iButtonCode code;                         // Variable to store ID-code
  
  Serial.println( "Reading... " );
  int8_t status = ibutton.readCode( code ); // Try to read ID-code
  
  if ( status > 0 ) {                       // Code read succesfully
    Serial.print( "iButton code read: " );
    ibutton.printCode( code );              // ID-code is in variable _code_
    Serial.println();
  }
}
