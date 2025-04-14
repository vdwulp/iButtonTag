# [📚 Arduino iButtonTag Library](https://vdwulp.github.io/iButtonTag/)

## Reference documentation
- This iButtonTag library reference documentation describes all available types, constants and functions.
- For more general information visit [Arduino iButtonTag Library](https://vdwulp.github.io/iButtonTag/).
- All current _constants_ are used to indicate iButton (re)writable tag types and their valid value range. These are used in functions related to _writing_ identification codes.
- Apart from the _constructor_, available functions can be arranged in three groups:
  - Reading identification code(s): [readCode](#readCode), [readCodes](#readCodes), [nextCode](#nextCode)
  - Writing identification code: [writeCode](#writeCode), [detectWritableType](#detectWritableType)
  - Utility: [testCode](#testCode), [equalCode](#equalCode), [printCode](#printCode), [updateChecksum](#updateChecksum)

## Types

<a id="iButtonCode"></a>
### iButtonCode
Type representing an iButton identification code of eight bytes.

The _iButtonCode_ type can be used as een array of eight uint8_t values. The first byte represents a OneWire family code indicating a iButton/device/sensor type. The middle six bytes are a (unique) identification code. The last byte is a checksum over the family and identification codes.

## Constants

<a id="IBUTTON_UNKNOWN"></a>
### IBUTTON_UNKNOWN
Indicates _unknown_ iButton tag type. Returned from [detectWritableType](#detectWritableType), argument to [writeCode](#writeCode).

<a id="IBUTTON_RW1990V1"></a>
### IBUTTON_RW1990V1
Indicates _RW1990V1_ iButton tag type. Returned from [detectWritableType](#detectWritableType), argument to [writeCode](#writeCode). This iButton tag type includes models sold as RW1990, RW1990.1, ТМ08 and ТМ08v2.

<a id="IBUTTON_RW1990V2"></a>
### IBUTTON_RW1990V2
Indicates _RW1990V2_ iButton tag type. Returned from [detectWritableType](#detectWritableType), argument to [writeCode](#writeCode). This iButton tag type includes models sold as RW1990v2 and RW1990.2.

<a id="IBUTTON_RW2004"></a>
### IBUTTON_RW2004
Indicates _RW2004_ iButton tag type. Returned from [detectWritableType](#detectWritableType), argument to [writeCode](#writeCode). This iButton tag type includes models sold as RW2004 and TM2004.

<a id="IBUTTON_TM01"></a>
### IBUTTON_TM01
Indicates _TM01_ iButton tag type. Argument to [writeCode](#writeCode). Because this iButton tag type is non-detectable, this value is _not_ returned from [detectWritableType](#detectWritableType). This iButton tag type includes models sold as TM01 and TM01C.

<a id="IBUTTON_MAXWRITABLE"></a>
### IBUTTON_MAXWRITABLE
Indicates the maximum value of an iButton tag type constant. Can be used to determine if a tag type value is in valid range.

## Functions

### iButtonTag( uint8_t pin )
Constructs an iButtonTag object linked to the supplied pin.

**Arguments**
| type | name | description |
|:-----|:-----|:------------|
| uint8_t | pin | Arduino pin number this iButtonTag object should be linked to. |

<a id="readCode"></a>
### int8_t readCode( iButtonCode code, bool old )
Reads one single iButton identifying code from the OneWire.

When multiple iButtons are connected to the OneWire this function will return an invalid reading because of a checksum failure (return value -2) caused by colliding responses. If there is _any_ possibility multiple iButtons are connected, use [readCodes](#readCodes) instead.

DS1990 iButton tags can't be used with multiple tags on a single OneWire data line and require special handling. This function facilitates compatibility with the DS1990 iButton Tags when argument _old_ is set to true. However, this reduces compatibility with other iButton tags: DS1990A, DS1990R and TM1990A will still be handled correctly (they offer backwards compatibility), but other OneWire devices (including iButtons) won't and may even show unexpected behaviour.

**Arguments**
| type | name | description |
|:-----|:-----|:------------|
| [iButtonCode](#iButtonCode) | code | Variable to store code read from the OneWire. |
| bool | old | Setting to _true_ enables compatibility with DS1990 iButton tags. Default value is _false_. |
 
**Returns**
| value | description |
|:-----:|:------------|
|  1 | Next iButton read succesfully, code array filled with identifying code |
|  0 | No more iButtons detected, code array is unchanged |
| -1 | Invalid iButton code read, checksum failed, code array with invalid bytes |
| -2 | Invalid iButton code read, all zeros, code array with invalid bytes |

<a id="readCodes"></a>
### int8_t readCodes()
Starts the search for multiple iButton identifying codes on the OneWire.

Resets the domain to search for iButton identifying codes. This function is needed to start searching for codes _again_. It's not really needed the first time, though it's good practice to always use it before enumerating codes with the [nextCode](#nextCode) function.

**Returns**
| value | description |
|:-----:|:------------|
| 1 | At least one iButton detected, enumerate with [nextCode](#nextCode) function
| 0 | No iButton detected

<a id="nextCode"></a>
### int8_t nextCode( iButtonCode code )
Continues the search for multiple iButton identifying codes on the OneWire.

Start the search for multiple iButton identyfying codes with the [readCodes](#readCodes) function. The is this function to enumerate all iButton identifying codes on the OneWire.

A return value 0 means searching finished succesfully, but there are no more iButtons on the OneWire. Negative return values indicate a problem during the search (mostly due to movement of the iButton on the reader), but additional calls to the function _may_ yield new iButton identifying codes. However, the overall result will be unreliable.

**Arguments**
| type | name | description |
|:-----|:-----|:------------|
| [iButtonCode](#iButtonCode) | code | Variable to store code read from the OneWire. |

**Returns**
| value | description |
|:-----:|:------------|
|  1 | Next iButton read succesfully, code array filled with identifying code |
|  0 | No more iButtons detected, code array is unchanged |
| -1 | Invalid iButton code read, checksum failed, code array with invalid bytes |
| -2 | Invalid iButton code read, all zeros, code array with invalid bytes |

<a id="testCode"></a>
### static int8_t testCode( iButtonCode code )
Tests [iButtonCode](#iButtonCode) for validity.

**Arguments**
| type | name | description |
|:-----|:-----|:------------|
| [iButtonCode](#iButtonCode) | code | Code to be tested. |

**Returns**
| value | description |
|:-----:|:------------|
|  1 | iButton code valid |
| -1 | iButton code invalid, checksum failed |
| -2 | iButton code invalid, all zeros |

<a id="equalCode"></a>
### static bool equalCode( iButtonCode a, iButtonCode b )
Tests if two [iButtonCode](#iButtonCode)'s are equal.

**Arguments**
| type | name | description |
|:-----|:-----|:------------|
| [iButtonCode](#iButtonCode) | a | First code to be tested. |
| [iButtonCode](#iButtonCode) | b | Second code to be tested. |

**Returns**
| value | description |
|:-----:|:------------|
| true  | The two [iButtonCode](#iButtonCode)'s are equal |
| false | The two [iButtonCode](#iButtonCode)'s are _not_ equal |

<a id="printCode"></a>
### static void printCode( iButtonCode code, bool reverse )
Prints [iButtonCode](#iButtonCode) to Serial as hexadecimal byte values.

Serial must be initialised in the main code first. By default the bytes are printed as received from the iButton (reverse = false). The order can be reversed (reverse = true) to match the sequence fysically engraved on many iButtons.

**Arguments**
| type | name | description |
|:-----|:-----|:------------|
| [iButtonCode](#iButtonCode) | code | The code to be printed. |
| bool | reverse | Setting to _true_ will reverse the printed code. Default value is _false_. |

<a id="updateChecksum"></a>
### static void updateChecksum( iButtonCode code )
Updates checksum of [iButtonCode](#iButtonCode) to a correct value.

**Arguments**
| type | name | description |
|:-----|:-----|:------------|
| [iButtonCode](#iButtonCode) | code | The code to be updated. |

<a id="detectWritableType"></a>
### int8_t detectWritableType()
Detects type of (re)writable iButton tag.

Performs multiple tests to check for known reponses of (re)writable iButton tag types. If a detectable (re)writable type is found, the return value indicates the specific model. All supported (re)writable types are defined as iButton (re)writable tag type constants, like [IBUTTON_RW1990V1](#IBUTTON_RW1990V1), [IBUTTON_RW1990V2](#IBUTTON_RW1990V2), [IBUTTON_RW2004](#IBUTTON_RW2004) or [IBUTTON_TM01](#IBUTTON_TM01).

A return value of iButton (re)writable type constant [IBUTTON_UNKNOWN](#IBUTTON_UNKNOWN) may indicate one of the following:
- iButton tag _is not_ a (re)writable tag. However, it _is_ readable.
- iButton tag _is not_ of a (re)writable type supported by this library. However, it _is_ readable.
- iButton tag _is_ of a (re)writable type supported by this library. However, this specific type _is not_ detectable, like types indicated by [IBUTTON_TM01](#IBUTTON_TM01).

**Returns**
| value | description |
|:-----:|:------------|
| \>0 | iButton writable type found as indicated by type constant |
|   0 | iButton writable type unknown, no detectable writable type found |
|  -1 | No iButton detected |

<a id="writeCode"></a>
### int8_t writeCode( iButtonCode code, int8_t type, bool check )
Writes a new iButton identifying code to a (re)writable tag.

Strong recommendations, please read carefully:
- It is recommended to have _only one_ iButton probe/tag connected to the data line when writing. Some iButton (re)writable tag types will allow multiple tags to be written at the same time, but it may lead to failure.
- It is _also_ recommended to only supply the new code as an argument. The function will try to detect a (re)writable iButton tag type. If it fails, you should seriously check if the tag really is a (re)writable tag of a type supported by this library. As iButton tag type TM01 is non-detectable, this type will never be detected. In this case supply iButton (re)writable tag type constant [IBUTTON_TM01](#IBUTTON_TM01). To override auto-detection in other cases, you can also supply an iButton (re)writable tag type constant.
- It is _also_ recommended to have checking _on_ like the default, making sure as much checks as possible are done before actually writing a new code to the iButton tag. As some writeble tags are just _write once_ there is a risk in trying to write without appropriate checking.

This function supports writing a new iButton identification code to tag models RW1990, RW1990.1, ТM08, ТM08v2 (type [IBUTTON_RW1990V1](#IBUTTON_RW1990V1)), RW1990v2, RW1990.2 (type [IBUTTON_RW1990V2](#IBUTTON_RW1990V2)), RW2004, TM2004 (type [IBUTTON_RW2004](#IBUTTON_RW2004)), TM01 and TM01C (type [IBUTTON_TM01](#IBUTTON_TM01)).

**Arguments**
| type | name | description |
|:-----|:-----|:------------|
| [iButtonCode](#iButtonCode) | code | Code to be written. |
| int8_t | type | iButton (re)writable tag type, use library constants. Default value is [IBUTTON_UNKNOWN](#IBUTTON_UNKNOWN). |
| bool | check | Setting to _false_ disables most checking done before trying to write. Default value is _true_. |

**Returns grouped**
| value | description |
|:-----:|:------------|
|  1 | Writing procedure finished successfully |
|  0 | No iButton detected at some time during procedure |
| -1 to -9 | Problem related to supplied code |
| -11 to -19 | Problem related to supplied type |
| -21 to -29 | Failure of actual writing |

**Returns specific**
| value | description |
|:-----:|:------------|
|   1 | Writing procedure finished successfully
|   0 | No iButton detected at some time during procedure
|  -1 | iButton code invalid, checksum failed
|  -2 | iButton code invalid, all zeros
| -11 | iButton writable type invalid, supplied value out of range
| -12 | iButton writable type not detectable, supply specific type constant
| -13 | iButton writable type incorrect, unexpected response while testing
| -21 | Writing code failed, code read after writing procedure is not equal
| -22 | Writing code failed, unexpected response while writing
