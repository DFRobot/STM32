/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2014 DFrobot.  All right reserved.
 * Copyright (c) 2011 Arduino.  All right reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/**
 *  @brief Wire library, ported from Arduino. Provides a simplistic
 *  interface to i2c.
 */

#include "Wire.h" 

/* low level conventions:
 * - SDA/SCL idle high (expected high)
 * - always start with i2c_delay rather than end
 */
uint32_t i2c_delay = 1;

void i2c_start(Port port) {
	digitalWrite(port.sda,HIGH);
    digitalWrite(port.scl,HIGH);
    I2C_DELAY;
    digitalWrite(port.sda,LOW);
    I2C_DELAY;
    digitalWrite(port.scl,LOW);
}

void i2c_stop(Port port) {
	
	digitalWrite(port.sda,LOW);
    digitalWrite(port.scl,LOW);
	
    I2C_DELAY;
    digitalWrite(port.scl,HIGH);
	I2C_DELAY;
    digitalWrite(port.sda,HIGH);
}

boolean i2c_get_ack(Port port) {
    int time = 0;

    I2C_DELAY;
	 digitalWrite(port.sda,HIGH);

    I2C_DELAY;
       digitalWrite(port.scl,HIGH);
    I2C_DELAY;
    while (digitalRead(port.sda))
    {
    	time++;

		if (time > 250)
			{
                     i2c_stop(port);
				
					 return false;
		}


	}

      digitalWrite(port.scl,LOW);
      return true;
   
}

void i2c_send_ack(Port port) {
    I2C_DELAY;
    digitalWrite(port.sda,LOW);
    I2C_DELAY;
    digitalWrite(port.scl,HIGH);
    I2C_DELAY;
    digitalWrite(port.scl,LOW);
}

void i2c_send_nack(Port port) {
    I2C_DELAY;
    digitalWrite(port.sda,HIGH);
    I2C_DELAY;
    digitalWrite(port.scl,HIGH);
}

uint8_t i2c_shift_in(Port port) {
    uint8_t data = 0;

    int i;
    for (i=0;i<8;i++) {
        I2C_DELAY;
        digitalWrite(port.scl,HIGH);
        I2C_DELAY;
        data += digitalRead(port.sda) << (7-i);
        I2C_DELAY;
        digitalWrite(port.scl,LOW);
    }

    return data;
}

void i2c_shift_out(Port port, uint8_t val) {
    int i;
	//Serial1.begin(9600);

	//digitalWrite(port.scl,LOW);
    for (i=0;i<8;i++) {
        I2C_DELAY;
        digitalWrite(port.sda, !!(val & (1 << (7 - i))));
        I2C_DELAY;
        digitalWrite(port.scl, HIGH);
        I2C_DELAY;
        digitalWrite(port.scl, LOW); 
		
    } 
}

TwoWire::TwoWire() {
    i2c_delay = 0;
    rx_buf_idx = 0;
    rx_buf_len = 0;
    tx_addr = 0;
    tx_buf_idx = 0;
    tx_buf_overflow = false;
}

/*
 * Sets pins SDA and SCL to OUPTUT_OPEN_DRAIN, joining I2C bus as
 * master.  If you want them to be some other pins, use begin(uint8,
 * uint8);
 */
void TwoWire::begin() {
    begin(SDA, SCL);
}

/*
 * Joins I2C bus as master on given SDA and SCL pins.
 */
void TwoWire::begin(uint8_t sda, uint8_t scl) {
    port.sda = sda;
    port.scl = scl;
    pinMode(scl, OUTPUT_OD);
    pinMode(sda, OUTPUT_OD);
    digitalWrite(scl, HIGH);
    digitalWrite(sda, HIGH);
}

void TwoWire::beginTransmission(uint8_t slave_address) {
    tx_addr = slave_address;
    tx_buf_idx = 0;
    tx_buf_overflow = false;
    rx_buf_idx = 0;
    rx_buf_len = 0;
}

void TwoWire::beginTransmission(int slave_address) {
    beginTransmission((uint8_t)slave_address);
}

uint8_t TwoWire::endTransmission(void) {
    if (tx_buf_overflow) return EDATA;
   // Serial1.begin(9600);
    i2c_start(port);

    i2c_shift_out(port, (tx_addr << 1) | I2C_WRITE);
    if (!i2c_get_ack(port)) return ENACKADDR;

    // shift out the address we're transmitting to
    for (uint8_t i = 0; i < tx_buf_idx; i++) {
        uint8_t ret = writeOneByte(tx_buf[i]); 
        if (ret) return ret;    // SUCCESS is 0
    } 
    i2c_stop(port);

    tx_buf_idx = 0;
    tx_buf_overflow = false;
    return SUCCESS;
}

uint8_t TwoWire::requestFrom(uint8_t address, int num_bytes) {
    if (num_bytes > WIRE_BUFSIZ) num_bytes = WIRE_BUFSIZ;

    rx_buf_idx = 0;
    rx_buf_len = 0;
    while (rx_buf_len < num_bytes) {
        if(!readOneByte(address, rx_buf + rx_buf_len)) rx_buf_len++;
        else break;
    }
    return rx_buf_len;
}

uint8_t TwoWire::requestFrom(int address, int numBytes) {
    return TwoWire::requestFrom((uint8_t)address, (uint8_t) numBytes);
}

void TwoWire::write(uint8_t value) {
    if (tx_buf_idx == WIRE_BUFSIZ) {
        tx_buf_overflow = true;
        return;
    }

    tx_buf[tx_buf_idx++] = value;
}

void TwoWire::write(uint8_t* buf, int len) {
    for (uint8_t i = 0; i < len; i++) write(buf[i]);
}

void TwoWire::write(int value) {
    write((uint8_t)value);
}

void TwoWire::write(int* buf, int len) {
    write((uint8_t*)buf, (uint8_t)len);
}

void TwoWire::write(char* buf) {
    uint8_t *ptr = (uint8_t*)buf;
    while(*ptr) {
        write(*ptr);
        ptr++;
    }
}

uint8_t TwoWire::available() {
    return rx_buf_len - rx_buf_idx;
}

uint8_t TwoWire::read() {
    if (rx_buf_idx == rx_buf_len) return 0;
    return rx_buf[rx_buf_idx++];
}

// private methods

uint8_t TwoWire::writeOneByte(uint8_t byte) {
    i2c_shift_out(port, byte);
    if (!i2c_get_ack(port)) return ENACKTRNS;
//Serial1.begin(9600);
//Serial1.println("44444444444444444444444444444");
    return SUCCESS;
}

uint8_t TwoWire::readOneByte(uint8_t address, uint8_t *byte) {
    i2c_start(port);

    i2c_shift_out(port, (address << 1) | I2C_READ);
    if (!i2c_get_ack(port)) return ENACKADDR;

    *byte = i2c_shift_in(port);

    i2c_send_nack(port);
    i2c_stop(port);

    return SUCCESS;      // no real way of knowing, but be optimistic!
}

// Declare the instance that the users of the library can use
TwoWire Wire;

