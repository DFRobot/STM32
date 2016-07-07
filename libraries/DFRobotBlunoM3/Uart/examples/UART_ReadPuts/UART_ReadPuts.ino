
void setup() {
  /* initialize both serial ports.*/
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  Serial4.begin(9600);
  Serial5.begin(9600);
  Serial1.println("Bluno M3 Serials test begin .......!");
  Serial1.println("Please choose the Serial to test,and Send a string for it:");
  
}


void loop() {
  char arr[50];
  int len;
  char ch;
  int i;
  if (Serial1.available()) {
   
    Serial1.println("the string you input just now is :");
    Serial1.println( ch = Serial1.read());
    Serial1.println("Please choose the Serial to test next,and Send a string for it:");
  }
  if ( Serial2.available()) {
    Serial2.println("the string you input just now is :");
    Serial2.println( ch = Serial2.read());
    Serial2.println("Please choose the Serial to test next,and Send a string for it:");
  }
  if (Serial3.available()) {
    Serial3.println("the string you input just now is :");
    Serial3.println( ch = Serial3.read());
    Serial3.println("Please choose the Serial to test next,and Send a string for it:");}
  if (Serial4.available()) {
    Serial4.println("the string you input just now is :");
    Serial4.println( ch = Serial4.read());
    Serial4.println("Please choose the Serial to test next,and Send a string for it:");  }
  if ( Serial5.available()) {
    Serial5.println("the string you input just now is :");
    Serial5.println( ch = Serial5.read());
    Serial5.println("Please choose the Serial to test next,and Send a string for it:"); }
  
}
