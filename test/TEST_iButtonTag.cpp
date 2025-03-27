// SA van der Wulp    | March 27, 2025
// Copyright (c) 2025 | MIT License
// https://github.com/vdwulp/iButtonTag

#include <ArduinoUnitTests.h>
#include <iButtonTag.h>

unittest( iButtonTag_basics ) {

  // Variables
  iButtonCode code, codezero, codecrc, codecrcfail;

  // Fill predefined codes
  for( uint8_t i = 0; i < 8; i++ ) {
    code[i] = 0x00;
    codezero[i] = 0x00;
    codecrc[i] = 10 * i + 1;
    codecrcfail[i] = 10 * i + 1;
  }
  codecrc[7] = OneWire::crc8( codecrc, 7 );

  // Simulate iButtonTag on PIN 2
  iButtonTag ibutton( 2 );

  // Function readCode
  // Return values:
  //    1 - Next iButton read succesfully, code array filled with identifying code
  //    0 - No more iButtons detected, code array is unchanged
  //   -1 - Invalid iButton code read, CRC8 failed, code array with invalid bytes
  //   -2 - Invalid iButton code read, all zeros, code array with invalid bytes
  assertEqual( 0, ibutton.readCode( code ) );         // No iButton detected
  assertTrue( ibutton.equalCode( code, codezero ) );

  assertEqual( 0, ibutton.readCode( code, false ) );  // No iButton detected
  assertTrue( ibutton.equalCode( code, codezero ) );

  assertEqual( 0, ibutton.readCode( code, true ) );   // No iButton detected
  assertTrue( ibutton.equalCode( code, codezero ) );

  // Function readCodes
  // Return values:
  //    1 - At least one iButton detected, enumerate with nextCode function
  //    0 - No iButton detected
  assertEqual( 0, ibutton.readCodes() );              // No iButton detected

  // Function nextCode
  // Return values:
  //    1 - Next iButton read succesfully, code array filled with identifying code
  //    0 - No more iButtons detected, code array is unchanged
  //   -1 - Invalid iButton code read, CRC8 failed, code array with invalid bytes
  //   -2 - Invalid iButton code read, all zeros, code array with invalid bytes
  assertEqual( 0, ibutton.nextCode( code ) );         // No iButton detected
  assertTrue( ibutton.equalCode( code, codezero ) );

  // Function testCode
  // Return values:
  //    1 - iButton code valid
  //   -1 - iButton code invalid, CRC8 failed
  //   -2 - iButton code invalid, all zeros
  assertEqual( -2, ibutton.testCode( code ) );        // Invalid code, all zeros
  assertEqual( -2, ibutton.testCode( codezero ) );    // Invalid code, all zeros
  assertEqual( -1, ibutton.testCode( codecrcfail ) ); // Invalid code, CRC failed
  assertEqual( 1, ibutton.testCode( codecrc ) );      // Valid code

  // Function equalCode
  // Return values:
  //   true - The two iButtonCode's are equal
  //   false - The two iButtonCode's are *NOT* equal
  assertFalse( ibutton.equalCode( codezero, codecrc ) );
  assertFalse( ibutton.equalCode( codezero, codecrcfail ) );
  assertFalse( ibutton.equalCode( codecrc, codecrcfail ) );

  assertTrue( ibutton.equalCode( codezero, codezero ) );
  assertTrue( ibutton.equalCode( codecrc, codecrc ) );
  assertTrue( ibutton.equalCode( codecrcfail, codecrcfail ) );

  // Function printCode
  GodmodeState* state = GODMODE();

  state -> serialPort[0].dataOut = "";
  ibutton.printCode( codezero );
  assertEqual( "00 00 00 00 00 00 00 00", state -> serialPort[0].dataOut );
  state -> serialPort[0].dataOut = "";
  ibutton.printCode( codecrc );
  assertEqual( "01 0B 15 1F 29 33 3D E8", state -> serialPort[0].dataOut );
  state -> serialPort[0].dataOut = "";
  ibutton.printCode( codecrcfail );
  assertEqual( "01 0B 15 1F 29 33 3D 47", state -> serialPort[0].dataOut );

  state -> serialPort[0].dataOut = "";
  ibutton.printCode( codecrc, false );
  assertEqual( "01 0B 15 1F 29 33 3D E8", state -> serialPort[0].dataOut );
  state -> serialPort[0].dataOut = "";
  ibutton.printCode( codecrcfail, false );
  assertEqual( "01 0B 15 1F 29 33 3D 47", state -> serialPort[0].dataOut );

  state -> serialPort[0].dataOut = "";
  ibutton.printCode( codecrc, true );
  assertEqual( "E8 3D 33 29 1F 15 0B 01", state -> serialPort[0].dataOut );
  state -> serialPort[0].dataOut = "";
  ibutton.printCode( codecrcfail, true );
  assertEqual( "47 3D 33 29 1F 15 0B 01", state -> serialPort[0].dataOut );

}

unittest_main()
