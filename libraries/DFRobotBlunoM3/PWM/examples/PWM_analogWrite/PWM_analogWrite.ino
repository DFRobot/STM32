 int pwmPin = 0;    // pwm connected to digital pin 0
  void setup() {          
     //nothing happens in setup        
 }      
 
  void loop() {          
    
    // fade in from min to max in increments of 5 points:          
    for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {            
                    
        analogWrite(pwmPin, fadeValue);         // sets the value (range from 0 to 255):                 
        delay(30);          // wait for 30 milliseconds to see the dimming effect        
    }        

    // fade out from max to min in increments of 5 points:         
    for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5) {                         
        analogWrite(pwmPin, fadeValue);           // sets the value (range from 0 to 255):        
        delay(30);                              // wait for 30 milliseconds to see the dimming effect         
    }        
 }
