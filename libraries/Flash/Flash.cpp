#include "arduino.h"
#include "Flash.h" 
#include <string.h>





#define PAGE_SIZE   2048       //һ��page ��2048���ֽ�
#define PAGE_NUM    255       // page num ��stm32f103reg6 page number arrange: 0 ~ 255;

Flash flash;
uint8_t Buffer[2048];

static FLASH_Status WriteBuffer(uint32_t WriteAddr, void *data, uint32_t len)
{
	FLASH_Status state;
	

	
	FLASH_Unlock();
       
	for (int i = 0; i <((len + 1) / 2) ; i += 1)
	{
		state = FLASH_ProgramHalfWord(WriteAddr + i * 2, ((uint16_t *)data)[i]);
		if (state != FLASH_COMPLETE)
                {
			break;
                        
                }
	}

	FLASH_Lock();

	
	return state;


}


Flash::Flash()
{

	
 	addr_Start = FLASH_START_ADDR;
	addr_End = FLASH_START_ADDR;
	
    
	
}
/*
Flash::~Flash()
{
	//delete [] Buffer;



}*/

/*
*@Addr: ָ���������ڵ�ַ��
*@NumByteToWrite: һ��Ҫ��ȡ���ֽ�����
*@   ����ֵ: �������ֽ���
*/
	
	
void Flash::Read(uint32_t Addr, void *data, uint32_t NumByteToRead)
{
	
    uint32_t count;
           
      memcpy(data, (void *)(Addr), NumByteToRead);

	
}
	
	

/*@����ֵ: �������ֽ���*/

uint16_t Flash::Read(uint32_t Addr)
{
		
    return *(uint16_t *)(Addr);

}
/*һ���ֽ�д����
*д֮ǰ��Ҫ����
*@WriteAddr: ������Ҫ����ĵ�ַ��
*
*@data:��Ҫд�����ݡ�
*
**/



FLASH_Status Flash::Write(uint32_t WriteAddr, uint16_t data)
{
	
	FLASH_Status state;
	
	assert_param(IS_FLASH_ADDRESS(WriteAddr));
         
	FLASH_Unlock();
        FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP|FLASH_FLAG_PGERR |FLASH_FLAG_WRPRTERR);	
 
	state = FLASH_ProgramHalfWord(WriteAddr, data);
        
        FLASH_Lock();
  
    return state;

}






void Flash::Write(uint32_t WriteAddr, void *data, uint32_t NumByteToWrite)
{
	uint32_t pageRemain, pageAddr, pageOff, pagepos; 
			char *data1 = (char *)data;
			
		pageRemain = PAGE_SIZE - (WriteAddr - FLASH_START_ADDR) % PAGE_SIZE; //��ҳʣ����ֽ���?	
		
		pageAddr = (((WriteAddr - FLASH_START_ADDR) / PAGE_SIZE ) * PAGE_SIZE) + FLASH_START_ADDR; //WriteAddr ����ҳ��ҳ��ַ��
		pageOff = WriteAddr - pageAddr;
		pagepos = (WriteAddr - FLASH_START_ADDR) / PAGE_SIZE;
			
			if (NumByteToWrite <= pageRemain)
				  pageRemain = NumByteToWrite;
	
		while(1)
		{	 
			 int i = 0;
					 
			 Read(pagepos * PAGE_SIZE + FLASH_START_ADDR , (void *)Buffer, PAGE_SIZE); //��ȡ����page�����ݡ�
	
			 for ( i = 0; i < pageRemain; i += 1)
			 {
				if (Buffer[i + pageOff] != 0xFF)
					break;
			 }
	
			 if (i < pageRemain)
			 {
							FLASH_Status status = FLASH_ErasePage(pageAddr);
				for (i = 0; i < pageRemain; i += 1)
					Buffer[i + pageOff] = data1[i];
	
				
				WriteBuffer(( pagepos * PAGE_SIZE + FLASH_START_ADDR), Buffer, PAGE_SIZE);
			 }
			 else
			 {
									
				WriteBuffer((pageOff + pagepos * PAGE_SIZE + FLASH_START_ADDR), data1, pageRemain);
				
	
			 }
	
			 if(NumByteToWrite == pageRemain)
				break;//д�������?
			 else //NumByteToWrite>pageremain
			 {
				  
							  pageOff = 0;
							  pagepos++;
							  pageAddr = pagepos * PAGE_SIZE + FLASH_START_ADDR;
				  data1 += pageRemain;
				  WriteAddr +=pageRemain; 
				  NumByteToWrite -=pageRemain;	  //��ȥ�Ѿ�д���˵��ֽ���
				  if(NumByteToWrite > PAGE_SIZE)
					pageRemain = PAGE_SIZE; //�ο���д��2048���ֽ�
				  else 
					pageRemain = NumByteToWrite;	 //����2048���ֽ���
			}
			 
		}	


}



FLASH_Status Flash::ErasePage(uint32_t Page_Address)
{
   
  	return FLASH_ErasePage(Page_Address);

}

FLASH_Status Flash::EraseAllPages(void)
{
	int i = 0;
	FLASH_Status state;
	for (i = 11; i <= PAGE_NUM; i++)
	{
		
		if ((state = ErasePage(i * PAGE_SIZE + FLASH_START_ADDR)) != FLASH_COMPLETE)
		{
			
			return state;

		}
	}

}





/*�����û��ܹ�ʹ�õ�flash�Ŀռ��С����λ: Byte   */
uint32_t Flash::GetFlashSpace(void)
{
	
	return flash.addr_End - flash.addr_Start;

}










