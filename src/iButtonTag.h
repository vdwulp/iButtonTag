// SA van der Wulp    | March 26, 2025
// Copyright (c) 2025 | MIT License
// https://github.com/vdwulp/iButtonTag

#ifndef iButtonTag_h
#define iButtonTag_h

// Includes
#include <inttypes.h>
#include <OneWire.h>

// Type definition
typedef uint8_t iButtonCode[8];

class iButtonTag {

  public:
    // Constructor
    iButtonTag( uint8_t );
    
    // Functions
    int8_t readCode( uint8_t*, bool = false );
    int8_t readCodes();
    int8_t nextCode( uint8_t* );

    // Static functions
    static int8_t testCode( const uint8_t* );
    static bool equalCode( const uint8_t*, const uint8_t* );
    static void printCode( const uint8_t*, bool = false );

  private:
    // OneWire instance
    OneWire* _wire;

};

#endif // iButtonTag_h
