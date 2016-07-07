/*
  Web Server
 
 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield. 
 
 * Analog inputs attached to pins A0 through A5 (optional)
 
 */

#include <SPI.h>
#include <Ethernet.h>
//注意！官网的SPI接口使用的是 D10作为 SS接口，这里需要根据实际SS接线情况定义一次SS管脚

#define SS    10  
#define nRST  8  
#define nPWDN 9  
#define nINT 3 

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,177);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
  ////下面是非常重要的设置，如果没有可靠的复位设置,W5200可能不工作 !!!! /////////
  pinMode(SS,OUTPUT);  
  pinMode(nRST,OUTPUT);
  pinMode(nPWDN,OUTPUT);
  pinMode(nINT,INPUT);  
  digitalWrite(nPWDN,LOW);  //enable power
 
  digitalWrite(nRST,LOW);  //Reset W5200
  delay(10);
  digitalWrite(nRST,HIGH);  
 delay(200);       // wait W5200 work
 /////////////////////////////////////////////////////////////
 // Open Serial1 communications and wait for port to open:
  Serial1.begin(9600);
   while (!Serial1) {
    ; // wait for Serial1 port to connect. Needed for Leonardo only
  }


  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial1.print("server is at ");
  Serial1.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial1.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial1.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connnection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
                    // add a meta refresh tag, so the browser pulls again every 5 seconds:
          client.println("<meta http-equiv=\"refresh\" content=\"5\">");
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");       
          }
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial1.println("client disonnected");
  }
}

