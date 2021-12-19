/*
 * telecommands.c
 *
 *  Created on: Dec 11, 2021
 *      Author: psimo
 */

#include "telecommands.h"


/**************************************************************************************
 *                                                                                    *
 * Function:  process_telecommand                                                     *
 * --------------------                                                               *
 * processes the information contained in the packet depending on the telecommand     *
 * received																	          *
 *                                                                                    *
 *  header: number of telecommand			                                          *
 *  info: information contained in the received packet								  *
 *                                                                                    *
 *  returns: nothing									                              *
 *                                                                                    *
 **************************************************************************************/
void process_telecommand(uint8_t header, uint8_t info) {
	switch(header) {
	case RESET2:
		/*Segons el drive s'ha de fer el reset si val 1 el bit, així que potser
		 * s'hauria de posar un if*/
		HAL_NVIC_SystemReset();
		break;
	case NOMINAL:
		Write_Flash(NOMINAL_ADDR, &info, 1);
		break;
	case LOW:
		Write_Flash(LOW_ADDR, &info, 1);
		break;
	case CRITICAL:
		Write_Flash(CRITICAL_ADDR, &info, 1);
		break;
	case EXIT_LOW_POWER:
		Write_Flash(EXIT_LOW_POWER_FLAG_ADDR, &info, 1);
		break;
	case SET_TIME:

		break;
	case SET_CONSTANT_KP:
		Write_Flash(KP_ADDR, &info, 1);
		break;
	case TLE:
		/*Aquí si es reben els TLEs en diferents paquets doncs s'han d'anar
		 * escrivint poc a poc o anar emmagatzemant la info i quan es tinguin
		 * tots els bytes junts s'emmagatzemen (suposo que els paquets arriben
		 * seguits)*/
		Write_Flash(TLE_ADDR, &info, 138);
		break;
	case SET_GYRO_RES:
		/*4 possibles estats, rebrem 00/01/10/11*/
		Write_Flash(GYRO_RES_ADDR, &info, 1);
		break;
	case SENDDATA:

		break;
	case SENDTELEMETRY:

		break;
	case STOPSENDINGDATA:

		break;
	case ACKDATA:

		break;
	case SET_SF: ; //semicolon added in order to be able to declare SF here
		uint8_t SF;
		if (info == 0) SF = 7;
		else if (info == 1) {uint8_t SF = 8;}
		else if (info == 2) {uint8_t SF = 9;}
		else if (info == 3) {uint8_t SF = 10;}
		else if (info == 4) {uint8_t SF = 11;}
		else if (info == 5) {uint8_t SF = 12;}
		Write_Flash(SF_ADDR, &SF, 1);
		break;
	case SET_CRC:
		/*4 cases (4/5, 4/6, 4/7,1/2), so we will receive and store 0, 1, 2 or 3*/
		Write_Flash(CRC_ADDR, &info, 1);
		break;
	case SEND_CALIBRATION:

		break;
	case TAKEPHOTO:
		/*GUARDAR TEMPS FOTO?*/
		Write_Flash(PAYLOAD_STATE_ADDR, TRUE, 1);
		Write_Flash(PL_TIME_ADDR, &info, 4);
		break;
	case SET_PHOTO_RESOL:
		Write_Flash(PHOTO_RESOL_ADDR, &info, 1);
		break;
	case PHOTO_COMPRESSION:
		Write_Flash(PHOTO_COMPRESSION_ADDR, &info, 1);
		break;
	case TAKERF:
		Write_Flash(PAYLOAD_STATE_ADDR, TRUE, 1);
		Write_Flash(PL_TIME_ADDR, &info, 4);
		break;
	case F_MIN:
		Write_Flash(F_MIN_ADDR, &info, 2);
		break;
	case F_MAX:
		Write_Flash(F_MAX_ADDR, &info, 2);
		break;
	case DELTA_F:
		Write_Flash(DELTA_F_ADDR, &info, 2);
		break;
	case INTEGRATION_TIME:
		Write_Flash(INTEGRATION_TIME_ADDR, &info, 1);
		break;
	case SEND_CONFIG: ; //semicolon added in order to be able to declare SF here
		uint8_t config[CONFIG_SIZE];
		Read_Flash(CONFIG_ADDR, &config, CONFIG_SIZE);
		//Send()
		break;
	}
}
