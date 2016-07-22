#include <FreeSixIMU.h>
#include <FIMU_ADXL345.h>
#include <FIMU_ITG3200.h>

#include <Wire.h>

float angles[3]; // yaw pitch roll

// Set the FreeSixIMU object
FreeSixIMU sixDOF = FreeSixIMU();

void setup() { 
  Serial1.begin(9600);
  Wire.begin();
  
  delay(5);
  sixDOF.init(); //begin the IMU
  delay(5);
}

void loop() { 
  
  sixDOF.getEuler(angles);
  
  Serial1.print(angles[0]);
  Serial1.print(" | ");  
  Serial1.print(angles[1]);
  Serial1.print(" | ");
  Serial1.println(angles[2]);
  
  delay(100); 
}

