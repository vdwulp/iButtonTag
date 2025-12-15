// SA van der Wulp    | April 14, 2025
// Copyright (c) 2025 | MIT License
// https://vdwulp.github.io/iButtonTag


#include <iButtonTag.h>                     // Include the library

#define PIN_PROBE 2                         // Data line connected to pin 2

iButtonTag ibutton( PIN_PROBE );            // Setup iButtonTag on the pin

// Variables to store ID-codes before/after
iButtonCode oldcode, currentcode;

// Variable to store new ID-code, last byte will be overwritten later. Change
// the byte-values to the code you like! The last byte may not be correct - it
// has to be a checksum of the other bytes - but we'll change that later on.
iButtonCode newcode = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF };

/*
 * Some iButton tags are (re)writeble. The iButtonTag library supports writing a
 * new identification code to models like RW1990, RW1990.1, ТM08, ТM08v2,
 * RW1990v2, RW1990.2, RW2004, TM2004, TM01 and TM01C.
 *
 * This example shows how a new identification code is written to these iButton
 * tag models. Because we want the new identification code written to the
 * iButtonTag only once, all example code is in the setup function.
 *
 * CAUTION: Some writable iButton tags are only **write-once**. Be aware of the
 *          risk in 'just' trying to write a random new code. Also, make sure
 *          there is just one probe or iButton connected to the data line when
 *          trying to write.
 */
void setup(void) {

  // Start serial port
  Serial.begin(9600);
  Serial.println("iButtonTag Library Demo");
  Serial.println();

  // Try to read old iButtonCode until a tag is present
  while ( ibutton.readCode( oldcode ) == 0 ) delay( 100 );
  Serial.print( "Old code on iButton tag     : " );
  ibutton.printCode( oldcode );
  Serial.println();

  // Update last byte of new ID-code to correct checksum
  ibutton.updateChecksum( newcode );
  Serial.print( "New code for iButton tag    : " );
  ibutton.printCode( newcode );
  Serial.println();

  // THE NEXT LINE OF CODE IS THE MOST IMPORTANT LINE OF THIS EXAMPLE!
  //
  // .. everything before/after is just reading, preparing, comparing and
  // printing. The function 'writeCode' will try to detect the specific type of
  // (re)writable iButton tag automatically.

  // Try to write the new ID-code
  int8_t status = ibutton.writeCode( newcode );

  // iButton tag type TM01 (including model TM01C) is non-detectable and _can_
  // be written. To write a new code to such a tag the specific type needs to be
  // passed. Replace the line above with this one, without comment slashes:
  //
  //   int8_t status = ibutton.writeCode( newcode, IBUTTON_TM01 );

  // Evaluate success/failure based on returned status
  Serial.print( "Writing procedure finished " );
  if ( status == 1 ) Serial.print( "successfully");
  else Serial.print( "with an error" ) ;
  Serial.print( " (status " );
  Serial.print( status );
  Serial.println( ")" );

  // Read current iButtonCode from tag
  while ( ibutton.readCode( currentcode ) == 0 ) delay( 100 );
  Serial.print( "Current code on iButton tag : " );
  ibutton.printCode( currentcode );
  Serial.println();

  // Evaluate success/failure based on iButtoncode read from tag
  Serial.print( "Current code on tag *is" );
  if ( !ibutton.equalCode( oldcode, currentcode ) ) Serial.print( " not" );
  Serial.println( "* the same as before writing" );

  Serial.print( "Current code on tag *is" );
  if ( !ibutton.equalCode( newcode, currentcode ) ) Serial.print( " not" );
  Serial.println( "* the same as new code supplied" );

}

/*
 * We want the new identification code written to the iButtonTag only once.
 * Therefore, all example code is in the setup function. No looping.
 */
void loop(void) {

  // Empty

}

/* SAMPLE OUTPUT >> Rewritable iButton tag model RW1990
----------------------------------------------------------
iButtonTag Library Demo

Old code on iButton tag     : 01 0B 15 1F 29 33 3D E8
New code for iButton tag    : 01 23 45 67 89 AB CD E9
Writing procedure finished successfully (1)
Current code on iButton tag : 01 23 45 67 89 AB CD E9
Current code on tag *is not* the same as before writing
Current code on tag *is* the same as new code supplied
----------------------------------------------------------
*/

/* SAMPLE OUTPUT >> NON-writable iButton tag model TM1990A
----------------------------------------------------------
iButtonTag Library Demo

Old code on iButton tag     : 01 D9 35 C4 01 00 00 A5
New code for iButton tag    : 01 23 45 67 89 AB CD E9
Writing procedure finished with an error (status -12)
Current code on iButton tag : 01 D9 35 C4 01 00 00 A5
Current code on tag *is* the same as before writing
Current code on tag *is not* the same as new code supplied
----------------------------------------------------------
*/
