#include <Flash.h>


void setup() {
  
    Serial1.begin(9600);
    
}
#define BUFSIZE     1026
void loop() {
    int i, j;
    char buf[BUFSIZE];
    char readbuf[BUFSIZE];
     
    Serial1.println("the data to write is :");
    for (i = 0; i < BUFSIZE; i++)
    {
        buf[i] = i;
        Serial1.print(i);
        Serial1.print(' ');
        if (i != 0 && i % 10 == 0)
            Serial1.println("");
    }
    
    for (i = 0; i < 19; i++)
    {
      
        flash.Write(0x0807B000 + 0x402 * i, buf, BUFSIZE);     
        delay(1000);
        
        Serial1.println("the data from reading is :");
        flash.Read(0x0807B000 + 0x402 * i, &readbuf, BUFSIZE);
        if (!memcmp(buf, readbuf, BUFSIZE))
        {
        
          Serial1.print('i');
          Serial1.print('=');
          Serial1.print(i);
          Serial1.println("test success!");
          
        
        }
        else
        {
          for (j = 0; j< BUFSIZE; j++)
          {
          Serial1.print(readbuf[j]);
          Serial1.print(' ');
          if (j != 0 && j % 10 == 0)
              Serial1.println("");
          }
         delay(10000);
        }
          
    }
    delay(10000);

}
