/* 
  *  Use the I2C bus with EEPROM 24C256 
  *  
  */

#include <Wire.h> //I2C library
#define EEPROM_ADDR 0x50           // I2C Buss address of 24LC256 256K EEPROM
#define PAGESIZE    64             // 64 bytes each Page of 24LC256
int temp ;
void setup()
{
  Wire.begin();   // join I2C bus (address optional for master)
  Serial1.begin(9600);
}

void loop()
{
         int i;
         byte arr[64];
         byte arread[64];
         // TESTS FOR EACH FUNCTION BEGIN HERE
          Serial1.println("the data is going to be write to the 24LC256:");
         for (i = 0; i < PAGESIZE; i++)
         {
             arr[i] = i;
             Serial1.print(i);
             Serial1.print(' ');
             if (i != 0 && i % 10 == 0)
                Serial1.println("");
         }
         
          Serial1.println("");
         i2c_eeprom_write_page(EEPROM_ADDR, 0, arr, PAGESIZE);
         delay(100); 
       
         i2c_eeprom_read_buffer(EEPROM_ADDR, 0, arread,PAGESIZE);
         Serial1.println("the data is reading from the 24LC256  :");
         delay(1000); 
         
         for (i = 0; i < PAGESIZE; i++)
         {
             Serial1.print(arread[i]);
             Serial1.print(' ');
             if (i != 0 &&i % 10 == 0)
                 Serial1.println("");
         
         }
          Serial1.println("");
         if (memcmp(arr, arread, PAGESIZE) == 0)
         {
             Serial1.println("the driver of the 24LC256 correct!");
         } 
         else
         {
              Serial1.println("the driver of the 24LC256 fault!");
         }
      
         while (1);  
  }

void i2c_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, byte data ) {
    int rdata = data;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.write(rdata);
    Wire.endTransmission();
  }

  // WARNING: address is a page address, 6-bit end will wrap around
  // also, data can be maximum of about 30 bytes, because the Wire library has a buffer of 32 bytes
void i2c_eeprom_write_page( int deviceaddress, unsigned int eeaddresspage, byte* data, byte length ) {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddresspage >> 8)); // MSB
    Wire.write((int)(eeaddresspage & 0xFF)); // LSB
    byte c;
    for ( c = 0; c < length; c++)
      Wire.write(data[c]);
    Wire.endTransmission();
  }

byte i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress ) {
    byte rdata = 0xFF;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaddress,1);
    if (Wire.available()) rdata = Wire.read();
    return rdata;
  }

  // maybe let's not read more than 30 or 32 bytes at a time!
void i2c_eeprom_read_buffer( int deviceaddress, unsigned int eeaddress, byte *buffer, int length ) {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaddress,length);
    int c = 0;
    for ( c = 0; c < length; c++ )
      if (Wire.available()) buffer[c] = Wire.read();
  }


