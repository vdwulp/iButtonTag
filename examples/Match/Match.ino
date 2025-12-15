// SA van der Wulp    | April 14, 2025
// Copyright (c) 2025 | MIT License
// https://vdwulp.github.io/iButtonTag


// Include the library
#include <iButtonTag.h>

// Data wire of the iButton probe is connected to pin 2 on the Arduino
#define PIN_PROBE 2

// Setup iButtonTag on the pin
iButtonTag ibutton( PIN_PROBE );

/*
 * The setup function.
 */
void setup( void ) {

  // Start serial port
  Serial.begin( 9600 );
  Serial.println( "iButtonTag Library Demo" );

}

/*
 * Main function, read identification code of an iButton tag and check for match
 */
void loop(void)
{
  
  // Variable to store identification code
  iButtonCode code;

  // Pre-defined code to match, change to the code _you_ want to match!
  iButtonCode matchcode = { 0x01, 0x5F, 0x94, 0xC5, 0x01, 0x00, 0x00, 0x8C };

  // Try to read an identification code from the probe
  Serial.println( "Reading... " );
  int8_t status = ibutton.readCode( code );

  if ( status > 0 ) { // iButton code read successfully
    
    Serial.print( "iButton code read: " );
    ibutton.printCode( code ); // Variable _code_ contains the ID-code
    
    // Compare identification code read from the probe to the pre-defined code
    if ( ibutton.equalCode( code, matchcode ) ) {
      Serial.println( " - MATCH FOUND!" );
    } else {
      Serial.println( " - not a match" );
    }

  }

}
