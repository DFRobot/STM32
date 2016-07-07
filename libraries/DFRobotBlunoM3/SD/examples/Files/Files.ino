/*
  SD card basic file example

 This example shows how to create and destroy an SD card file
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4

 This example code is in the public domain.

 */
#include <SPI.h>
#include <SD.h>

File myFile;
int cs_pin = 4;       //D4 is attached as cs pin
void setup()
{
  // Open Serial1 communications and wait for port to open:
  Serial1.begin(9600);
  while (!Serial1) {
    ; // wait for Serial1 port to connect. Needed for Leonardo only
  }


  Serial1.print("Initializing SD card...");

  if (!SD.begin(cs_pin)) {
    Serial1.println("initialization failed!");
    return;
  }
  Serial1.println("initialization done.");

  if (SD.exists("example.txt")) {
    Serial1.println("example.txt exists.");
  }
  else {
    Serial1.println("example.txt doesn't exist.");
  }

  // open a new file and immediately close it:
  Serial1.println("Creating example.txt...");
  myFile = SD.open("example.txt", FILE_WRITE);
  myFile.close();

  // Check to see if the file exists:
  if (SD.exists("example.txt")) {
    Serial1.println("example.txt exists.");
  }
  else {
    Serial1.println("example.txt doesn't exist.");
  }

  // delete the file:
  Serial1.println("Removing example.txt...");
  SD.remove("example.txt");

  if (SD.exists("example.txt")) {
    Serial1.println("example.txt exists.");
  }
  else {
    Serial1.println("example.txt doesn't exist.");
  }
}

void loop()
{
  // nothing happens after setup finishes.
}


