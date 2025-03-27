// SA van der Wulp    | March 27, 2025
// Copyright (c) 2025 | MIT License
// https://github.com/vdwulp/iButtonTag

#include <ArduinoUnitTests.h>
#include <iButtonTag.h>

unittest( iButtonTag_basics ) {

  // Variables
  iButtonCode code, codezero, codecrc, codecrcfail;
  int8_t status;
  bool equal;

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
  status = ibutton.readCode( code );
  equal = ibutton.equalCode( code, codezero );
  assertEqual( 0,  status ); // No iButton detected
  assertTrue( equal );

  status = ibutton.readCode( code, false );
  equal = ibutton.equalCode( code, codezero );
  assertEqual( 0,  status ); // No iButton detected
  assertTrue( equal );

  status = ibutton.readCode( code, true );
  equal = ibutton.equalCode( code, codezero );
  assertEqual( 0,  status ); // No iButton detected
  assertTrue( equal );

  // Function readCodes
  status = ibutton.readCodes();
  assertEqual( 0,  status ); // No iButton detected

  // Function nextCode
  status = ibutton.nextCode( code );
  equal = ibutton.equalCode( code, codezero );
  assertEqual( 0,  status ); // No iButton detected
  assertTrue( equal );

  // Function testCode
  // Return values:
  //    1 - iButton code valid
  //   -1 - iButton code invalid, CRC8 failed
  //   -2 - iButton code invalid, all zeros
  status = ibutton.testCode( code );         // Invalid code, all zeros
  assertEqual( -2,  status );
  status = ibutton.testCode( codezero );     // Invalid code, all zeros
  assertEqual( -2,  status );
  status = ibutton.testCode( codecrcfail );  // Invalid code, CRC failed
  assertEqual( -1,  status );
  status = ibutton.testCode( codecrc );      // Valid code
  assertEqual( 1,  status );

  // Function equalCode
  // Return values:
  //   true - The two iButtonCode's are equal
  //   false - The two iButtonCode's are *NOT* equal
  equal = ibutton.equalCode( codezero, codecrc );
  assertFalse( equal );
  equal = ibutton.equalCode( codezero, codecrcfail );
  assertFalse( equal );
  equal = ibutton.equalCode( codecrc, codecrcfail );
  assertFalse( equal );

  equal = ibutton.equalCode( codezero, codezero );
  assertTrue( equal );
  equal = ibutton.equalCode( codecrc, codecrc );
  assertTrue( equal );
  equal = ibutton.equalCode( codecrcfail, codecrcfail );
  assertTrue( equal );

  // Function printCode
  GodmodeState* state = GODMODE();

  ibutton.printCode( codezero );
  assertEqual( "00 00 00 00 00 00 00 00", state -> serialPort[0].dataOut );
  ibutton.printCode( codecrc );
  assertEqual( "01 0B 15 1F 29 33 3D E8", state -> serialPort[0].dataOut );
  ibutton.printCode( codecrcfail );
  assertEqual( "01 0B 15 1F 29 33 3D 47", state -> serialPort[0].dataOut );

  ibutton.printCode( codecrc, false );
  assertEqual( "01 0B 15 1F 29 33 3D E8", state -> serialPort[0].dataOut );
  ibutton.printCode( codecrcfail, false );
  assertEqual( "01 0B 15 1F 29 33 3D 47", state -> serialPort[0].dataOut );

  ibutton.printCode( codecrc, true );
  assertEqual( "E8 3D 33 29 1F 15 0B 01", state -> serialPort[0].dataOut );
  ibutton.printCode( codecrcfail, true );
  assertEqual( "47 3D 33 29 1F 15 0B 01", state -> serialPort[0].dataOut );

}

unittest_main()
