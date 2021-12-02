/*
 * payload_camera.c
 *
 *  Created on: Nov 25, 2021
 *      Author: USUARIO
 */

#include <payload_camera.h>
#include <flash.h>

//VARIABLES
uint8_t dataBuffer[201], bufferLength;
uint32_t frameLength;
uint16_t framePointer;

uint8_t commInit[2] = {0x56, 0x00};
uint8_t commCapture = 0x36;
uint32_t bSize = 128;

//NO ACABADA
uint8_t readResponse(UART_HandleTypeDef *huart, uint8_t expLength, uint8_t attempts){
  int i = 0;
  bufferLength = 0;

//  while (attempts != i && bufferLength != expLength)//si attemps == i o si bufferLenght == expLength sale del while
//  {
////    if ()//mirar si tenemos algo que leer
////    { // Is there any data?
////      delay(1);
////      i++;
////      continue;
////    }
//    i = 0;
//    bufferLength++;
//    //dataBuffer[bufferLength++] = swsInstance.read(); // And we fill it with data
//	//for(int i = 0; i<)
//    HAL_UART_Receive(huart, dataBuffer, expLength, 100);
//  }
  HAL_UART_Receive(huart, dataBuffer, expLength, 100);

  return expLength;
}

bool runCommand(UART_HandleTypeDef *huart, uint8_t command, uint8_t *hexData, uint8_t dataArrayLength, uint8_t expLength, bool doFlush)
{ // Flushes the buffer, sends the command and hexData then checks and verifies it

// Flush the reciever buffer
  if (doFlush)
  {
    readResponse(huart, 100, 10);
  }

  // Send the data
  HAL_UART_Transmit(huart, commInit, 2, HAL_MAX_DELAY);
  HAL_UART_Transmit(huart, &command, 1, HAL_MAX_DELAY);

  for (int i = 0; i < dataArrayLength; i++)
  {
	  HAL_UART_Transmit(huart, &hexData[i], 1, HAL_MAX_DELAY);
  }

  // Check the data
  if (readResponse(huart, expLength, 100) != expLength)
  {
    return false;
  }

  // Data should always be 76, 00, command, 00
  //CREO QUE PUEDO HACER UN RETURNE DIRECTO MUCHO MAS BONITO
  if (dataBuffer[0] != 0x76 ||
      dataBuffer[1] != 0x0 ||
      dataBuffer[2] != command ||
      dataBuffer[3] != 0x0)
  {
    return false;
  }

  return true;
}

void getFrameLength(UART_HandleTypeDef *huart)
{ // ~ Get frame length
  uint8_t hexData[] = {0x01, 0x00};
  if (!runCommand(huart, 0x34, hexData, sizeof(hexData), 9, true))
  {
    while (1);
  }
  frameLength = dataBuffer[5]; //Recreating split hex numbers from 4 bytes
  frameLength <<= 8;
  frameLength |= dataBuffer[6];
  frameLength <<= 8;
  frameLength |= dataBuffer[7];
  frameLength <<= 8;
  frameLength |= dataBuffer[8];
}

void retrieveImage(UART_HandleTypeDef *huart)
{ // * Retrieve photo data
	uint8_t dataVect[frameLength];
  //BORRAR
	//MEMSET
	for(int i = 0; i < frameLength; i++){
	  dataVect[i] = 0;
	}
  //BORRAR

	while (frameLength > 0)
	{
		HAL_Delay(100);

		int toRead = min(bSize, frameLength); // Bytes read each loop
		uint8_t hexData[] = {0x0C, 0x0, 0x0A, 0x0, 0x0,
							 framePointer >> 8, framePointer & 0xFF, 0x0, 0x0,
							 0x0, toRead, 0x0, 0x0A
							};

		if (!runCommand(huart, 0x32, hexData, sizeof(hexData), 5, false))
		{
			HAL_Delay(1);
		}
		if (readResponse(huart, toRead + 5, 0xff) == 0) // +5 for verification header
		{
			HAL_Delay(1);
		}

		//AIXO HO HAURIEM DE MODIFICAT AMB OBC
		for(int i = 0; i < toRead; i++){
		  dataVect[framePointer+i] = dataBuffer[i];
		}

		framePointer += toRead;
		frameLength -= toRead;
	}

	Flash_Write_Data(PHOTO_ADDR, dataVect, sizeof(dataVect));
}

int min(int x, int y)
{
  return (x < y) ? x : y;
}
