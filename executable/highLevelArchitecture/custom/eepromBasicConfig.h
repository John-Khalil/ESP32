#ifndef EEPROM_BASIC_CONFIG_H
#define EEPROM_BASIC_CONFIG_H

#include "pointerTool.h"
#include <EEPROM.h>




namespace EEPROM_UTILS {

	#define MAX_EEPROM_SIZE 512U
	#define SSID_MAX_LENGTH 32
	#define WIFI_PASSWORD_MAX_LENGTH 63
	#define WIFI_CONFIG 0

	#define LINKER_CONFIG WIFI_CONFIG+SSID_MAX_LENGTH+WIFI_PASSWORD_MAX_LENGTH
	#define LINKER_CONFIG_LENGTH 100

	#define EEPROM_BUFFER_SIZE 200
	unsigned char EEPROM_BUFFER[EEPROM_BUFFER_SIZE]="";



	unsigned char ssidSave(unsigned char *networkSSID){
		unsigned short startAddress=WIFI_CONFIG;
		while(startAddress<(WIFI_CONFIG+SSID_MAX_LENGTH)){
			EEPROM.write(startAddress&((MAX_EEPROM_SIZE-1)),*networkSSID);
			startAddress++;
			networkSSID+=(*networkSSID!=0);
		}
		return (unsigned char)EEPROM.commit();
	}


	unsigned char wifiPasswordSave(unsigned char *networkPassword){
		unsigned short startAddress=WIFI_CONFIG+SSID_MAX_LENGTH;
		while(startAddress<(WIFI_CONFIG+SSID_MAX_LENGTH+WIFI_PASSWORD_MAX_LENGTH)){
			EEPROM.write(startAddress&((MAX_EEPROM_SIZE-1)),*networkPassword);
			startAddress++;
			networkPassword+=(*networkPassword!=0);
		}
		return (unsigned char)EEPROM.commit();
	}



	unsigned char NETWORK_SSID[SSID_MAX_LENGTH+1]="";
	unsigned char NETWORK_PASSWORD[WIFI_PASSWORD_MAX_LENGTH+1]="";

	unsigned char *userSSID(void){
		unsigned char *getSSID=NETWORK_SSID;
		unsigned short startAddress=WIFI_CONFIG;
		while(startAddress<(WIFI_CONFIG+SSID_MAX_LENGTH)){
			*getSSID=EEPROM.read(startAddress);
			getSSID++;
			startAddress++;
		}
		return NETWORK_SSID;
	}


	unsigned char *userPassword(void){
		unsigned char *getPassword=NETWORK_PASSWORD;
		unsigned short startAddress=WIFI_CONFIG+SSID_MAX_LENGTH;
		while(startAddress<(WIFI_CONFIG+SSID_MAX_LENGTH+WIFI_PASSWORD_MAX_LENGTH)){
			*getPassword=EEPROM.read(startAddress);
			getPassword++;
			startAddress++;
		}
		return NETWORK_PASSWORD;
	}

	unsigned char linkerSave(unsigned char *linkerJson){
		unsigned short startAddress=LINKER_CONFIG;
		while(startAddress<(LINKER_CONFIG+LINKER_CONFIG_LENGTH)){
			EEPROM.write(startAddress&((MAX_EEPROM_SIZE-1)),*linkerJson);
			startAddress++;
			linkerJson+=(*linkerJson!=0);
		}
		return (unsigned char)EEPROM.commit();
	}

	unsigned char *linkerData(void){
		CLR_LENGTH=EEPROM_BUFFER_SIZE;
		CLR(EEPROM_BUFFER);
		unsigned char *getLinkerData=EEPROM_BUFFER;
		unsigned short startAddress=LINKER_CONFIG;
		while(startAddress<(LINKER_CONFIG+LINKER_CONFIG_LENGTH)){
			*getLinkerData=EEPROM.read(startAddress);
			getLinkerData++;
			startAddress++;
		}
		return EEPROM_BUFFER;
	}

	unsigned char FIRST_RUN=0;
	void eepromInit(void){
		EEPROM.begin(MAX_EEPROM_SIZE);
		if(EEPROM.read((MAX_EEPROM_SIZE-1))==0xFF){
			for(unsigned short clearCounter=0;clearCounter<MAX_EEPROM_SIZE;clearCounter++){
				EEPROM.write(clearCounter,0);
			}
			EEPROM.commit();
			FIRST_RUN=1;
			wifiPasswordSave((unsigned char*)"12345678");
		}
		return;
	}

}


#endif
