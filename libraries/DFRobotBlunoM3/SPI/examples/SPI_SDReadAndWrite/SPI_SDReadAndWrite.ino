#include "SPI.h"


#define SD_TYPE_ERR     0X00
#define SD_TYPE_MMC     0X01
#define SD_TYPE_V1      0X02
#define SD_TYPE_V2      0X04
#define SD_TYPE_V2HC    0X06	   

#define CMD0    0      
#define CMD1    1
#define CMD8    8     
#define CMD9    9       
#define CMD10   10      
#define CMD12   12     
#define CMD16   16      
#define CMD17   17    
#define CMD18   18    
#define CMD23   23   
#define CMD24   24     
#define CMD25   25   
#define CMD41   41     
#define CMD55   55    
#define CMD58   58    
#define CMD59   59      
#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF
//SD¿¨»ØÓ¦±ê¼Ç×Ö
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF


const int slaveSelectPin = 4;
u8  SD_Type=0;

u8 SD_WaitReady(void)
{
  u32 t=0;
  do
  {
    if(SPI.transfer(0xff)==0xff)
    {
      return 0;//is ok!!!
    }
    t++;
  }while(t<0xffffff);
  return 1;
}

void SD_DisSelect(void)
{
	digitalWrite(slaveSelectPin, HIGH);
	SPI.transfer(0xff);
}

u8 SD_Select(void)
{
	digitalWrite(slaveSelectPin, LOW);
	if(SD_WaitReady()==0)
          return 0;
	SD_DisSelect();
	return 1;
}

u8 SD_SendCmd(u8 cmd, u32 arg, u8 crc)
{
    u8 r1;	
    u8 Retry=0; 
    SD_DisSelect();
    if(SD_Select())
    {
      return 0XFF;
    }
    SPI.transfer(cmd | 0x40);//·Ö±ðÐ´ÈëÃüÁî
    SPI.transfer(arg >> 24);
    SPI.transfer(arg >> 16);
    SPI.transfer(arg >> 8);
    SPI.transfer(arg);	  
    SPI.transfer(crc); 
    if(cmd==CMD12)
      SPI.transfer(0xff);
    Retry=0X1F;
    do
    {
	r1=SPI.transfer(0xFF);
    }while((r1&0X80) && Retry--);	 

    return r1;
}

u8 SD_Initialize(void)
{
	u16 retry;
	u8 r1;
	u8 buf[4]; 
	u16 i;
	// set the slaveSelectPin as an output:
	pinMode (slaveSelectPin, OUTPUT);
	// initialize SPI:
	SPI.begin();
        SPI.setClockDivider(SPI_CLOCK_DIV256);
        SPI.setDataMode(SPI_MODE3);
	//SPI.setDataMode(SPI_MODE3);
	SPI.setClockDivider(SPI_CLOCK_DIV256);
	for(int i=0;i<10;i++)
	SPI.transfer(0xff);
	retry=20;
	do
	{
		r1=SD_SendCmd(CMD0,0,0x95);
	}while((r1!=0x01)&&retry--);
	SD_Type=0;
	if(r1==0X01)
	{
		if(SD_SendCmd(CMD8,0x1AA,0x87)==1)//SD V2.0
		{
			for(int i=0;i<4;i++)buf[i]=SPI.transfer(0XFF);	
			if(buf[2]==0X01&&buf[3]==0XAA)
			{
				retry=0XFFFE;
				do
				{
					SD_SendCmd(CMD55,0,0X01);	
					r1=SD_SendCmd(CMD41,0x40000000,0X01);
				}while(r1&&retry--);
				if(retry&&SD_SendCmd(CMD58,0,0X01)==0)
				{
					for(int i=0;i<4;i++)buf[i]=SPI.transfer(0XFF);
					if(buf[0]&0x40)SD_Type=SD_TYPE_V2HC;   
					else SD_Type=SD_TYPE_V2;   
				}
			}
		}else//SD V1.x/ MMC	V3
		{
			SD_SendCmd(CMD55,0,0X01);	
			r1=SD_SendCmd(CMD41,0,0X01);	
			if(r1<=1)
			{		
				SD_Type=SD_TYPE_V1;
				retry=0XFFFE;
				do 
				{
					SD_SendCmd(CMD55,0,0X01);
					r1=SD_SendCmd(CMD41,0,0X01);
				}while(r1&&retry--);
			}else
			{
				SD_Type=SD_TYPE_MMC;//MMC V3
				retry=0XFFFE;
				do 
				{											    
					r1=SD_SendCmd(CMD1,0,0X01);
				}while(r1&&retry--);  
			}
			if(retry==0||SD_SendCmd(CMD16,512,0X01)!=0)SD_Type=SD_TYPE_ERR;
		}
	}
	SD_DisSelect();
	//SPIsetClockDivider(SPI_BaudRatePrescaler_2);
	if(SD_Type)
	return 0;
	else if(r1)return r1; 	   
	return 0xaa;
}

u8 SD_GetResponse(u8 Response)
{
	u16 Count=0xFFF;						  
	while ((SPI.transfer(0XFF)!=Response)&&Count)Count--;
	if (Count==0)return MSD_RESPONSE_FAILURE;
	else return MSD_RESPONSE_NO_ERROR;
}

u8 SD_RecvData(u8*buf,u16 len)
{			  	  
    if(SD_GetResponse(0xFE))return 1;
    while(len--)
    {
        *buf=SPI.transfer(0xFF);
        buf++;
    }
    
    SPI.transfer(0xFF);
    SPI.transfer(0xFF);									  					    
    return 0;
}

u8 SD_GetCSD(u8 *csd_data)
{
    u8 r1;	 
    r1=SD_SendCmd(CMD9,0,0x01);
    if(r1==0)
    {
      r1=SD_RecvData(csd_data, 16);
    }
  SD_DisSelect();
  if(r1)
    return 1;
  else 
    return 0;
} 

u32 SD_GetSectorCount(void)
{
    u8 csd[16];
    u32 Capacity;  
    u8 n;
    u16 csize;  					    
    if(SD_GetCSD(csd)!=0) 
    {
      return 0;	    
    }

    if((csd[0]&0xC0)==0x40)	
    {	
		csize = csd[9] + ((u16)csd[8] << 8) + 1;
		Capacity = (u32)csize << 10;	 		   
    }
    else
    {	
		n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
		csize = (csd[8] >> 6) + ((u16)csd[7] << 2) + ((u16)(csd[6] & 3) << 10) + 1;
		Capacity= (u32)csize << (n - 9); 
    }
    return Capacity;
}

u8 SD_ReadDisk(u8* buf,u32 sector,u8 cnt)
{
	u8 r1;
	if(SD_Type!=SD_TYPE_V2HC)sector <<= 9;
	if(cnt==1)
	{
		r1=SD_SendCmd(CMD17,sector,0X01);
		if(r1==0)
		{
			r1=SD_RecvData(buf,512);
		}
	}else
	{
		r1=SD_SendCmd(CMD18,sector,0X01);
		do
		{
			r1=SD_RecvData(buf,512);
			buf+=512;  
		}while(--cnt && r1==0); 	
		SD_SendCmd(CMD12,0,0X01);	
	}   
	SD_DisSelect();
	return r1;//
}

u8 SD_WriteDisk(u8* buf,u32 sector,u8 cnt)
{
	u8 r1;
	if(SD_Type!=SD_TYPE_V2HC)sector *= 512;
	if(cnt==1)
	{
		r1=SD_SendCmd(CMD24,sector,0X01);
		if(r1==0)
		{
			r1=SD_SendBlock(buf,0xFE);
		}
	}
        else
	{
		if(SD_Type!=SD_TYPE_MMC)
		{
			SD_SendCmd(CMD55,0,0X01);	
			SD_SendCmd(CMD23,cnt,0X01);
		}
 		r1=SD_SendCmd(CMD25,sector,0X01);
		if(r1==0)
		{
			do
			{
				r1=SD_SendBlock(buf,0xFC);
				buf+=512;  
			}while(--cnt && r1==0);
			r1=SD_SendBlock(0,0xFD);
		}
	}   
	SD_DisSelect();
	return r1;//
}	 

u8 SD_SendBlock(u8*buf,u8 cmd)
{	
	u16 t;		  	  
	if(SD_WaitReady())return 1;
	SPI.transfer(cmd);
	if(cmd!=0XFD)
	{
	    for(t=0;t<512;t++)
              SPI.transfer(buf[t]);
	      SPI.transfer(0xFF);
	      SPI.transfer(0xFF);
		t=SPI.transfer(0xFF);
		if((t&0x1F)!=0x05)return 2;								  					    
	}						 									  					    
    return 0;
}

void setup() 
{
  u32 sector_size;
  u8 ybuf[512]="DFROBOT!";
  u8 xbuf[512];
  
  Serial1.begin(9600);
 // SPI.begin();
  Serial1.println("Serial1 init is OK!");
   while(SD_Initialize()!=0)
   {
     Serial1.println("SD init Failed");
     delay(1000);
   }
   Serial1.println("SD init OK!");
   sector_size=SD_GetSectorCount()/1024;
  // sector_size=0x3af000;
   Serial1.println(sector_size/4);
   SD_WriteDisk(ybuf,0,1);
   delay(500);
   SD_ReadDisk(xbuf,0,1);
      
   Serial1.println(xbuf[0]);
   Serial1.println(xbuf[1]);
   Serial1.println(xbuf[2]);
   Serial1.println(xbuf[3]);
   Serial1.println(xbuf[4]);
   Serial1.println(xbuf[5]);
}

void loop() {
  // put your main code here, to run repeatedly:

}
