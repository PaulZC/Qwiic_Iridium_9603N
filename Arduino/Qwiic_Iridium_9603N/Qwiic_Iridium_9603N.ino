/*
  Qwiic Iridium 9603N
  By: Paul Clark
  Date: 7th July 2019
  
  Based extensively on:
  Qwiic MP3 Trigger (April 23rd, 2018)
  By: Nathan Seidle, SparkFun Electronics
  
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Uses Spence Konde's ATTinyCore boards:
  https://github.com/SpenceKonde/ATTinyCore
  https://github.com/SpenceKonde/ATTinyCore/blob/master/Installation.md
  Add this URL to the Additional Boards Manager URLs in File\Preferences:
  http://drazzy.com/package_drazzy.com_index.json
  and then use the Boards Manager to add support for the attiny

  Set Board to ATtiny24/44/84
  Set Chip to ATtiny84
  Set Clock to 8MHz(internal)
  Set Pin Mapping to Clockwise(damellis)

  Pin Allocation:
  0:  Physical Pin 13 (PCINT0)           : Serial Tx - connected to 9603N TX(IN)
  1:  Physical Pin 12 (PCINT1)           : Serial Rx - connected to 9603N RX(OUT)
  2:  Physical Pin 11 (PCINT2)           : 9603N ON_OFF - pull high to enable the 9603N, pull low to disable it
  3:  Physical Pin 10 (PCINT3)           : PWR_EN - pull high to enable the 5.3V supply to the 9603N via the P-FET
  4:  Physical Pin 9  (PCINT4)           : I2C SCL
  5:  Physical Pin 8  (PCINT5)           : MISO - connected to pin 1 of the ISP header
  6:  Physical Pin 7  (PCINT6)           : I2C SDA
  7:  Physical Pin 6  (PCINT7)           : 9603N Network Available - high when the 9603 can successfully receive the Ring Channel
  8:  Physical Pin 5  (INT0 / PCINT10)   : 9603N Ring Indicator - will pulse high twice when a new Mobile Terminated SBD message is available
  9:  Physical Pin 3  (PCINT9)           : LTC3225 Power Good - goes high when the supercapacitors are charged (will also be high when !SHDN is low!)
  10: Physical Pin 2  (PCINT8)           : LTC3225 !SHDN - pull low to disable the supercapacitor charger
  11: Physical Pin 4  (!RESET / PCINT11) : ATtiny84 !Reset

  To write to the I/O pins, the Master should send a two byte message: the first byte should be the special character SET_IO (0x10);
  the second byte will be written to I/O 'register'. Use a read-modify-write approach as necessary!

  To write serial data to the 9603N, the Master should send the special character WR_9603 (0x96) followed by the data bytes.

  Data sent by the ATtiny84 in response to a request from the Master will always be at least one byte long.
  The first byte will be the state of the I/O 'register'; serial data from the 9603N (if any) will follow.

  Change log:
  V1.0 - First commit
*/

#include <SoftwareSerial.h>

SoftwareSerial ssIridium(1, 0); //RX, TX pins

#include <Wire.h>

#include <avr/sleep.h> //Needed for sleep_mode
#include <avr/power.h> //Needed for powering down perihperals such as the ADC/TWI and Timers

//Define the ATtiny84's I2C address
const uint8_t I2C_ADDRESS = 0x63;

//Define the special characters for receiveEvents 
#define SET_IO  0x10 // Set the I/O pins
#define WR_9603 0x96 // Write serial data to the 9603N

//These are the bit definitions for the IO 'register'
const uint8_t IO_SHDN   = (1 << 0); // LTC3225 !SHDN : Read / Write
const uint8_t IO_PWR_EN = (1 << 1); // 9603N power enable via the P-FET : Read / Write
const uint8_t IO_ON_OFF = (1 << 2); // 9603N ON_OFF pin : Read / Write
const uint8_t IO_RI     = (1 << 3); // 9603N Ring Indicator _flag_ : Read / Write (Set by the INT0 service routine, _cleared_ by writing a _1_ to this bit)
const uint8_t IO_NA     = (1 << 4); // 9603N Network Available : Read only
const uint8_t IO_PGOOD  = (1 << 5); // LTC3225 PGOOD : Read only

//Create the IO 'register'
//A '1' in any of the bits indicates that the pin is ON (not necessarily that it is HIGH!)
volatile byte IO_REG;

//Create a flag for the RI interrupt and clear it
volatile boolean RI_FLAG = false;

//Digital pins
const byte ON_OFF = 2; // 9603N ON_OFF - pull high to enable the 9603N, pull low to disable it
const byte PWR_EN = 3; // PWR_EN - pull high to enable the 5.3V supply to the 9603N via the P-FET
const byte NA = 7; // 9603N Network Available - high when the 9603 can successfully receive the Ring Channel
const byte RI = 8; // 9603N Ring Indicator - will pulse high twice when a new Mobile Terminated SBD message is available
const byte PGOOD = 9; // LTC3225 Power Good - goes high when the supercapacitors are charged (will also be high when !SHDN is low!)
const byte SHDN = 10; // LTC3225 !SHDN - pull high to enable the supercapacitor charger

//Define the ON and OFF states for each pin
#define ON_OFF__ON  HIGH // 9603N ON_OFF - pull high to enable the 9603N, pull low to disable it
#define ON_OFF__OFF LOW
#define PWR_EN__ON  HIGH // PWR_EN - pull high to enable the 5.3V supply to the 9603N via the P-FET
#define PWR_EN__OFF LOW
#define SHDN__ON    HIGH // LTC3225 !SHDN - pull high to enable the supercapacitor charger
#define SHDN__OFF   LOW
#define NA__ON      HIGH // 9603N Network Available - high when the 9603 can successfully receive the Ring Channel
#define NA__OFF     LOW
#define PGOOD__ON   HIGH // LTC3225 Power Good - goes high when the supercapacitors are charged (will also be high when !SHDN is low!)
#define PGOOD__OFF  LOW

//INT0 Interrupt Service Routine
//ISR(INT0_vect) { RI_FLAG = true; }
void int0ISR() { RI_FLAG = true; } // Set RI_FLAG to true

void setup()
{
  pinMode(ON_OFF, OUTPUT);
  digitalWrite(ON_OFF, ON_OFF__OFF); // Disable the 9603N until PGOOD has gone high
  pinMode(PWR_EN, OUTPUT);
  digitalWrite(PWR_EN, PWR_EN__OFF); // Disable power to the 9603N until PGOOD has gone high
  pinMode(SHDN, OUTPUT);
  digitalWrite(SHDN, SHDN__ON); // Enable the LTC3225 and charge the supercapacitors

  IO_REG = IO_SHDN; // Set (only) the SHDN bit in the IO register (Probably redundant. requestEvent will update it.)

  pinMode(PGOOD, INPUT); // Has its own pullup
  pinMode(NA, INPUT); // The datasheet isn't clear if a pullup is required... (Probably not?)
  pinMode(RI, INPUT); // The datasheet isn't clear if a pullup is required... (Probably not?)

  attachInterrupt(8, int0ISR, RISING); // Attach the INT0 interrupt (physical pin 8), rising edge

  ssIridium.begin(19200); //9603N communicates at 19200bps

  //Begin listening on I2C
  startI2C();
}

void loop()
{
  ; // Nothing much to do here - everything gets done by the I2C interrupts
}

//Begin listening on I2C bus as I2C slave using the global I2C_ADDRESS
void startI2C()
{
  Wire.end(); //Before we can change addresses we need to stop

  Wire.begin(I2C_ADDRESS); //Do the Wire.begin using the defined I2C_ADDRESS

  //The connections to the interrupts are severed when a Wire.begin occurs. So re-declare them.
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

//Software delay. Does not rely on internal timers.
void noIntDelay(byte amount)
{
  for (volatile byte y = 0 ; y < amount ; y++)
  {
    //ATtiny84 at 8MHz
    for (volatile unsigned int x = 0 ; x < 1500 ; x++) //1ms at 8MHz
    {
      __asm__("nop\n\t");
    }
  }
}
