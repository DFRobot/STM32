#include <Flash.h>

void setup() {
    Serial1.begin(9600);
}

void loop() {
  
    unsigned short temp = 68, readBuf;
    Serial1.println("the data to write is :");
    Serial1.println(temp);
    flash.Write(0x0807B010, &temp, 1);     
    delay(1000);
    
    Serial1.println("the data from reading is :");
    flash.Read(0x0807B010, &readBuf, 1);
    Serial1.println(readBuf);
 
    delay(10000);

}
