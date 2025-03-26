# Arduino iButtonTag Library

An Arduino library handling iButton identification tags.

## Supported Devices

- DS1990
- DS1990A
- DS1990R
- TM1990A
- RW1990
- Many other iButton ID tags

## Installation

### Manual installation of library iButtonTag
_Installation of iButtonTag using the Arduino IDE Library Manager is not available at the moment, possibly in the future._
1. Download the latest release from [GitHub releases](https://github.com/vdwulp/iButtonTag/releases)
2. In Arduino IDE, go to Sketch > Include Library > Add .ZIP Library...
3. Select the downloaded ZIP file
4. Make sure to install required library OneWire too (see below)

### Installation of required library OneWire
_OneWire can be installed using the Arduino IDE Library Manager._
1. Open Arduino IDE
2. Go to Tools > Manage Libraries...
3. Search for "OneWire"
4. Click Install

## Basic Usage

1. **Hardware Setup**
   - Connect data line of iButton probe to an Arduino digital pin, this is the 1-Wire data line.
   - Connect a 4k7 kΩ pull-up resistor between the 1-Wire data line and Arduino 5V power pin.
   - Connect ground line of iButton probe to an Arduino ground (GND) pin.

2. **Code Example**
   ```cpp
   #include <iButtonTag.h>                     // Include the library

   #define PIN_PROBE 2                         // Data line connected to pin 2

   iButtonTag ibutton( PIN_PROBE );            // Setup iButtonTag on the pin

   void setup(void) {
     Serial.begin(9600);
   }

   void loop(void) {
     iButtonCode code;                         // Variable to store ID-code

     Serial.println( "Reading... " );
     int8_t status = ibutton.readCode( code ); // Try to read ID-code

     if ( status > 0 ) {                       // Code read succesfully
       Serial.print( "iButton code read: " );
       ibutton.printCode( code );              // ID-code is in variable /code/
       Serial.println();
     }
   }
   ```

## Advanced Features

- Multiple iButton probes on the same 1-Wire bus
- Utility functions to print, test code validity and test code equality

## Hardware notes

### iButton identification tags

- Many iButton tags have their identification code engraved on them in hexadecimal: in the center the _reversed_ tag specific 6-byte code, above it on the right side 1-byte family code and on the left side 1-byte checksum. When read by this library the order is: family code - tag specific code - checksum.
- DS1990 can't be used with multiple identification tags on a single 1-Wire bus and requires special handling. For details see comments on function _readCode_ in _iButtonTag.ccp_ source file.
- RW1990 is the only iButton tag that had a rewritable identification code. Currently only reading is supported, research to add writing capability to the library is on it's way.

### iButton probes
- iButton probes come in many forms, some have 1 or 2 build-in indicator LED's.

### No probe? No problem...

iButton probes are essentially just connecting the two parts of an iButton _cylinder_ to wires, so you can simulate a probe and setup like this:
- Connect the _flat circular surface_ of the iButton to an Arduino digital pin, this is the 1-Wire data line.
- Connect a 4k7 kΩ pull-up resistor between the 1-Wire data line and Arduino 5V power pin.
- Connect the _side of the cylinder_ of the iButton to an Arduino ground (GND) pin.

## Documentation and references

### Articles

- [What is an iButton Device?](https://www.analog.com/en/resources/technical-articles/what-is-an-ibutton-device.html)

### Data sheets

- [Book of iButton Standards](https://www.analog.com/media/en/technical-documentation/tech-articles/book-of-ibuttonreg-standards.pdf)
- [DS1990A](https://www.analog.com/media/en/technical-documentation/data-sheets/ds1990a.pdf)
- [DS1990R](https://www.analog.com/media/en/technical-documentation/data-sheets/DS1990R-DS1990R-F5.pdf)

## License

MIT License | Copyright (c) 2025 SA van der Wulp

Full license text available in [LICENSE](LICENSE) file.
