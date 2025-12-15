// SA van der Wulp    | April 14, 2025
// Copyright (c) 2025 | MIT License
// https://vdwulp.github.io/iButtonTag


/*
 * Reference documentation available in doc-folder of library. Only short
 * descriptions in this source file. Full documentation can be viewed online
 * via: https://vdwulp.github.io/iButtonTag/REFERENCE.html
 */


#include "iButtonTag.h"


// PUBLIC FUNCTIONS

/*
 * Constructs an iButtonTag object linked to the supplied pin.
 *
 * See https://vdwulp.github.io/iButtonTag/REFERENCE.html#constructor
 */
iButtonTag::iButtonTag( uint8_t pin ) {
  _wire = new OneWire( pin );
}

/*
 * Reads one single iButtonCode from the data line.
 *
 * See https://vdwulp.github.io/iButtonTag/REFERENCE.html#readCode
 */
int8_t iButtonTag::readCode( uint8_t* code, bool old /* = false */ ) {
  // RESET the data line
  // - Connected devices will assert presence with a pulse
  // - Returns 1 if at least one device is present, 0 otherwise
  // - Exit with status 0 when no device asserted presence
  if ( _wire -> reset() == 0 ) return 0;

  // Issue READ ROM command to the data line
  // - 0x33 is protocol standard
  // - 0x0F for compatibility with DS1990
  _wire -> write( old ? 0x0F : 0x33 );
  // Read 8 bytes of identifying code
  for ( uint8_t i = 0; i < 8; i++ ) code[i] = _wire -> read();

  // Test the identifying code and return result
  return testCode( code );
}

/*
 * Starts the search for multiple iButtonCode's on the data line.
 *
 * See https://vdwulp.github.io/iButtonTag/REFERENCE.html#readCodes
 */
int8_t iButtonTag::readCodes() {
  // RESET the data line
  // - Connected devices will assert presence with a pulse
  // - Returns 1 if at least one device is present, 0 otherwise
  // - Exit with status 0 when no device asserted presence
  if ( _wire -> reset() == 0 ) return 0;

  // Reset search domain on data line
  _wire -> reset_search();

  return 1;
}

/*
 * Continues the search for multiple iButtonCode's on the data line.
 *
 * See https://vdwulp.github.io/iButtonTag/REFERENCE.html#nextCode
 */
int8_t iButtonTag::nextCode( uint8_t* code ) {
  // Search for the next iButtonCode on data line
  // - Returns 1 when a code is found, 0 when there are no more iButtons
  // - Exit with status 0 when no more iButtons are detected
  if ( _wire -> search( code ) == 0 ) return 0;

  // Test the identifying code and return result
  return testCode( code );
}

/*
 * Tests iButtonCode for validity.
 *
 * See https://vdwulp.github.io/iButtonTag/REFERENCE.html#testCode
 */
int8_t iButtonTag::testCode( const uint8_t* code ) {
  // Check if last byte of code is CRC8 of first 7 bytes
  // - Exit with status -1 when CRC8 fails
  if ( calculateChecksum( code ) != code[7] ) return -1;

  // First byte read is a FAMILY CODE. Some code found online implies all values
  // other than 0x01 are invalid. The documentation doesn't support this view:
  // it doesn't even say values 0xFF (observed when no iButton is present) or
  // 0x00 (observed when reader is shorted) are invalid. In theory these values
  // are possible, as long as an iButton asserts presence in response to the
  // RESET command.
  // However, sliding around an iButton on a reader may lead to a code of 8 0x00
  // bytes being 'successfully' read (how? apparently reset is successfull, then
  // a short occurs?). Then, the CRC8 is also correct (0x00), so this case needs
  // an extra check to raise an error status.
  // Because it _is_ theoretically possible to have a FAMILY CODE of 0x00, we
  // need to check all code positions and thus only exclude one specific case.
  //
  // Check if all bytes of code are 0x00
  // - Exit with status -2 when code is all zeros
  uint8_t i;
  for( i = 0; i < 8; i++ ) if ( code[i] != 0x00 ) break;
  if ( i == 8 ) return -2;

  // Identifying code is valid
  //   - Exit with status 1 valid
  return 1;
}

/*
 * Tests if two iButtonCode's are equal.
 *
 * See https://vdwulp.github.io/iButtonTag/REFERENCE.html#equalCode
 */
bool iButtonTag::equalCode( const uint8_t* a, const uint8_t* b ) {
  for( uint8_t i = 0; i < 8; i++ ) if ( a[i] != b[i] ) return false;
  return true;
}

/*
 * Prints iButtonCode to Serial as hexadecimal byte values.
 *
 * See https://vdwulp.github.io/iButtonTag/REFERENCE.html#printCode
 */
void iButtonTag::printCode( const uint8_t* code, bool reverse /* = false */ ) {
  for( uint8_t i = 0; i < 8; i++ ) {
    uint8_t j = reverse ? 7 - i : i;
    if ( code[j] < 0x10 ) Serial.print( "0" );
    Serial.print( code[j], HEX );
    if ( i < 7 ) Serial.print( " " );
  }
}

/*
 * Updates checksum of iButtonCode to the correct value.
 *
 * See https://vdwulp.github.io/iButtonTag/REFERENCE.html#updateChecksum
 */
void iButtonTag::updateChecksum( uint8_t* code ) {
  code[7] = calculateChecksum( code );
}

/*
 * Detects type of (re)writable iButton tag.
 *
 * See https://vdwulp.github.io/iButtonTag/REFERENCE.html#detectWritableType
 */
int8_t iButtonTag::detectWritableType() {
  int8_t t = isWritableTypeRW1990V1();
  if ( t == 1 ) return IBUTTON_RW1990V1; // Detected
  if ( t < 0 ) return t;                 // Failure

  t = isWritableTypeRW1990V2();
  if ( t == 1 ) return IBUTTON_RW1990V2; // Detected
  if ( t < 0 ) return t;                 // Failure

  t = isWritableTypeRW2004();
  if ( t == 1 ) return IBUTTON_RW2004;   // Detected
  if ( t < 0 ) return t;                 // Failure

  return IBUTTON_UNKNOWN;
}

/*
 * Writes a new iButtonCode to a (re)writable tag.
 *
 * See https://vdwulp.github.io/iButtonTag/REFERENCE.html#writeCode
 */
int8_t iButtonTag::writeCode( const uint8_t* code,
                              int8_t type /* = IBUTTON_UNKNOWN */,
                              bool check /* = true */ ) {

  int8_t val;

  // Check code if checking is on
  if ( check ) {
    val = testCode( code );
    if ( val < 1 ) return val; // Invalid code
  }

  // Check valid type argument - always
  if ( type > IBUTTON_MAXWRITABLE ) return -11;    // Type out of range

  // Find out / check writable type
  if ( type == IBUTTON_UNKNOWN ) {             // Run auto-detection routine
    type = detectWritableType();
    if ( type == IBUTTON_UNKNOWN ) return -12; // Unable to detect
    if ( type < 0 ) return 0;                  // No iButton detected
  } else if ( check ) {                        // Check type if checking is on
    switch ( type ) {
      case IBUTTON_RW1990V1:
        val = isWritableTypeRW1990V1();
        if ( val == 0 ) return -13;
        if ( val == -1 ) return 0;
        break;
      case IBUTTON_RW1990V2:
        val = isWritableTypeRW1990V2();
        if ( val == 0 ) return -13;
        if ( val == -1 ) return 0;
        break;
      case IBUTTON_RW2004:
        val = isWritableTypeRW2004();
        if ( val == 0 ) return -13;
        if ( val == -1 ) return 0;
        break;
      case IBUTTON_TM01: break;                // Non-detectable type, allow
      default: return -11;                     // Type out of range
    }
  }

  // Checks passed or turned off, run actual writing procedure
  switch ( type ) {
    case IBUTTON_RW1990V1:
      if ( writeCodeCommon( code, 0xD1, true ) == 0 ) return 0;
      break;
    case IBUTTON_RW1990V2:
      if ( writeCodeCommon( code, 0x1D ) == 0 ) return 0;
      break;
    case IBUTTON_TM01:
      if ( writeCodeCommon( code, 0xC1 ) == 0 ) return 0;
      break;
    case IBUTTON_RW2004:
      int8_t status = writeCodeRW2004( code );
      if ( status < 1 ) return status;
      break;
  }

  // Writing procedure finished, check success if checking is on
  if ( check ) {
    iButtonCode result;
    readCode( result );
    if ( !equalCode( result, code ) ) return -21;
  }

  return 1;

}


// PRIVATE FUNCTIONS

/*
 * Performs test to determine if iButton tag is of (re)writable type RW1990v1.
 *
 * This type includes iButton tag models RW1990, RW1990.1, ТM08 and ТM08v2.
 *
 * Return values:
 *    1 - iButton _is_ of (re)writable type RW1990v1
 *    0 - iButton _is not_ of (re)writable type RW1990v1
 *   -1 - No iButton detected during test
 */
int8_t iButtonTag::isWritableTypeRW1990V1() {
  // Write flag value 1 (writing disabled)
  if ( _wire -> reset() == 0 ) return -1;
  _wire -> write( 0xD1 );
  writeBitDelayed( 1 );

  // Read flag command
  if ( _wire -> reset() == 0 ) return -1;
  _wire -> write( 0xB5 );

  // Read response and determine result
  if ( _wire -> read() == 0xFE ) return 1;
  return 0;
}

/*
 * Performs test to determine if iButton tag is of (re)writable type RW1990v2.
 *
 * This type includes iButton tag models RW1990v2 and RW1990.2.
 *
 * Return values:
 *    1 - iButton _is_ of (re)writable type RW1990v2
 *    0 - iButton _is not_ of (re)writable type RW1990v2
 *   -1 - No iButton detected during test
 */
int8_t iButtonTag::isWritableTypeRW1990V2() {
  // Write flag value 1 (writing enabled)
  if ( _wire -> reset() == 0 ) return -1;
  _wire -> write( 0x1D );
  writeBitDelayed( 1 );

  // Read flag command
  if ( _wire -> reset() == 0 ) return -1;
  _wire -> write( 0x1E );

  // Read response and determine result
  if ( _wire -> read() == 0xFE ) {
    // Restore write flag value 0 (writing disabled)
    if ( _wire -> reset() == 0 ) return -1;
    _wire -> write( 0x1D );
    writeBitDelayed( 0 );
    _wire -> depower();
    return 1;
  }
  return 0;
}

/*
 * Performs test to determine if iButton tag is of (re)writable type RW2004.
 *
 * This type includes iButton tag models RW2004 and TM2004.
 *
 * Return values:
 *    1 - iButton _is_ of (re)writable type RW2004
 *    0 - iButton _is not_ of (re)writable type RW2004
 *   -1 - No iButton detected during test
 */
int8_t iButtonTag::isWritableTypeRW2004() {
  // Send command 0xAA to read status register starting address 0x00 0x00. The
  // response of RW2004/TM2004 is CRC8 of those three bytes, followed by one
  // byte from status memory.
  // Based on 'iButton Book of Standards' (fig6-10, p78) model DS1982 (also)
  // responds as tested in this function. So, this tag type needs more testing.
  //
  // If you know where to get a RW2004/TM2004 model, please contact developer!

  // Send command
  uint8_t seq[3] = { 0xAA, 0x00, 0x00 };
  if ( _wire -> reset() == 0 ) return -1;
  for ( uint8_t i = 0; i < 3; i++ ) _wire -> write( seq[i] );

  // Read response and determine result
  int8_t result = 0;
  if ( _wire -> read() == OneWire::crc8( seq, 3 ) ) { // CRC8 of command/address
    // Read another byte and reset
    _wire -> read(); // Read byte from status register
    if ( _wire -> reset() == 0 ) return -1;
    result = 1;
  }  
  return result;
}

/*
 * Writes new iButtonCode to (re)writable types RW1990V1, RW1990V2 and TM01
 * without checks.
 *
 * These types include iButton tag models RW1990, RW1990.1, ТM08, ТM08v2 (type
 * IBUTTON_RW1990V1), RW1990v2, RW1990.2 (type IBUTTON_RW1990V2), TM01 and TM01C
 * (type IBUTTON_TM01).
 *
 * When using this function, apart from the new iButtonCode, the byte-code for
 * the type-specific write-enable command needs to be supplied. For RW1990V1 all
 * written bits need to be inverted.
 *
 * Return values:
 *    1 - Writing procedure finished successfully
 *    0 - No iButton detected at some time during procedure
 */
int8_t iButtonTag::writeCodeCommon( const uint8_t* code, uint8_t enablecommand,
                                    bool invert /* = false */ ) {
  // Set flag value to [writing enabled]
  if ( _wire -> reset() == 0 ) return 0;
  _wire -> write( enablecommand );
  writeBitDelayed( invert ? 0 : 1 );
  // Write code
  if ( _wire -> reset() == 0 ) return 0;
  _wire -> write( 0xD5 );
  for ( uint8_t i = 0; i < 8; i++ )
    writeByteDelayed( invert ? ~code[i] : code[i] );
  // Set flag value [writing disabled]
  if ( _wire -> reset() == 0 ) return 0;
  _wire -> write( enablecommand );
  writeBitDelayed( invert ? 1 : 0 );
  return 1;
}

/*
 * Writes new iButtonCode to (re)writable type RW2004 without checks.
 *
 * This type includes iButton tag models RW2004 and TM2004.
 *
 * Return values:
 *    1 - Writing procedure finished successfully
 *    0 - No iButton detected at some time during procedure
 *  -22 - Writing code failed, unexpected response while writing
 */
int8_t iButtonTag::writeCodeRW2004( const uint8_t* code ) {
  // Send command 0x3C to start writing at address 0x00 0x00. Then the code is
  // written byte-by-byte: write byte > read value > send program pulse > read
  // written byte [ > write next byte >>> ]. The first byte read after each
  // write is probably CRC8, probably of command/address/data on first pass and
  // of address/data on additional passes. Much like the procedure for model
  // DS1982 command 0x55 in 'iButton Book of Standards' (fig6-11, p80). This tag
  // type needs more testing to be able to check more responses.
  //
  // If you know where to get a RW2004/TM2004 model, please contact the
  // developer!

  // Send command
  uint8_t seq[3] = { 0x3C, 0x00, 0x00 };
  if ( _wire -> reset() == 0 ) return 0;
  for ( uint8_t i = 0; i < 3; i++ ) _wire -> write( seq[i] );

  // Write code byte-by-byte
  for ( uint8_t i = 0; i < 8; i++ ) {
    _wire -> write( code[i] );   // Write byte
    _wire -> read();             // Read value - CRC8 (needs testing)
    delayMicroseconds( 600 );
    _wire -> write_bit( 1 );     // Program pulse
    delay( 50 );
    if ( _wire -> read() != code[i] ) return -22; // Confirm byte written
  }

  return 1;
}

/*
 * Writes a byte to the data line with a delay after each bit.
 *
 * By default the byte is written _as is_, if indicated all bits are inverted.
 * 
 * The data line stays _high_ after this function. Calling procedures should
 * perform other actions on the data line or _depower_.
 */
void iButtonTag::writeByteDelayed( uint8_t b, bool invert /* = false */ ) {
  // Invert byte if specified
  if ( invert ) b = ~b;
  // Write all bits with a delay LSB-first
  for ( uint8_t mask = 0x01; mask; mask <<= 1 ) {
    writeBitDelayed( ( mask & b ) ? 1 : 0 );
  }  
}

/*
 * Writes a bit to the data line with a delay after it.
 *
 * The data line stays _high_ after this function. Calling procedures should
 * perform other actions on the data line or _depower_.
 */
void iButtonTag::writeBitDelayed( uint8_t b ) { // b can only be 0 or 1 !!
  // Write as always
  _wire -> write_bit( b );
  // Delay 10ms - May need as much as 20ms? Or can do with less?
  delay( 10 );
}

/*
 * Calculates checksum of iButtonCode.
 *
 * Returns correct checksum value
 */
uint8_t iButtonTag::calculateChecksum( const uint8_t* code ) {
  return OneWire::crc8( code, 7 );
}
