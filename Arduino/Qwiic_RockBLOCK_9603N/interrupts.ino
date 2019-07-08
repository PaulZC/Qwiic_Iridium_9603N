//Handle a write to the IO pins _or_ the Serial port
void receiveEvent(int numberOfBytesReceived) {
  if (Wire.available()) {
    
    byte incoming = Wire.read(); // Read the first byte

    if (incoming == SET_IO) { // Does the Master want to set the I/O pins?
      if (Wire.available()) { // Did the Master send the second byte?
        incoming = Wire.read(); // Read the second byte

        if (incoming & IO_SHDN) { // If the SHDN bit is ON
          digitalWrite(SHDN, SHDN__ON); // Turn the pin ON
        }
        else {
          digitalWrite(SHDN, SHDN__OFF); // Turn the pin OFF
        }
        
        if (incoming & IO_PWR_EN) { // If the PWR_EN bit is ON
          digitalWrite(PWR_EN, PWR_EN__ON); // Turn the pin ON
        }
        else {
          digitalWrite(PWR_EN, PWR_EN__OFF); // Turn the pin OFF
        }
        
        if (incoming & IO_ON_OFF) { // If the ON_OFF bit is ON
          digitalWrite(ON_OFF, ON_OFF__ON); // Turn the pin ON
        }
        else {
          digitalWrite(ON_OFF, ON_OFF__OFF); // Turn the pin OFF
        }
        
        if (incoming & IO_RI) { // If the RI bit is ON
          RI_FLAG = false; // Clear the RI flag
        }
      } 
    }

    else if (incoming == WR_9603) { // Does the Master want to write serial data to the 9603N?      
      while (Wire.available()) { // Read all bytes and echo them to the serial port
        byte incoming = Wire.read();
        ssIridium.write(incoming);
      }      
    }
    
    else {
      ; //Something bad happened! The first byte was not SET_IO or WR_9603...
    }
  }
}

//Handle a read of the IO 'register' and the Serial port
void requestEvent()
{
  // Read the IO pins one at a time and set or clear bits in IO_REG appropriately
  
  if (digitalRead(SHDN) == SHDN__ON) { // If the SHDN pin is in its ON state (not necessarily that it is HIGH)
    IO_REG |= IO_SHDN; // Set the SHDN bit in IO_REG
  }
  else {
    IO_REG &= ~IO_SHDN; // Clear the SHDN bit in IO_REG
  }
  
  if (digitalRead(PWR_EN) == PWR_EN__ON) { // If the PWR_EN pin is in its ON state (not necessarily that it is HIGH)
    IO_REG |= IO_PWR_EN; // Set the PWR_EN bit in IO_REG
  }
  else {
    IO_REG &= ~IO_PWR_EN; // Clear the PWR_EN bit in IO_REG
  }
  
  if (digitalRead(ON_OFF) == ON_OFF__ON) { // If the ON_OFF pin is in its ON state (not necessarily that it is HIGH)
    IO_REG |= IO_ON_OFF; // Set the ON_OFF bit in IO_REG
  }
  else {
    IO_REG &= ~IO_ON_OFF; // Clear the ON_OFF bit in IO_REG
  }
  
  if (RI_FLAG == true) { // If the RI_FLAG is true (set by the ISR)
    IO_REG |= IO_RI; // Set the RI bit in IO_REG
  }
  else {
    IO_REG &= ~IO_RI; // Clear the RI bit in IO_REG
  }
  
  if (digitalRead(NA) == NA__ON) { // If the NA pin is in its ON state (not necessarily that it is HIGH)
    IO_REG |= IO_NA; // Set the NA bit in IO_REG
  }
  else {
    IO_REG &= ~IO_NA; // Clear the NA bit in IO_REG
  }
  
  if (digitalRead(PGOOD) == PGOOD__ON) { // If the PGOOD pin is in its ON state (not necessarily that it is HIGH)
    IO_REG |= IO_PGOOD; // Set the PGOOD bit in IO_REG
  }
  else {
    IO_REG &= ~IO_PGOOD; // Clear the PGOOD bit in IO_REG
  }

  // Now write IO_REG to I2C
  Wire.write(IO_REG);

  while (ssIridium.available()) { // Now read all available serial bytes (if any) and forward them via I2C
    byte outgoing = ssIridium.read();
    Wire.write(outgoing);
  }
}
