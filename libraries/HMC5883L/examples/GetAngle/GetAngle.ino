#include <Wire.h>
#include <hmc5883l.h>

HMC5883L compass;

void setup(){
	Serial1.begin(115200);
	Wire.begin();
	Serial1.println("Starting the I2C interface.");
	if(compass.init()){
		Serial1.println("connect success!");
	}else{
		Serial1.println("connect failure!");
	}
}

void loop(){
	MagnetometerRaw raw = compass.ReadRawAxis();
	float angle = atan2(raw.YAxis,raw.XAxis)*180/PI+180;
	Serial1.println(angle);
	delay(200);
}
