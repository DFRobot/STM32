#include <Wire.h>

#define DEVICE (0x53)      //ADXL345 device address
#define TO_READ (6)        //num of bytes we are going to read each time (two bytes for each axis)

char buff[TO_READ] ;        //6 bytes buffer for saving data read from the device
char str[512];              //string buffer to transform data before sending it to the Serial1 port
char regAddress = 0x32;      //first axis-acceleration-data register on the ADXL345
int16_t x, y, z;                //three axis acceleration data
double roll = 0.00, pitch = 0.00;   //Roll & Pitch are the angles which rotate by the axis X and y 
//in the sequence of R(x-y-z),more info visit
// https://www.dfrobot.com/wiki/index.php?title=How_to_Use_a_Three-Axis_Accelerometer_for_Tilt_Sensing#Introduction

void setup() {
  Wire.begin();         // join i2c bus (address optional for master)
  Serial1.begin(9600);  // start Serial1 for output


   while (!Serial1) {
        ; // wait for serial port to connect. Only needed for ATmega32u4-based boards (Leonardo, etc).
    }
  
  //Turning on the ADXL345
  writeTo(DEVICE, 0x2D, 0); 
  delay(30);     
  writeTo(DEVICE, 0x2D, 16);
  delay(30);  
  writeTo(DEVICE, 0x2D, 8);
  delay(30);  
}

void loop() {
  int i = 0;
  readFrom(DEVICE, regAddress, TO_READ, buff); //read the acceleration data from the ADXL345
                                              //each axis reading comes in 10 bit resolution, ie 2 bytes.  Least Significat Byte first!!
                                              //thus we are converting both bytes in to one int

  x = *((int16_t *)(&buff[0]));
  y = *((int16_t *)(&buff[2])); 
  z = *((int16_t *)(&buff[4]));
  Serial1.println( x); 
  Serial1.println(y);
  Serial1.println(z);
  

  //we send the x y z values as a string to the Serial1 port
  Serial1.print("The acceleration info of x, y, z are:");
  sprintf(str, "%d %d %d", x, y, z);  
  Serial1.print(str);
  Serial1.write(10);
  Serial1.println("======================================================");
  //Roll & Pitch calculate
  RP_calculate();
  Serial1.print("Roll:"); Serial1.println( roll ); 
  Serial1.print("Pitch:"); Serial1.println( pitch );
  Serial1.println("");
  //It appears that delay is needed in order not to clog the port
  
  delay(500);
}

//---------------- Functions
//Writes val to address register on device
void writeTo(int device, byte address, byte val) {
  int ret;
  Wire.beginTransmission(device); //start transmission to device 
  Wire.write(address);        // send register address
  Wire.write(val);        // send value to write
  Wire.endTransmission(); //end transmission
 
}

//reads num bytes starting from address register on device in to buff array
void readFrom(int device, byte address, int num, byte buff[]) {
  int ret = 0;
  
  Wire.beginTransmission(device); //start transmission to device 
  Wire.write(address);        //sends address to read from
  Wire.endTransmission(); //end transmission

  Wire.beginTransmission(device); //start transmission to device
  Wire.requestFrom(device, num);    // request 6 bytes from device
  
  int i = 0;
  while(Wire.available())    //device may send less than requested (abnormal)
  { 
    buff[i] = Wire.read(); // receive a byte
    delay(30);
    i++;
  }
  Wire.endTransmission(); //end transmission
}

//calculate the Roll&Pitch
void RP_calculate(){
  double x_Buff = float(x);
  double y_Buff = float(y);
  double z_Buff = float(z);
  roll = atan2(y_Buff , z_Buff) * 57.3;
  pitch = atan2((- x_Buff) , sqrt(y_Buff * y_Buff + z_Buff * z_Buff)) * 57.3;
}

