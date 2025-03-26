#include "iButtonTag.h"

// Constructs an iButtonTag object linked to the supplied pin.
iButtonTag::iButtonTag( uint8_t pin ) {
  _wire = new OneWire( pin );
}

// Reads one single iButton identifying code from the OneWire.
//
// When multiple iButtons are connected to the OneWire this function will return
// an invalid reading because of a CRC8 failure (return value -2) caused by
// colliding responses. If there is _any_ possibility multiple iButtons are
// connected, use _getCodes_ instead.
//
// DS1990 iButton tags can't be used with multiple tags on a single 1-Wire bus
// and require special handling. This function facilitates compatibility with
// the DS1990 iButton Tags when argument _old_ is set to true. However, this
// reduces compatibility with other iButton tags: DS1990A, DS1990R and TM1990A
// will still be handled correctly (they offer backwards compatibility), but
// other 1-Wire devices (including iButtons) won't and may even show unexpected
// behaviour.
//
// Return values:
//    1 - Next iButton read succesfully, code array filled with identifying code
//    0 - No more iButtons detected, code array is unchanged
//   -1 - Invalid iButton code read, CRC8 failed, code array with invalid bytes
//   -2 - Invalid iButton code read, all zeros, code array with invalid bytes
int8_t iButtonTag::readCode( uint8_t* code, bool old /* = false */ ) {
  // RESET the OneWire
  // - Connected devices will assert presence with a pulse
  // - Returns 1 if at least one device is present, 0 otherwise
  // - Exit with status 0 when no device asserted presence
  int b = _wire -> reset();
  if ( b == 0 ) return 0;

  // Issue READ ROM command to the OneWire
  // - 0x33 is protocol standard
  // - 0x0F for compatibility with DS1990
  _wire -> write( old ? 0x0F : 0x33 );
  // Read 8 bytes of identifying code
  for ( uint8_t i = 0; i < 8; i++ ) code[i] = _wire -> read();

  // Test the identifying code and return result
  return testCode( code );
}

// Starts the search for multiple iButton identifying codes on the OneWire.
//
// Resets the domain to search for iButton identifying codes. This function is
// needed to start searching for codes _again_. It's not really needed the first
// time, though it's good practice to always use it before enumerating codes
// with the nextCode function.
//
// Return values:
//    1 - At least one iButton detected, enumerate with nextCode function
//    0 - No iButton detected
int8_t iButtonTag::readCodes() {
  // RESET the OneWire
  // - Connected devices will assert presence with a pulse
  // - Returns 1 if at least one device is present, 0 otherwise
  // - Exit with status 0 when no device asserted presence
  int b = _wire -> reset();
  if ( b == 0 ) return 0;

  // Reset search domain on OneWire
  _wire -> reset_search();

  return 1;
}

// Continues the search for multiple iButton identifying codes on the OneWire.
//
// Start the search for multiple iButton identyfying codes with the readCodes
// function. The is this function to enumerate all iButton identifying codes on
// the OneWire.
//
// A return value 0 means searching finished succesfully, but there are no more
// iButtons on the OneWire. Negative return values indicate a problem during the
// search (mostly due to movement of the iButton on the reader), but additional
// calls to the function _may_ yield new iButton identifying codes. However, the
// overall result will be unreliable.
//
// Return values:
//    1 - Next iButton read succesfully, code array filled with identifying code
//    0 - No more iButtons detected, code array is unchanged
//   -1 - Invalid iButton code read, CRC8 failed, code array with invalid bytes
//   -2 - Invalid iButton code read, all zeros, code array with invalid bytes
int8_t iButtonTag::nextCode( uint8_t* code ) {
  // Search for the next identification code on OneWire
  // - Returns 1 when a code is found, 0 when there are no more iButtons
  // - Exit with status 0 when no more iButtons are detected
  int b = _wire -> search( code );
  if ( b == 0 ) return 0;

  // Test the identifying code and return result
  return testCode( code );
}

// Tests iButtonCode for validity.
//
// Return values:
//    1 - iButton code valid
//   -1 - iButton code invalid, CRC8 failed
//   -2 - iButton code invalid, all zeros
int8_t iButtonTag::testCode( const uint8_t* code ) {
  // Check if last byte of code is CRC8 of first 7 bytes
  // - Exit with status -1 when CRC8 fails
  if ( OneWire::crc8( code, 7 ) != code[7] ) return -1;

  // First byte read is a FAMILY CODE. Some code found online implies all values
  // other than 0x01 are invalid. The documentation doesn't support this view:
  // it doesn't even say values 0xFF (observed when no iButton is present) or
  // 0x00 (observed when reader is shorted) are invalid. In theory these values
  // are possible, as long as an iButton asserts presence in response to the
  // OneWire RESET command.
  // However, sliding around an iButton on a reader may lead to a code of 8 0x00
  // bytes being 'succesfully' read (how? apparently reset is succesfull, then a
  // short occurs?). Then, the CRC8 is also correct (0x00), so this case needs
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

// Tests if two iButtonCode's are equal.
//
// Return values:
//   true - The two iButtonCode's are equal
//   false - The two iButtonCode's are *NOT* equal
bool iButtonTag::equalCode( const uint8_t* a, const uint8_t* b ) {
  for( uint8_t i = 0; i < 8; i++ ) if ( a[i] != b[i] ) return false;
  return true;
}

// Prints iButton identifying code to Serial as hexadecimal byte values. 
//
// Serial must be initialised in the main code first. By default the bytes are
// printed as received from the iButton (reverse = false). The order can be
// reversed (reverse = true) to match the sequence fysically engraved on many
// iButtons.
void iButtonTag::printCode( const uint8_t* code, bool reverse /* = false */ ) {
  if ( reverse ) reverseCode( code );
  for( uint8_t i = 0; i < 8; i++ ) {
    if ( code[i] < 0x10 ) Serial.print( "0" );
    Serial.print( code[i], HEX );
    if ( i < 7 ) Serial.print( " " );
  }
  if ( reverse ) reverseCode( code );
}

// Reverses iButtonCode bytes.
//
// All bytes of iButtonCode are _changed_ to reversed order.
void iButtonTag::reverseCode( uint8_t* code ) {
  uint8_t tmp;
  for( uint8_t i = 0; i < 4; i++ ) {
    tmp = code[i];
    code[i] = code[7-i];
    code[7-i] = tmp;
  }
}

// Reverses iButtonCode bytes in a different array.
//
// All bytes of _original_ iButtonCode are transferred to _reversed_ iButtonCode
// in reversed order.
void iButtonTag::reverseCode( const uint8_t* original, uint8_t* reversed ) {
  for( uint8_t i = 0; i < 8; i++ ) reversed[i] = original[7-i];
}
