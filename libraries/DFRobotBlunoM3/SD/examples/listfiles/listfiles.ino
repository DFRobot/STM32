/*
  Listfiles
 
 This example shows how print out the files in a 
 directory on a SD card 
 	
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4

 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */
#include <SPI.h>
#include <SD.h>

File root;

void setup()
{
  // Open Serial1 communications and wait for port to open:
  Serial1.begin(9600);
  while (!Serial1) {
    ; // wait for Serial1 port to connect. Needed for Leonardo only
  }

  Serial1.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
  pinMode(10, OUTPUT);

  if (!SD.begin(10)) {
    Serial1.println("initialization failed!");
    return;
  }
  Serial1.println("initialization done.");

  root = SD.open("/");

  printDirectory(root, 0);

  Serial1.println("done!");
}

void loop()
{
  // nothing happens after setup finishes.
}

void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial1.print('\t');
     }
     Serial1.print(entry.name());
     if (entry.isDirectory()) {
       Serial1.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial1.print("\t\t");
       Serial1.println(entry.size(), DEC);
     }
     entry.close();
   }
}



