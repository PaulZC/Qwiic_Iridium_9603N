# Qwiic_Iridium_9603N: LEARN

![Qwiic_Iridium_9603N_1](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/Qwiic_Iridium_9603N_1.JPG)

## What is it?

The Qwiic Iridium 9603N is a breakout for the [Iridium 9603N Short Burst Data transceiver](https://www.iridium.com/products/iridium-9603/)
which can be used to send and receive 'short' messages (340 bytes transmitted, 270 bytes received) to and from _anywhere_ on Earth.
The 9603N is the heart of the [Iridium 9603 Beacon](https://github.com/PaulZC/Iridium_9603_Beacon),
the [Iridium 9603N Solar Beacon](https://github.com/PaulZC/Iridium_9603N_Solar_Beacon) and the
[Rock7 RockBLOCK 9603](http://www.rock7mobile.com/products-rockblock-9603).

The 9603N has a serial (UART) interface which is great for many applications, but by no means all. The Qwiic Iridium 9603N uses an
[ATtiny841 AVR microcontroller](https://www.microchip.com/wwwproducts/en/ATTINY841) to bridge the serial interface to I2C,
allowing you to easily add Iridium communication to all Qwiic projects.

The key components of the Qwiic Iridium 9603N are:

## Iridium 9603N SBD Transceiver

![9603_Pins](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/9603_Pins.JPG)

The 9603N transceiver is a 5V device but has 3.3V serial (UART) and Input/Ouput pins. The serial interface uses an old-school "AT"
command set to send and receive messages. The I/O pins control its operation. The Qwiic Iridium 9603N connects to:

- Pins 1 & 2: these are connected to the 5.3V supply from the supercapacitors via a P-FET switch.
- Pin 5: enables or disables the 9603N. High enables the 9603N, low disables it.
- Pin 6: is the serial data transmit _input_. The ATtiny841 serial transmit is connected to this pin.
- Pin 7: is the serial receive data _output_. This connects to the ATtiny841 serial receive pin.
- Pin 12: is the Ring Indicator. This pin will pulse low twice when a new 'Mobile Terminated' message is waiting to be downloaded.
- Pin 13: is the serial Request To Send input. This needs to be pulled low to allow serial communication to take place.
- Pin 14: is the serial Data Terminal Ready input. This too need to be pulled low.
- Pin 19: will be high when the Iridium network is available and the 9603N is able to receive the ring channel.

The ATtiny is connected to the I/O pins to form an I2C to serial bridge.

The full AT command reference can be found [here](http://www.rock7mobile.com/downloads/IRDM_ISU_ATCommandReferenceMAN0009_Rev2.0_ATCOMM_Oct2012.pdf).

## LTC3225 Supercapacitor Charger

![LTC3225](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/LTC3225.JPG)

The 9603N requires a power input of 5VDC +/- 0.5V. The power consumption at 5VDC is:

- 34mA average when idle
- 156mA peak when idle
- 39mA average during receive
- 156mA peak during receive
- 145mA average during transmit
- 1.3A peak during transmit
- The average current draw during a complete receive/transmit cycle is 158mA

The Qwiic Iridium 9603N uses an [LTC3225](https://www.analog.com/en/products/ltc3225.html) supercapacitor charger to meet these power needs.
When powered by 3.3V from the Qwiic interface, it can charge a pair of supercapacitors to 5.3V. These capacitors deliver the 1.3A peak
current draw when the 9603N transmits.

The LTC3225 charge current is adjustable up to a maximum of 150mA. When set to the full 150mA, the LTC3225 can match the 145mA average current
drawn by the 9603N during transmit. This means that 1 Farad supercapacitors are adequate as they only need to hold enough charge to meet the
9603N's peak current draw during the very brief (8.3ms) transmit bursts.

The LTC3225 has an efficiency of approximately 75% when powered from 3.3V. This means the current drawn from the Qwiic interface is
approximately 200mA when charging the supercapacitors.

If you want to power the Qwiic Iridium 9603N from a low current source, e.g. solar panels, the charge current can be reduced to 60mA by
changing the JP1 jumper link. The current draw from the Qwiic interface falls to approximately 80mA. The 60mA charge current is enough
to offset the average current draw during receive, but bigger supercapacitors are needed to deliver the average current draw during a complete
receive/transmit cycle. So, if you do change the charge current to 60mA, you will also need to solder additional 10 Farad supercapacitors on to
the rear of the PCB using the solder pads provided. Please see the BOM for the capacitor part numbers.

The connections between the LTC3225 and the ATtiny841 are:

- !SHDN (LTC3225 Pin 6) is used to enable or disable the LTC3225. Pulling this pin high enables it, low disables it.
- PGOOD (LTC3225 Pin 5) goes high when the supercapacitors are 94% charged. This pin is used to illuminated LED2; LED2 lights up when the capacitors
are _charging_.
- PWR_EN is a signal from the ATtiny841 which enables a P-channel FET (Q1) to switch power to the 9603N. Pulling PWR_EN high enables 9603N power, low disables it.

The power-up sequence is:

- Enable the LTC3225 by pulling !SHDN high
- Wait until PGOOD goes high (LED2 goes out)
- Enable 9603N power by pulling PWR_EN high

[This fork of Mikal Hart's Iridium SBD library](https://github.com/PaulZC/IridiumSBD) contains all of the functions you need to control the
Qwiic Iridium 9603N. The library contains several examples too. The power control functions are:

- _enableSuperCapCharger(true)_ will enable the LTC3225
- _checkSuperCapCharger()_ will return true when PGOOD has gone high
- _enable9603Npower(true)_ will enable power for the 9603N by pulling PWR_EN high

## ATtiny841

![ATtiny841](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/ATtiny841.JPG)

The ATtiny841 is an enhanced version of the popular ATtiny84. It includes hardware support for both I2C and Serial which is vital when using it as an
I2C to Serial bridge. (Software serial isn't up to the job.)

Spence Konde has written a comprehensive library called [ATTinyCore](https://github.com/SpenceKonde/ATTinyCore) which provides full support for the ATtiny841.
**At the time of writing, you will need to download ATTinyCore directly from GitHub as the version in Arduino Board Manager does not yet include
[one important fix](https://github.com/SpenceKonde/ATTinyCore/commit/0d17000a645234bb540b82086debf39ccb87f172) which the Qwiic Iridium 9603N needs.**

[The Iridium SBD library](https://github.com/PaulZC/IridiumSBD) contains high level functions which you can call to control and communicate with
the Qwiic Iridium 9603N. They do all of the heavy lifting for you. However, the following will be useful if you want to understand how the I2C interface works
so you can develop your own Wire functions should you want to.

The Qwiic Iridium 9603N emulates a slave I2C device with a fixed address of 0x63. You can change the address but only by editing the
[Arduino code](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/Arduino/Qwiic_Iridium_9603N_ATtiny841/Qwiic_Iridium_9603N_ATtiny841.ino#L90).

The code in the ATtiny841 emulates three read/write 'registers':

- IO_REG (0x10) : the Input/Output register which is used to read and write the I/O pins
- LEN_REG (0xFD) : this register returns the number of bytes waiting to be read from the serial buffer. The buffer length is returned in MSB,LSB format.
- DATA_REG (0xFF) : this register allows serial data to be written to and read from the 9603N

### IO_REG

To read the I/O pins, the I2C Master needs to:

```
Wire.beginTransmission(0x63); // Open communication with the Qwiic Iridium 9603N
Wire.write(0x10); // Point to the IO register
Wire.endTransmission(); // Send data and release the bus
Wire.requestFrom(0x63, 1); // Request one byte from the IO register
if (Wire.available())
{
  uint8_t io_pins = Wire.read(); // Read the IO register
}
```

The WireS library for the 841 doesn't like it if the Master holds the bus between write and requestFrom, so don't use _endTransmission(false);_

The bit definitions for the IO_REG are:

- Bit 7: Not Used
- Bit 6: Not Used
- Bit 5: PGOOD - Read Only - this bit will be high when the LTC3225 PGOOD signal is high
- Bit 4: NA - Read Only - this bit will be high when the 9603N Network Available pin is high
- Bit 3: RI - Read/Write - this bit is a flag which is set when the 9603N Ring Indicator pin pulses low. The flag can be cleared by writing a 0 to this bit
- Bit 2: ON_OFF - Read/Write - this bit controls the 9603N on/off pin (Pin 5). Writing a 1 to this bit enables the 9603N
- Bit 1: PWR_EN - Read/Write - this bit controls the PWR_EN signal which enables power to the 9603N via the P-FET. Writing 1 to this bit enables the 9603N power
- Bit 0: SHDN - Read/Write - this bit controls the LTC3225 !SHDN signal to enable/disable the supercapacitor charger. Writing 1 to this bit enables the LTC3225

When writing to the IO_REG, you will need to use a read-modify-write approach: read the IO_REG, set or clear the appropriate bit, write the value back to the IO_REG.
E.g. to enable the LTC3225, the Master needs to:

```
Wire.beginTransmission(0x63); // Open communication with the Qwiic Iridium 9603N
Wire.write(0x10); // Point to the IO register
Wire.endTransmission(); // Send data and release the bus
Wire.requestFrom(0x63, 1); // Request one byte from the IO register
uint8_t io_pins = Wire.read(); // Read the IO register
io_pins |= 0x01; // Set the SHDN bit to enable the LTC3225
Wire.beginTransmission(0x63); // Open communication with the Qwiic Iridium 9603N
Wire.write(0x10); // Point to the IO register
Wire.write(io_pins); // Update the pins
Wire.endTransmission(); // Send data and release the bus
```

### LEN_REG

The serial length register indicates how many bytes are available in the Qwiic Iridium 9603N's serial buffer. The length is returned as two bytes in MSB,LSB format.

To read the buffer length, the I2C Master needs to:

```
uint16_t bytesAvailable = 0;
Wire.beginTransmission(0x63); // Open communication with the Qwiic Iridium 9603N
Wire.write(0xFD); // Point to the length register
Wire.endTransmission(); // Send data and release the bus
Wire.requestFrom(0x63, 2); // Request two bytes from the length register
if (Wire.available() >= 2)
{
  uint8_t msb = Wire.read(); // Read the length MSB
  uint8_t lsb = Wire.read(); // Read the length LSB
  bytesAvailable = (((uint16_t)msb) << 8) | (uint16_t)lsb; // Update bytesAvailable
}
```

Reading serial data from the DATA_REG needs to be done eight bytes at a time (if there are more than eight bytes available) due to limitations in the way the
way Wire.onRequest works. See below for details.

### DATA_REG

The serial data register is used to send data to and receive data from the 9603N.

To write serial data to the 9603N, the I2C Master simply needs to:

```
Wire.beginTransmission(0x63); // Open communication with the Qwiic Iridium 9603N
Wire.write(0xFF); // Point to the data register
Wire.print(str); // Write the string
Wire.endTransmission(); // Send data and release the bus
```

Take care not to send more than 32 bytes in one go or you will overflow the ATtiny's I2C buffer! Use multiple writes as required.

Reading serial data is a little more complex as the Wire.onRequest code in the ATtiny does not know how many bytes are being requested, simply that _some_ data is being requested.
For the Qwiic Iridium 9603N, we define that the Master will request data eight bytes at a time (if there are eight or more bytes available in the serial buffer).

A complete read cycle would be:

```
uint16_t bytesAvailable = 0;
Wire.beginTransmission(0x63); // Open communication with the Qwiic Iridium 9603N
Wire.write(0xFD); // Point to the length register
Wire.endTransmission(); // Send data and release the bus
Wire.requestFrom(0x63, 2); // Request two bytes from the length register
if (Wire.available() >= 2)
{
  uint8_t msb = Wire.read(); // Read the length MSB
  uint8_t lsb = Wire.read(); // Read the length LSB
  bytesAvailable = (((uint16_t)msb) << 8) | (uint16_t)lsb; // Update bytesAvailable
}
while (bytesAvailable > 8) // If there are more than 8 bytes waiting to be read
{
  Wire.requestFrom(0x63, 8); // Request 8 bytes from the data register
  while (Wire.available())
  {
    uint8_t data_byte = Wire.read(); // Read the data
    // Store the data byte
  }
  bytesAvailable -= 8; // Decrease bytesAvailable by 8
}
if (bytesAvailable > 0) // If there are some bytes waiting to be read
{
  Wire.requestFrom(0x63, bytesAvailable); // Request the remaining bytes from the data register
  while (Wire.available())
  {
    uint8_t data_byte = Wire.read(); // Read the data
    // Store the data byte 
  }
}
```

The code in the ATtiny keeps its own copy of how many bytes were in the serial buffer when the length register was read, so it knows how many bytes to send
to the Master even if new serial data from the 9603N arrives part way through.

## ATTinyCore

The [Arduino folder](https://github.com/PaulZC/Qwiic_Iridium_9603N/tree/master/Arduino) contains the code for the ATtiny841.

The Qwiic Iridium 9603N relies upon Spence Konde's [ATTinyCore](https://github.com/SpenceKonde/ATTinyCore) to provide support for the ATtiny841.
**At the time of writing, you will also need to download ATTinyCore directly from GitHub as the version in the Arduino Board Manager does not yet include
[one important fix](https://github.com/SpenceKonde/ATTinyCore/commit/0d17000a645234bb540b82086debf39ccb87f172) which the Qwiic Iridium 9603N needs.**

When compiling the code:
- Set Board to ATtiny441/841 (No bootloader)
- Set Chip to ATtiny841
- Set Clock to 8MHz(internal)
- Set BOD Level to BOD Enabled 1.8V
- Set BOD Mode (Active) and (Sleep) to BOD Disabled
- Set Save EEPROM to EEPROM Not Retained
- **Set Pin Mapping to Clockwise**
- **Set Wire Modes to Slave Only**

The code has been tested using Arduino 1.8.9 **but using version 1.6.21 of the Arduino AVR Boards**. At the time of writing, later versions contain a
bug in the compiler tool chain which causes segmentation faults. Please see the [Gotchas List in Spence's repo](https://github.com/SpenceKonde/ATTinyCore#quick-gotcha-list).

The version of AVRDUDE in version 1.6.21 of the Arduino AVR Boards unfortunately does not provide support for the ATtiny841. This means you will need
to download [AVRDUDESS](http://blog.zakkemble.net/avrdudess-a-gui-for-avrdude/) and run it separately, using the compiled binary produced by the Arduino IDE
to configure the ATtiny841:

- Use _Sketch\Export compiled Binary_ to generate the .hex and .lst files for the ATtiny
- Configure an Arduino board as an _Arduino as ISP_ programmer
- Connect the Arduino and use AVRDUDESS to program the ATTiny841 with the .hex file via the ISP header

The ATtiny841 is delivered with the CKDIV8 bit programmed, which will make the 841 run at 1MHz instead of 8MHz. You will need to use AVRDUDESS to read and then
set the fuses to correct this:

- Set the L (Low) fuse byte to **0xE2**
- Set the H (High) fuse byte to 0xDF
- Set the E (Extended) fuse byte to 0xFF

Enjoy!

**_Paul_**





