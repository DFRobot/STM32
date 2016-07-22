#include "hmc5883l.h"

HMC5883L::HMC5883L(){
}

char HMC5883L::init(){
	this->Write(0x02, 0x00);
	
	Wire.beginTransmission(HMC5883L_Address);
	Wire.write(10);
	Wire.endTransmission();
	Wire.requestFrom(HMC5883L_Address, 1);
	if (Wire.available() == 1 && Wire.read() == 0x48){
		return 1;
	}
	else
		return 0;
}

MagnetometerRaw HMC5883L::ReadRawAxis(){
	uint8_t* buffer = Read(DataRegisterBegin, 6);
	MagnetometerRaw raw = MagnetometerRaw();
	raw.XAxis = (buffer[0] << 8) | buffer[1];
	raw.ZAxis = (buffer[2] << 8) | buffer[3];
	raw.YAxis = (buffer[4] << 8) | buffer[5];
	return raw;
}

void HMC5883L::Write(int16_t address, int16_t data)
{
	Wire.beginTransmission(HMC5883L_Address);
	Wire.write(address);
	Wire.write(data);
	Wire.endTransmission();
}

uint8_t* HMC5883L::Read(int16_t address, int16_t length)
{
	Wire.beginTransmission(HMC5883L_Address);
	Wire.write(address);
	Wire.endTransmission();

	Wire.beginTransmission(HMC5883L_Address);
	Wire.requestFrom(HMC5883L_Address, length);

	uint8_t buffer[length];
	if (Wire.available() == length)
	{
		for (uint8_t i = 0; i < length; i++)
		{
			buffer[i] = Wire.read();
		}
	}
	Wire.endTransmission();

	return buffer;
}