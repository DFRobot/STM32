#ifndef HMC5883L_DF_H
#define HMC5883L_DF_H

#include <inttypes.h>
#include <Arduino.h>
#include "../Wire/Wire.h"


#define HMC5883L_Address 0x1E
#define DataRegisterBegin 0x03

struct MagnetometerRaw{
	int16_t XAxis;
	int16_t YAxis;
	int16_t ZAxis;
};

class HMC5883L{
public:
	HMC5883L();
	char init();
	MagnetometerRaw ReadRawAxis();

protected:
	void Write(int16_t address, int16_t byte);
	uint8_t* Read(int16_t address, int16_t length);
};


#endif
