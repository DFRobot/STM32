 int sensorPin = 0;    // select the input pin for the potentiometer        
 int sensorValue = 0;  // variable to store the value coming from the sensor        
 
 void setup() {          
    Serial1.begin(9600);    
    adcMode(sensorPin, ADC_12_BIT);  
 }       

 void loop() {	          
            
    sensorValue = adcRead(sensorPin);
    Serial1.println((int)sensorValue);        
    delay(1000);               
}
