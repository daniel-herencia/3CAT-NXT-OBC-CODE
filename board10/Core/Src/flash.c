/*!
 * \file      flash.c
 *
 * \brief     It contains all the functions to read from / write in the flash memory
 *
 *
 * \created on: 15/11/2021
 *
 * \author    Pol Simon
 *
 * \author    David Reiss
 */


#include "flash.h"
#include "stm32f4xx_hal.h"
#include "string.h"
#include "stdio.h"

/**************************************************************************************
 *                                                                                    *
 * Function:  GetSector                                                     		  *
 * --------------------                                                               *
 * defines the SECTORS according to the reference manual					          *
 * STM32F411CE has:																	  *
 *  Sector 0 to Sector 3 each 16KB													  *
 *  Sector 4 as 64KB																  *
 *  Sector 5 to Sector 7 each 128KB													  *
 *                                                                                    *
 *  Address: Specific address of a read/write function                                *
 *                                                                                    *
 *  returns: sector in which the address is contained		                          *
 *                                                                                    *
 **************************************************************************************/

static uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;

  if((Address < 0x08003FFF) && (Address >= 0x08000000))
  {
    sector = FLASH_SECTOR_0;
  }
  else if((Address < 0x08007FFF) && (Address >= 0x08004000))
  {
    sector = FLASH_SECTOR_1;
  }
  else if((Address < 0x0800BFFF) && (Address >= 0x08008000))
  {
    sector = FLASH_SECTOR_2;
  }
  else if((Address < 0x0800FFFF) && (Address >= 0x0800C000))
  {
    sector = FLASH_SECTOR_3;
  }
  else if((Address < 0x0801FFFF) && (Address >= 0x08010000))
  {
    sector = FLASH_SECTOR_4;
  }
  else if((Address < 0x0803FFFF) && (Address >= 0x08020000))
  {
    sector = FLASH_SECTOR_5;
  }
  else if((Address < 0x0805FFFF) && (Address >= 0x08040000))
  {
    sector = FLASH_SECTOR_6;
  }
  else if((Address < 0x0807FFFF) && (Address >= 0x08060000))
  {
    sector = FLASH_SECTOR_7;
  }
/*  else if((Address < 0x0809FFFF) && (Address >= 0x08080000))
  {
    sector = FLASH_SECTOR_8;
  }
  else if((Address < 0x080BFFFF) && (Address >= 0x080A0000))
  {
    sector = FLASH_SECTOR_9;
  }
  else if((Address < 0x080DFFFF) && (Address >= 0x080C0000))
  {
    sector = FLASH_SECTOR_10;
  }
  else if((Address < 0x080FFFFF) && (Address >= 0x080E0000))
  {
    sector = FLASH_SECTOR_11;
  }
  else if((Address < 0x08103FFF) && (Address >= 0x08100000))
  {
    sector = FLASH_SECTOR_12;
  }
  else if((Address < 0x08107FFF) && (Address >= 0x08104000))
  {
    sector = FLASH_SECTOR_13;
  }
  else if((Address < 0x0810BFFF) && (Address >= 0x08108000))
  {
    sector = FLASH_SECTOR_14;
  }
  else if((Address < 0x0810FFFF) && (Address >= 0x0810C000))
  {
    sector = FLASH_SECTOR_15;
  }
  else if((Address < 0x0811FFFF) && (Address >= 0x08110000))
  {
    sector = FLASH_SECTOR_16;
  }
  else if((Address < 0x0813FFFF) && (Address >= 0x08120000))
  {
    sector = FLASH_SECTOR_17;
  }
  else if((Address < 0x0815FFFF) && (Address >= 0x08140000))
  {
    sector = FLASH_SECTOR_18;
  }
  else if((Address < 0x0817FFFF) && (Address >= 0x08160000))
  {
    sector = FLASH_SECTOR_19;
  }
  else if((Address < 0x0819FFFF) && (Address >= 0x08180000))
  {
    sector = FLASH_SECTOR_20;
  }
  else if((Address < 0x081BFFFF) && (Address >= 0x081A0000))
  {
    sector = FLASH_SECTOR_21;
  }
  else if((Address < 0x081DFFFF) && (Address >= 0x081C0000))
  {
    sector = FLASH_SECTOR_22;
  }
  else if (Address < 0x081FFFFF) && (Address >= 0x081E0000)
  {
    sector = FLASH_SECTOR_23;
  }*/
  return sector;
}





void float2Bytes(uint8_t * ftoa_bytes_temp,float float_variable)
{
    union {
      float a;
      uint8_t bytes[4];
    } thing;

    thing.a = float_variable;

    for (uint8_t i = 0; i < 4; i++) {
      ftoa_bytes_temp[i] = thing.bytes[i];
    }

}

float Bytes2float(uint8_t * ftoa_bytes_temp)
{
    union {
      float a;
      uint8_t bytes[4];
    } thing;

    for (uint8_t i = 0; i < 4; i++) {
    	thing.bytes[i] = ftoa_bytes_temp[i];
    }

   float float_variable =  thing.a;
   return float_variable;
}


/**************************************************************************************
 *                                                                                    *
 * Function:  Flash_Write_Data                                                 		  *
 * --------------------                                                               *
 * Writes in the flash memory														  *
 *                                                                                    *
 *  StartSectorAddress: first address to be written		                              *
 *	Data: information to be stored in the FLASH/EEPROM memory						  *
 *	numberofbytes: Data size in Bytes					    						  *
 *															                          *
 *  returns: Nothing or error in case it fails			                              *
 *                                                                                    *
 **************************************************************************************/
uint32_t Flash_Write_Data (uint32_t StartSectorAddress, uint8_t *Data, uint16_t numberofbytes)
{

	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t SECTORError;
	int sofar=0;

	//int numberofwords = (strlen(Data)/4) + ((strlen(Data)%4) != 0);


	 /* Unlock the Flash to enable the flash control register access *************/
	  HAL_FLASH_Unlock();

	  /* Erase the user Flash area */

	  /* Get the number of sector to erase from 1st sector */

	  uint32_t StartSector = GetSector(StartSectorAddress);
	  uint32_t EndSectorAddress = StartSectorAddress + numberofbytes;
	  uint32_t EndSector = GetSector(EndSectorAddress);

	  /* Fill EraseInit structure*/
	  EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
	  EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
	  EraseInitStruct.Sector        = StartSector;
	  EraseInitStruct.NbSectors     = (EndSector - StartSector) + 1;

	  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
	     you have to make sure that these data are rewritten before they are accessed during code
	     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
	     DCRST and ICRST bits in the FLASH_CR register. */
	  if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
	  {
		  return HAL_FLASH_GetError ();

	  }

	  /* Program the user Flash area word by word
	    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

	   while (sofar<numberofbytes)
	   {
	     if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, StartSectorAddress, Data[sofar]) == HAL_OK)
	     {
	    	 StartSectorAddress += 1;  // use StartPageAddress += 2 for half word and 8 for double word
	    	 sofar++;
	     }
	     else
	     {
	       /* Error occurred while writing data in Flash memory*/
	    	 return HAL_FLASH_GetError ();
	     }
	   }

	  /* Lock the Flash to disable the flash control register access (recommended
	     to protect the FLASH memory against possible unwanted operation) *********/
	  HAL_FLASH_Lock();

	   return 0;
}

/**************************************************************************************
 *                                                                                    *
 * Function:  Write_Flash                                                		 	  *
 * --------------------                                                               *
 * It's the function that must be called when writing in the Flash memory.			  *
 * Depending on the address, it writes 1 time or 3 times (Redundancy)				  *
 *                                                                                    *
 *  StartSectorAddress: first address to be written		                              *
 *	Data: information to be stored in the FLASH/EEPROM memory						  *
 *	numberofbytes: Data size in Bytes					    						  *
 *															                          *
 *  returns: Nothing									                              *
 *                                                                                    *
 **************************************************************************************/
void Write_Flash(uint32_t StartSectorAddress, uint8_t *Data, uint16_t numberofbytes) {
	if (StartSectorAddress >= 0x08000000 && StartSectorAddress <= 0x0800BFFF) {
		Flash_Write_Data(StartSectorAddress, Data, numberofbytes);
		Flash_Write_Data(StartSectorAddress + 0x4000, Data, numberofbytes);
		Flash_Write_Data(StartSectorAddress + 0x8000, Data, numberofbytes);
	}
	else {
		Flash_Write_Data(StartSectorAddress, Data, numberofbytes);
	}
}

/**************************************************************************************
 *                                                                                    *
 * Function:  Flash_Read_Data                                                 		  *
 * --------------------                                                               *
 * Writes in the flash memory														  *
 *                                                                                    *
 *  StartSectorAddress: first address to be read		                              *
 *	RxBuf: Where the data read from memory will be stored							  *
 *	numberofbytes: Reading data size in Bytes					    				  *
 *															                          *
 *  returns: Nothing									                              *
 *                                                                                    *
 **************************************************************************************/
void Flash_Read_Data (uint32_t StartSectorAddress, uint8_t *RxBuf, uint16_t numberofbytes)
{
	while (1)
	{

		*RxBuf = *(__IO uint8_t *)StartSectorAddress;
//		if (*RxBuf == 0xffffffff)
//		{
//			*RxBuf = '\0';
//			break;
//		}
		StartSectorAddress += 1;
		RxBuf++;
		numberofbytes--;
		if (numberofbytes == 0x00) break;
	}
}


/**************************************************************************************
 *                                                                                    *
 * Function:  Check_Redundancy                                                 		  *
 * --------------------                                                               *
 * Reads the data from the 3 addresses where it is stored and chooses the value		  *
 * that coincides at least in 2 of the 3 addresses (in case one gets corrupted)		  *
 * All the addresses of variables with Redundancy follow the same pattern: each		  *
 * address is separated 0x4000 positions in memory									  *
 *                                                                                    *
 *  Address: first address to be read		                              			  *
 *	RxBuf1: Buffer to store the lecture from the first address						  *
 *	RxBuf2: Buffer to store the lecture from the second address						  *
 *	RxBuf3: Buffer to store the lecture from the third address						  *
 *	RxDef: Buffer to store the lecture that coincides at least 2 times				  *
 *															                          *
 *  returns: Nothing									                              *
 *                                                                                    *
 **************************************************************************************/
void Check_Redundancy(uint32_t Address, uint8_t *RxBuf1, uint8_t *RxBuf2, uint8_t *RxBuf3, uint8_t *RxDef){
	Flash_Read_Data(Address, RxBuf1, sizeof(RxBuf1));
	Flash_Read_Data(Address + 0x4000, RxBuf2, sizeof(RxBuf2));
	Flash_Read_Data(Address + 0x8000, RxBuf3, sizeof(RxBuf3));

	if(*RxBuf1 == *RxBuf2 || *RxBuf1 == *RxBuf3) {
		Flash_Read_Data(Address, RxDef, sizeof(RxDef));
	}
	else if(*RxBuf2 == *RxBuf3) {
		Flash_Read_Data(Address + 0x4000, RxDef, sizeof(RxDef));
	}
	else {
		*RxDef = *RxBuf1; /*PREGUNTAR QUÈ FER QUAN NO COINCIDEIX CAP LECTURA (POC PROBABLE)*/
	}
}


/**************************************************************************************
 *                                                                                    *
 * Function:  Read_Flash	                                                 		  *
 * --------------------                                                               *
 * It's the function that must be called when reading from the Flash memory.		  *
 * Depending on the address, it reads from 1 or 3 addresses (Redundancy)			  *
 *                                                                                    *
 *  StartSectorAddress: starting address to read		                              *
 *	RxBuf: Where the data read from memory will be stored							  *
 *	numberofbytes: Reading data size in Bytes					    				  *
 *															                          *
 *  returns: Nothing									                              *
 *                                                                                    *
 **************************************************************************************/
void Read_Flash(uint32_t StartSectorAddress, uint8_t *RxBuf, uint16_t numberofbytes) {
	if (StartSectorAddress >= 0x08000000 && StartSectorAddress <= 0x0800BFFF) {
		uint8_t lect1[numberofbytes], lect2[numberofbytes], lect3[numberofbytes];
		Check_Redundancy(StartSectorAddress, lect1, lect2, lect3, RxBuf);
	}
	else {
		Flash_Read_Data(StartSectorAddress, RxBuf, numberofbytes);
	}
}
