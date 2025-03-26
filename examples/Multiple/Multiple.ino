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
 * Main function, read identification code of possibly _multiple_ iButton tags.
 *
 * It's possible to connect multiple iButton probes to just one pin. If there
 * multiple probes connected there is a chance multiple iButtons are presented
 * at once. In this case the identification codes must be read in a different
 * way.
 */
void loop(void)
{
  // Variable to store identification code
  iButtonCode code;

  // Start the search for identification codes
  Serial.println( "Reading... " );
  int8_t status = ibutton.readCodes(); // The plural _codes_ in this function
                                       // name indicates we're searching for
                                       // (possibly) multiple codes.
  if ( status == 0 ) {
    Serial.println("No iButton detected");
    return;
  }
  
  // Keep trying to get next code until status indicates there are no more codes
  while ( status = ibutton.nextCode( code ) != 0 ) {

    // Variable /status/ will now indicate the succes
    switch( status ) {
      
      case 1: // Succes
        Serial.print( "iButton code read succesfully: " );
        ibutton.printCode( code ); // Variable _code_ contains the ID-code
        Serial.println();
        break;

      case 0: // No iButton
        Serial.println( "No iButton detected" );
        break;

      case -1: // Checksum failed
      case -2: // Code all zeros
        Serial.println( "iButton code invalid" );
        break;

      default: // Unknown - Shouldn't happen
        Serial.println( "Unknown status" );

    }

  }

}
