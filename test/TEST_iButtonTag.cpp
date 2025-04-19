// SA van der Wulp    | April 14, 2025
// Copyright (c) 2025 | MIT License
// https://vdwulp.github.io/iButtonTag


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
  assertEqual( 0, ibutton.readCode( code ) );         // No iButton detected
  assertTrue( ibutton.equalCode( code, codezero ) );

  assertEqual( 0, ibutton.readCode( code, false ) );  // No iButton detected
  assertTrue( ibutton.equalCode( code, codezero ) );

  assertEqual( 0, ibutton.readCode( code, true ) );   // No iButton detected
  assertTrue( ibutton.equalCode( code, codezero ) );

  // Function readCodes
  assertEqual( 0, ibutton.readCodes() );              // No iButton detected

  // Function nextCode
  assertEqual( 0, ibutton.nextCode( code ) );         // No iButton detected
  assertTrue( ibutton.equalCode( code, codezero ) );

  // Function testCode
  assertEqual( -2, ibutton.testCode( code ) );        // Invalid code, all zeros
  assertEqual( -2, ibutton.testCode( codezero ) );    // Invalid code, all zeros
  assertEqual( -1, ibutton.testCode( codecrcfail ) ); // Invalid code, CRC failed
  assertEqual( 1, ibutton.testCode( codecrc ) );      // Valid code

  // Function equalCode
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

  // Function detectWritableType
  assertEqual( -1, ibutton.detectWritableType() );

  // Function writeCode
  assertEqual(   0, ibutton.writeCode( codecrc ) );
  assertEqual(  -1, ibutton.writeCode( codecrcfail ) );
  assertEqual(  -2, ibutton.writeCode( codezero ) );
  assertEqual(   0, ibutton.writeCode( codecrc, IBUTTON_UNKNOWN ) );
  assertEqual(   0, ibutton.writeCode( codecrc, IBUTTON_RW1990V1 ) );
  assertEqual(   0, ibutton.writeCode( codecrc, IBUTTON_RW1990V2 ) );
  assertEqual(   0, ibutton.writeCode( codecrc, IBUTTON_RW2004 ) );
  assertEqual(   0, ibutton.writeCode( codecrc, IBUTTON_TM01 ) );
  assertEqual( -11, ibutton.writeCode( codecrc, IBUTTON_MAXWRITABLE + 1 ) );
  assertEqual(   0, ibutton.writeCode( codecrcfail, IBUTTON_UNKNOWN, false ) );
  assertEqual(   0, ibutton.writeCode( codecrcfail, IBUTTON_RW1990V1, false ) );
  assertEqual(   0, ibutton.writeCode( codecrcfail, IBUTTON_RW1990V2, false ) );
  assertEqual(   0, ibutton.writeCode( codecrcfail, IBUTTON_RW2004, false ) );
  assertEqual(   0, ibutton.writeCode( codecrcfail, IBUTTON_TM01, false ) );
  assertEqual( -11, ibutton.writeCode( codecrcfail, IBUTTON_MAXWRITABLE + 1, false ) );

  // Function updateChecksum - last because it changes codecrcfail
  ibutton.updateChecksum( codecrcfail );
  assertTrue( ibutton.equalCode( codecrc, codecrcfail ) );

}

unittest_main()
