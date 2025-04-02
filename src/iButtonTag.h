// SA van der Wulp    | April 2, 2025
// Copyright (c) 2025 | MIT License
// https://github.com/vdwulp/iButtonTag

#ifndef iButtonTag_h
#define iButtonTag_h

// Includes
#include <inttypes.h>
#include <OneWire.h>

// Constants for iButton (re)writable tag types
const int8_t IBUTTON_UNKNOWN  = 0;
const int8_t IBUTTON_RW1990V1 = 1; // Models sold as RW1990, RW1990.1, ТМ08, ТМ08v2
const int8_t IBUTTON_RW1990V2 = 2; // Model sold as RW1990v2, RW1990.2
const int8_t IBUTTON_RW2004   = 3; // Models sold as RW2004, TM2004
const int8_t IBUTTON_TM01     = 4; // Model sold as TM01, TM01C - Non-detectable
const int8_t IBUTTON_MAXTYPE  = 4; // Always equal to maximum type constant

// Type definition
typedef uint8_t iButtonCode[8];

class iButtonTag {

  public:
    // Constructor / destructor
    iButtonTag( uint8_t );
    ~iButtonTag() { delete _wire; };

    // Functions
    int8_t readCode( uint8_t*, bool = false );
    int8_t readCodes();
    int8_t nextCode( uint8_t* );

    // Static functions
    static int8_t testCode( const uint8_t* );
    static bool equalCode( const uint8_t*, const uint8_t* );
    static void printCode( const uint8_t*, bool = false );
    static void updateChecksum( uint8_t* );

    // Functions for writing
    int8_t detectWritableType();
    int8_t writeCode( const uint8_t*, uint8_t = IBUTTON_UNKNOWN, bool = true );

  private:
    // OneWire instance
    OneWire* _wire;

    // Functions for writing
    int8_t isWritableTypeRW1990V1();
    int8_t isWritableTypeRW1990V2();
    int8_t isWritableTypeRW2004();
    int8_t writeCodeCommon( const uint8_t*, uint8_t, bool = false );
    int8_t writeCodeRW2004( const uint8_t* );
    void writeByteDelayed( uint8_t, bool invert = false );
    void writeBitDelayed( uint8_t );

    // Static functions
    static uint8_t calculateChecksum( const uint8_t* );

};

#endif // iButtonTag_h
