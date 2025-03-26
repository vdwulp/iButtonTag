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

    // Static functions
    static void reverseCode( uint8_t* );
    static void reverseCode( const uint8_t*, uint8_t* );

};

#endif // iButtonTag_h
