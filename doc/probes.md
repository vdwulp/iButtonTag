# [📚 Arduino iButtonTag Library](https://vdwulp.github.io/iButtonTag/)

## 🔗 Quick links
- [General information](https://vdwulp.github.io/iButtonTag/)
- [Reference documentation](https://vdwulp.github.io/iButtonTag/REFERENCE.html)
- [GitHub repository](https://github.com/vdwulp/iButtonTag)
- [Download latest release](https://github.com/vdwulp/iButtonTag/releases/latest)

## iButton probes
- iButton probes come in many forms, some have LED-indicator(s) with one or two colors.
- iButton probes have at least two wires: a data line and a ground. Other wires are for indicator LED(s).
- iButton probes with two-color LED-indicator usualy have one bi-directional LED that changes color when polarity on two available LED-wires is reversed.
- On this page are some examples of iButton probes with their wiring. Wiring may vary for the model you receive.

<a id="noprobe"></a>
## No probe? No problem...
iButton probes are essentially just connecting the two parts of an iButton _cylinder_ to wires, so you can simulate a probe and setup like this:
- Connect the _flat circular surface_ of the iButton to an Arduino digital pin, this is the 1-Wire data line.
- Connect a 2200 Ω to 4700 Ω pull-up resistor between the 1-Wire data line and Arduino 5V pin.
- Connect the _side of the cylinder_ of the iButton to an Arduino ground (GND) pin.

<a id="examples"></a>
## Examples

### Flat iButton probe with _two color_ indicator LED
_Looks the same as one color model, see below. Different colors of LED-indicator by reversing polarity._
<table><tr><td rowspan=8><img src="https://vdwulp.github.io/iButtonTag/iButtonProbe-Flat.png" alt="iButton probe with one or two color indicator LED" width=200 height=200></td>
 <td><b>Color</b></td><td><b>Function</b></td></tr>
 <tr><td>Red</td><td>1-Wire Data</td></tr>
 <tr><td>Green</td><td>LED+ (red) / LED- (green)</td></tr>
 <tr><td>Yellow</td><td>LED- (red) / LED+ (green)</td></tr>
 <tr><td>Black</td><td>1-Wire Ground</td></tr>
</table>

### Flat iButton probe with _one color_ indicator LED
_Looks the same as two color model, see above._
<table><tr><td rowspan=8><img src="https://vdwulp.github.io/iButtonTag/iButtonProbe-Flat.png" alt="iButton probe with one or two color indicator LED" width=200 height=200></td>
 <td><b>Color</b></td><td><b>Function</b></td></tr>
 <tr><td>Red</td><td>1-Wire Data</td></tr>
 <tr><td>Green</td><td>LED+</td></tr>
 <tr><td>White</td><td>LED-</td></tr>
 <tr><td>Black</td><td>1-Wire Ground</td></tr>
</table>

### iButton probe with _one color_ indicator LED
<table><tr><td rowspan=8><img src="https://vdwulp.github.io/iButtonTag/iButtonProbe-OneLed.png" alt="iButton probe with one color indicator LED" width=200 height=200></td>
 <td><b>Color</b></td><td><b>Function</b></td></tr>
 <tr><td>Green</td><td>1-Wire Data</td></tr>
 <tr><td>Black</td><td>LED+</td></tr>
 <tr><td>White</td><td>LED-</td></tr>
 <tr><td>Red</td><td>1-Wire Ground</td></tr>
</table>

### iButton probe _without_ indicator LED
<table><tr><td rowspan=6><img src="https://vdwulp.github.io/iButtonTag/iButtonProbe-NoLed.png" alt="iButton probe without indicator LED" width=200 height=200></td>
 <td><b>Color</b></td><td><b>Function</b></td></tr>
 <tr><td>Red</td><td>1-Wire Data</td></tr>
 <tr><td>Black</td><td>1-Wire Ground</td></tr>
</table>
