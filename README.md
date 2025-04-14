# 📚 Arduino iButtonTag Library

[![Arduino CI](https://github.com/vdwulp/iButtonTag/actions/workflows/arduino-ci.yml/badge.svg)](https://github.com/marketplace/actions/arduino_ci)
[![Arduino-lint](https://github.com/vdwulp/iButtonTag/actions/workflows/arduino-lint.yml/badge.svg)](https://github.com/marketplace/actions/arduino-arduino-lint-action)
[![JSON-syntax](https://github.com/vdwulp/iButtonTag/actions/workflows/json-syntax.yml/badge.svg)](https://github.com/marketplace/actions/json-syntax-check)
[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](https://github.com/vdwulp/iButtonTag/blob/main/LICENSE)
[![GitHub release](https://img.shields.io/github/v/release/vdwulp/iButtonTag.svg)](https://github.com/vdwulp/iButtonTag/releases/latest)

An Arduino library handling iButton identification tags.

## 👍 Supported Devices

| Model                                    |  Reading ID-code   |  Writing ID-code   |
|:-----------------------------------------|:------------------:|:------------------:|
| DS1990, DS1990A, DS1990R, TM1990A        |         ✔️         |                    |
| RW1990, RW1990.1, RW1990v1, TM08, TM08v2 |         ✔️         |         ✔️        |
| RW1990.2, RW1990v2                       |         ✔️         |         ✔️        |
| RW2004, TM2004                           |         ✔️         |         ✔️        |
| TM01, TM01C                              |         ✔️         |         ✔️        |
| Many other iButton tags                  |         ✔️         |                    |

<img src="https://vdwulp.github.io/iButtonTag/iButtonTag-TM1990A.png" alt="iButton tags model TM1990A" width=220 height=220><img src="https://vdwulp.github.io/iButtonTag/iButtonTag-RW1990.png" alt="iButton tags model TM1990A" width=220 height=220><img src="https://vdwulp.github.io/iButtonTag/iButtonProbe-OneLed.png" alt="iButton probe flat model with indicator LED" width=220 height=220>

## ✈️ Installation

### Using Arduino IDE Library Manager - _Recommended!_
1. Open Arduino IDE
2. Go to Tools > Manage Libraries...
3. Search for "iButtonTag"
4. Click Install
5. Click Install All to also install required OneWire library

_If you don't see the Install All button mentioned in step 5, you have an older Arduino IDE (<1.8.10). Consider upgrading the IDE, or repeat steps 1 to 4 above to install OneWire library. In step 3 search for "OneWire"._

### Manual installation
1. Download the latest release ZIP file from [iButtonTag releases](https://github.com/vdwulp/iButtonTag/releases/latest)
2. In Arduino IDE, go to Sketch > Include Library > Add .ZIP Library...
3. Select the downloaded ZIP file
4. Repeat steps above to install required _OneWire_ library; in step 1 download ZIP file from [OneWire releases](https://github.com/PaulStoffregen/OneWire/releases/latest)

## 🚶 Basic Usage

### Hardware Setup
- Connect data line of iButton probe to an Arduino digital pin, this is the 1-Wire data line.
- Connect a 2200 Ω to 4700 Ω pull-up resistor between the 1-Wire data line and Arduino 5V pin.
- Connect ground line of iButton probe to an Arduino ground (GND) pin.

### Code Example

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
       ibutton.printCode( code );              // ID-code is in variable _code_
       Serial.println();
     }
   }
   ```

## 🏃 Advanced Features
- Writing new identification code to (re)writable iButton tag models
- Reading multiple iButton probes on the same 1-Wire data line

## 🛠️ Hardware Notes

### iButton identification tags
- Many iButton tags have their identification code engraved on them in hexadecimal: in the center the _reversed_ tag specific 6-byte code, above it on the right side 1-byte family code and on the left side 1-byte checksum. When read by this library the order is: family code - tag specific code - checksum.
- Official iButton identification tags all have a unique identification code that _cannot_ be changed. However, this library supports writing a new code to multiple types of (re)writable _compatible_ tags.
- DS1990 can't be used with multiple identification tags on a single data line and requires special handling. For details see comments on function _readCode_ in _iButtonTag.ccp_ source file.

### Pull-up resistor
- The official recommendation is to use a 4700 Ω pull-up resistor between the 1-Wire data line and Arduino 5V pin when _reading_ from an iButton tag.
- Writing a new code to (re)writable iButton tags may require _more power_ for a successful and persistent result. To get more power to the tag, a 2200 Ω pull-up resistor between the 1-Wire data line and Arduino 5V pin has been tested to be a good value.

### iButton probes
- iButton probes come in many forms, some have LED-indicator(s) with one or two colors.
- iButton probes have at least two wires: a data line and a ground. Other wires are for indicator LED(s).
- iButton probes with two-color LED-indicator usualy have one _bi-directional_ LED that changes color when polarity on two available LED-wires is reversed.

### No probe? No problem...
iButton probes are essentially just connecting the two parts of an iButton _cylinder_ to wires, so you can simulate a probe and setup like this:
- Connect the _flat circular surface_ of the iButton to an Arduino digital pin, this is the 1-Wire data line.
- Connect a 4k7 kΩ pull-up resistor between the 1-Wire data line and Arduino 5V pin.
- Connect the _side of the cylinder_ of the iButton to an Arduino ground (GND) pin.

## 📓 Documentation & References

### Articles
- [What is an iButton Device?](https://www.analog.com/media/en/technical-documentation/tech-articles/what-is-an-ibutton-device.pdf)
- [How to Power the Extended Features of 1-Wire Devices](https://www.analog.com/media/en/technical-documentation/tech-articles/how-to-power-the-extended-features-of-1wire-devices.pdf)

### Data sheets
- [Book of iButton Standards](https://www.analog.com/media/en/technical-documentation/tech-articles/book-of-ibuttonreg-standards.pdf)
- [DS1990A](https://www.analog.com/media/en/technical-documentation/data-sheets/ds1990a.pdf)
- [DS1990R](https://www.analog.com/media/en/technical-documentation/data-sheets/DS1990R-DS1990R-F5.pdf)

## 🙏 Thanks
- The creator(s) of the [OneWire](https://github.com/PaulStoffregen/OneWire) library iButtonTag depends on. It's a very solid implementation of the 1-Wire protocol. I learned a lot by studying the protocol documentation in combination with the source of this library.
- The creator(s) of the [DallasTemperature](https://github.com/milesburton/Arduino-Temperature-Control-Library) library. When studying the 1-Wire protocol, working with the Dallas DS18B20 Temperature Sensor made everything more practical. In this period I also tried to contribute to this library.

## 🗒️ License
MIT License, full text available in [LICENSE](https://github.com/vdwulp/iButtonTag/blob/main/LICENSE) file.

Copyright (c) 2025 SA van der Wulp
