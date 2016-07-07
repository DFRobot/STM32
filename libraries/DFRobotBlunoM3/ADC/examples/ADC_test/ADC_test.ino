 


int sensorPin = 0;    // select the input pin for the potentiometer              
int sensorValue = 0;  // variable to store the value coming from the sensor        
int i;

void setup() {          
   char *s = new char[5];            
   Serial1.begin(9600);        
              
}        

void loop() {  
    Serial1.println("sensorValue data list:");       
   for (i = 0; i < 12; i++)  
   {
           
       sensorValue = analogRead(i);
       Serial1.println((int)sensorValue);        
       
   }   
   delay(1000);    
}

