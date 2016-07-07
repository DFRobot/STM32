

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
        
}

void loop() {
  // put your main code here, to run repeatedly:
  static int i;
   if (Serial1.available())
   {
        i = Serial1.read();
        
        for (i = 0; i < 39; i++)
        {
           
              pinMode(i, OUTPUT);
              digitalWrite(i, HIGH);   // turn the LED on (HIGH is the voltage level)
              delay(3000);               // wait for a second
              digitalWrite(i, LOW);    // turn the LED off by making the voltage LOW
              delay(3000);               // wait for a second
       
        }
  
  }
  
}
