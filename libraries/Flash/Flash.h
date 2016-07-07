#ifndef __Flash_H_
#define __Flash_H_

#include "variant.h"
#include <stdint.h>
#include "stm32f10x_flash.h"

/********************************
*
*
*
*
**********************************/


#define FLASH_START_ADDR 0x0807B000       //the start address of the user's data space.
#define FLASH_END_ADDR   0x08080000       //the end address of the user's data space.

// Delay definition //
#define EraseTimeout		((uint32)0x00000FFF)
#define ProgramTimeout		((uint32)0x0000001F)

#define IS_FLASH_ADDRESS(ADDRESS)   (((ADDRESS) >= FLASH_START_ADDR) && ((ADDRESS) <= FLASH_END_ADDR   ))



/*
typedef unsigned long  uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char  uint8_t;
*/



class Flash {

	public:
		Flash();
		//~Flash();
		FLASH_Status ErasePage(uint32_t Page_Address);
		FLASH_Status EraseAllPages(void);
		void Read(uint32_t Addr, void *data, uint32_t NumByteToRead);
		
        uint16_t Read(uint32_t Addr);
		FLASH_Status Write(uint32_t WriteAddr, uint16_t data);
		void Write(uint32_t WriteAddr, void *data, uint32_t NumByteToWrite);
	
		uint32_t GetFlashSpace();
		
		
    private:
		uint32_t addr_Start;
		uint32_t addr_End;
	
		

};

extern uint8_t Buffer[2048];
extern Flash flash;






#endif /*__Flash_H_*/
