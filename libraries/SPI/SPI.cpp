/*
 * Copyright (c) 2014 MakerLab.me & Andy Sze(andy.sze.mail@gmail.com) 
 * ported to stm32
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@bug.st>
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "SPI.h"
#include "stm32f10x.h"

SPIClass::SPIClass(SPI_TypeDef *_spi, void(*_initCb)(void)) :
	spi(_spi),  initCb(_initCb), initialized(false),bitOrder(SPI_FirstBit_MSB),
  mode(SPI_MODE0),divider(SPI_CLOCK_DIV4)
{
	
}

void SPIClass::begin() {
	
	init();

	setClockDivider(divider);
	setDataMode( mode);
	setBitOrder( bitOrder);
}

#if 0
void SPIClass::begin(uint8_t _pin) {
	init();

	uint32_t spiPin = BOARD_PIN_TO_SPI_PIN(_pin);
	PIO_Configure(
		g_APinDescription[spiPin].pPort,
		g_APinDescription[spiPin].ulPinType,
		g_APinDescription[spiPin].ulPin,
		g_APinDescription[spiPin].ulPinConfiguration);

	// Default speed set to 4Mhz
	setClockDivider(_pin, 21);
	setDataMode(_pin, SPI_MODE0);
	setBitOrder(_pin, MSBFIRST);
}
#endif

void SPIClass::init() {
	if (initialized)
		return;
	initCb();
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_CRCPolynomial = 7;

  SPI_Init(spi,&SPI_InitStructure);
  SPI_Cmd(spi,ENABLE);
	initialized = true;
}

#if 0
void SPIClass::end(uint8_t _pin) {
	uint32_t spiPin = BOARD_PIN_TO_SPI_PIN(_pin);
	// Setting the pin as INPUT will disconnect it from SPI peripheral
	pinMode(spiPin, INPUT);
}
#endif

void SPIClass::end() {
	SPI_Cmd(spi,DISABLE);
	initialized = false;
}

void SPIClass::setBitOrder( uint16_t _bitOrder) {

  SPI_Cmd(spi,DISABLE);
  SPI_InitStructure.SPI_FirstBit = _bitOrder;
  bitOrder = _bitOrder;

  SPI_Init(spi,&SPI_InitStructure);
  SPI_Cmd(spi,ENABLE);
}

void SPIClass::setDataMode( uint8_t _mode) {

  mode = _mode;
  SPI_Cmd(spi,DISABLE);
  if(_mode == SPI_MODE0)
  {
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  } else if(_mode == SPI_MODE1)
  {
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  } else if(_mode == SPI_MODE2)
  {
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  } else if(_mode == SPI_MODE3)
  {
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  }

  SPI_Init(spi,&SPI_InitStructure);
  SPI_Cmd(spi,ENABLE);
}

void SPIClass::setClockDivider( uint8_t _divider) {

  SPI_Cmd(spi,DISABLE);
  SPI_InitStructure.SPI_BaudRatePrescaler = _divider;
  divider = _divider;

  SPI_Init(spi,&SPI_InitStructure);
  SPI_Cmd(spi,ENABLE);
}

uint8_t SPIClass::transfer( uint8_t _data)
 {
	u8 retry=0;

	while (SPI_I2S_GetFlagStatus(spi,SPI_I2S_FLAG_TXE) == RESET);
	{
		retry++;
		if(retry>200)
			return 0;
	}
	SPI_I2S_SendData(spi,_data);
	retry = 0;

	while (SPI_I2S_GetFlagStatus(spi,SPI_I2S_FLAG_RXNE) == RESET)
	{
		retry++;
		if(retry>200)
			return 0;
	}

	return  SPI_I2S_ReceiveData(spi);//->SPI_RDR;
}

void SPIClass::attachInterrupt(void) {
	// Should be enableInterrupt()
}

void SPIClass::detachInterrupt(void) {
	// Should be disableInterrupt()
	
}

#if SPI_INTERFACES_COUNT > 0
static void SPI_1_Init(void) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

  pinMode(12,AF_OUTPUT_PUSHPULL);           //MISO
  pinMode(11,AF_OUTPUT_PUSHPULL);           //MOSI
  pinMode(13, AF_OUTPUT_PUSHPULL);          //SCK
  

}


SPIClass SPI(STM32_SPI1,  SPI_1_Init);

#if SPI_INTERFACES_COUNT > 1
static void SPI_2_Init(void) {
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);

  pinMode(MISO1,AF_OUTPUT_PUSHPULL);
  pinMode(MOSI1,AF_OUTPUT_PUSHPULL);
  pinMode(SCK1, AF_OUTPUT_PUSHPULL);
}

SPIClass SPI1(SPI_INTERFACE1,  SPI_2_Init);

#if SPI_INTERFACES_COUNT > 2
static void SPI_3_Init(void) {
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);

  pinMode(MISO2,AF_OUTPUT_PUSHPULL);
  pinMode(MOSI2,AF_OUTPUT_PUSHPULL);
  pinMode(SCK2, AF_OUTPUT_PUSHPULL);
}

SPIClass SPI_2(SPI_INTERFACE2,  SPI_3_Init);
#endif//>2
#endif//>1
#endif//>0
