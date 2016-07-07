#include <Arduino.h> 
 int pwmPin = 0;    // PWM connected to digital pin 0
 int flag = 1;  
 void setup() {
     //initailizing the pwmPin,  setting the period of PWM as 2000Hz and it's mode as 8'bit. 
     pwmMode(pwmPin, 2000, PWM_8_BIT);
 }

 void loop() {
  			
     // output the duty of the PWM , one is 39% based on the formula privided above.  
     pwmWrite(pwmPin, 100); 
     while (1);
       		 
}               
                
