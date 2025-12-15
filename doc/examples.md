# [📚 Arduino iButtonTag Library](https://vdwulp.github.io/iButtonTag/)

## 🔗 Quick links
- [General information](https://vdwulp.github.io/iButtonTag/)
- [Reference documentation](https://vdwulp.github.io/iButtonTag/REFERENCE.html)
- [GitHub repository](https://github.com/vdwulp/iButtonTag)
- [Download latest release](https://github.com/vdwulp/iButtonTag/releases/latest)

## Examples
- After installation in the Arduino IDE examples are available via _File-menu > Examples > iButtonTag_.
- A short description of all available examples follows below.

<a id="Simple"></a>
### Simple
[source code](https://github.com/vdwulp/iButtonTag/blob/main/examples/Simple/Simple.ino)

Simple example showing basic usage of the library to read an identification code from an iButton tag.

Uses functions [readCode](https://vdwulp.github.io/iButtonTag/REFERENCE.html#readCode) and [printCode](https://vdwulp.github.io/iButtonTag/REFERENCE.html#printCode).

<a id="Single"></a>
### Single
[source code](https://github.com/vdwulp/iButtonTag/blob/main/examples/Single/Single.ino)

Example showing usage of the library to read an identification code from an iButton tag and handle all possible status codes.

Uses functions [readCode](https://vdwulp.github.io/iButtonTag/REFERENCE.html#readCode) and [printCode](https://vdwulp.github.io/iButtonTag/REFERENCE.html#printCode).

<a id="Multiple"></a>
### Multiple
[source code](https://github.com/vdwulp/iButtonTag/blob/main/examples/Multiple/Multiple.ino)

Example showing usage of the library to read identification codes from multiple iButton tags connected to the same data line.

Uses functions [readCodes](https://vdwulp.github.io/iButtonTag/REFERENCE.html#readCodes), [nextCode](https://vdwulp.github.io/iButtonTag/REFERENCE.html#nextCode) and [printCode](https://vdwulp.github.io/iButtonTag/REFERENCE.html#printCode).

<a id="Match"></a>
### Match
[source code](https://github.com/vdwulp/iButtonTag/blob/main/examples/Match/Match.ino)

Example showing usage of the library to read an identification code from an iButton tag and check if it matches a predefined code.

Uses functions [readCode](https://vdwulp.github.io/iButtonTag/REFERENCE.html#readCode), [equalCode](https://vdwulp.github.io/iButtonTag/REFERENCE.html#equalCode) and [printCode](https://vdwulp.github.io/iButtonTag/REFERENCE.html#printCode).

<a id="WriteCode"></a>
### WriteCode
[source code](https://github.com/vdwulp/iButtonTag/blob/main/examples/WriteCode/WriteCode.ino)

Example showing usage of the library to _write_ an identification code to a (re)writable iButton tag and check if writing succeeded.

Uses functions [readCode](https://vdwulp.github.io/iButtonTag/REFERENCE.html#readCode), [equalCode](https://vdwulp.github.io/iButtonTag/REFERENCE.html#equalCode), [printCode](https://vdwulp.github.io/iButtonTag/REFERENCE.html#printCode), [updateChecksum](https://vdwulp.github.io/iButtonTag/REFERENCE.html#updateChecksum) and [writeCode](https://vdwulp.github.io/iButtonTag/REFERENCE.html#writeCode).
