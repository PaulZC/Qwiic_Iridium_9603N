# Qwiic_Iridium_9603N

A breakout for the Iridium 9603N Short Burst Data Transceiver in Qwiic format with I2C connectivity

![Qwiic_Iridium_9603N_1](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/Qwiic_Iridium_9603N_1.JPG)

![Qwiic_Iridium_9603N_2](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/Qwiic_Iridium_9603N_2.JPG)

![Dimensions](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/Dimensions.png)

![Top](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/Top.JPG)

![Bottom](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/Bottom.JPG)

[LEARN.md](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/LEARN.md) describes how the Qwiic Iridium 9603N works.

[ASSEMBLY.md](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/ASSEMBLY.md) describes how to assemble the PCB.

The [Arduino folder](https://github.com/PaulZC/Qwiic_Iridium_9603N/tree/master/Arduino) contains the code for the ATtiny841.

At the time of writing, you will need to use [this fork of Mikal Hart's Iridium SBD library](https://github.com/PaulZC/IridiumSBD) to provide
I2C support and examples of how to interface the Qwiic Iridium 9603N to other Qwiic devices.
The Beacon_I2C example demonstrates how to interface the Qwiic Iridium 9603N with (e.g.) a [SparkFun SAM-M8Q breakout](https://www.sparkfun.com/products/15210)
or [ZOE-M8Q breakout](https://www.sparkfun.com/products/15193) to form an I2C GPS + Iridium beacon or tracker:

![Beacon_I2C](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/Beacon_I2C.JPG)

The Eagle files are available in the [Eagle folder](https://github.com/PaulZC/Qwiic_Iridium_9603N/tree/master/Eagle).

The schematic is available [here](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/Schematic.png).

The BOM is available [here](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/Qwiic_Iridium_9603N_BOM.pdf) in PDF format
and [here](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/Qwiic_Iridium_9603N_BOM.xlsx) in Excel .xlsx format.

## Acknowledgements

This project is based extensively on the Qwiic MP3 Trigger and the SparkFun Ublox library by Nathan Seidle of SparkFun Electronics.
It would not have been possible without the open source designs kindly provided by SparkFun. Their work is gratefully acknowledged.

Feel like supporting open source hardware? Buy a board from SparkFun!

This project also relies upon [Spence Konde's ATTinyCore](https://github.com/SpenceKonde/ATTinyCore) for the ATtiny841.
Spence's work is gratefully acknowledged.

## Licence

This project is distributed under a Creative Commons Attribution + Share-alike (BY-SA) licence.
Please refer to section 5 of the licence for the "Disclaimer of Warranties and Limitation of Liability".

Enjoy!

**_Paul_**

