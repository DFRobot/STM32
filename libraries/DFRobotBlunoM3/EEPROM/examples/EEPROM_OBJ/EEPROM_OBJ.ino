#include <STM32_EEPROM.h>

char temp;
void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
}

void loop() {
  
  EEPROM.write(0x50,11,0x36);
   delay(100);  
  temp = EEPROM.read(0x50,11);  // put your main code here, to run repeatedly:
    Serial1.print("temp is :   ");
    Serial1.print(temp, DEC);
    Serial1.print('\n');
  if (temp == 0x36)
    Serial1.println("test success!");
   else
    Serial1.println("\ntest fail!");
    
    
  while (1);
}
