// SA van der Wulp    | March 26, 2025
// Copyright (c) 2025 | MIT License
// https://github.com/vdwulp/iButtonTag

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
  Serial.begin(9600);
  Serial.println("iButtonTag Library Demo");
}

/*
 * Main function, read identification code of an iButton tag.
 */
void loop(void)
{
  // Variable to store identification code
  iButtonCode code;

  // Try to read an identification code from the probe
  Serial.print( "Reading... " );
  int8_t status = ibutton.readCode( code );

  // Variable /status/ will now indicate the succes
  switch( status ) {

    case 1: // Succes
      Serial.print( "iButton code read succesfully: " );
      ibutton.printCode( code, true ); // Variable _code_ contains the ID-code
      Serial.println();
      break;

    case 0: // No iButton
      Serial.println( "No iButton detected" );
      break;

    case -1: // Checksum invalid
    case -2: // Code all zeros
      Serial.println( "iButton code invalid" );
      break;

    default: // Unknown - Shouldn't happen
      Serial.println( "Unknown status" );

  }

}
