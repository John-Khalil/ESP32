

#include <bits/stdc++.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>


#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

#include <ArduinoJson.h>

#define LOCAL_STARTUP(startupCode) {static std::function<void(void)>localStartup=[&](void){startupCode;localStartup=[](){};};localStartup();}



uint32_t heartRate(void){
    static MAX30105 particleSensor;

    #define RATE_AVG 4

    LOCAL_STARTUP({
        while(!particleSensor.begin(Wire, I2C_SPEED_FAST));

        particleSensor.setup();
        particleSensor.setPulseAmplitudeRed(0x1A);
        particleSensor.setPulseAmplitudeGreen(0);
    });

    uint16_t loopCounter=RATE_AVG;
    uint32_t beatAvg=0;
    uint32_t lastBeat=millis();
    while(loopCounter--){

        for(;;){
            uint32_t irValue = particleSensor.getIR();
            if(irValue<5000){
                return -1;
            }
            if(checkForBeat(irValue))
                break;
        };
        beatAvg +=  60 / ((millis() - lastBeat) / 1000.0);
        
        lastBeat = millis();

    }

    return (beatAvg/RATE_AVG);
}

std::string getHeartRate(void){
    DynamicJsonDocument bleObject(200);
    bleObject["heartRate"]=heartRate();
    std::string jsonString;
    serializeJson(bleObject, jsonString);
    return jsonString;
}

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
BLEDescriptor *pDescr;
BLE2902 *pBLE2902;

bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

void setup() {
  Serial.begin(115200);


  // Create the BLE Device
  BLEDevice::init("ESP32");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_NOTIFY|BLECharacteristic::PROPERTY_READ|BLECharacteristic::PROPERTY_WRITE
                    );                   

  // Create a BLE Descriptor
  
  pDescr = new BLEDescriptor((uint16_t)0x2901);
  pDescr->setValue("A very interesting variable");
  pCharacteristic->addDescriptor(pDescr);
  
  pBLE2902 = new BLE2902();
  pBLE2902->setNotifications(true);
  pCharacteristic->addDescriptor(pBLE2902);

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
    // notify changed value
    if (deviceConnected) {
        pCharacteristic->setValue(getHeartRate());
        pCharacteristic->notify();
        value++;
        delay(1000);
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}

// #include <Wire.h>
// #include "MAX30105.h"
// #include "heartRate.h"

// MAX30105 particleSensor;

// const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
// byte rates[RATE_SIZE]; //Array of heart rates
// byte rateSpot = 0;
// long lastBeat = 0; //Time at which the last beat occurred

// float beatsPerMinute;
// int beatAvg;

// void setup() {
//   Serial.begin(115200);
//   Serial.println("Initializing...");

  

//   // Initialize sensor
//   while (!particleSensor.begin(Wire, I2C_SPEED_STANDARD)) {
//     Serial.println("MAX30102 was not found. Please check wiring/power. ");
//     // while (1);
//     delay(500) ;
//   }
//   Serial.println("Place your index finger on the sensor with steady pressure.");

//   particleSensor.setup(); //Configure sensor with default settings
//   particleSensor.setPulseAmplitudeRed(0x1A); //Turn Red LED to low to indicate sensor is running
//   particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
// }

// void loop() {
//   long irValue = particleSensor.getIR();

//   if (checkForBeat(irValue) == true) {
//     //We sensed a beat!
//     long delta = millis() - lastBeat;
//     lastBeat = millis();

//     beatsPerMinute = 60 / (delta / 1000.0);

//     if (beatsPerMinute < 255 && beatsPerMinute > 20) {
//       rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
//       rateSpot %= RATE_SIZE; //Wrap variable

//       //Take average of readings
//       beatAvg = 0;
//       for (byte x = 0 ; x < RATE_SIZE ; x++)
//         beatAvg += rates[x];
//       beatAvg /= RATE_SIZE;
//     }
//   }

//   Serial.print("IR=");
//   Serial.print(irValue);
//   Serial.print(", BPM=");
//   Serial.print(beatsPerMinute);
//   Serial.print(", Avg BPM=");
//   Serial.print(beatAvg);

//   if (irValue < 50000)
//     Serial.print(" No finger?");

//   Serial.println();
// }

