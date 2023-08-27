#pragma once

#include "consoleLogger.h"
#include "consoleLogger.cpp"
#include "highLevelMemory.cpp"
#include "pointerTool.h"
#include "constDefinitions.h"
#include "eepromBasicConfig.h"
#include "networkUtils.h"
#include "platform.h"
#include "http.h"
#include "webService.h"
#include"fetch.h"

#include "jsonParser.h"
#include "localStorage.h"


#include <stdint.h>
#include <functional>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <type_traits>

#include <PubSubClient.h>

class mqttClient{
  public:


    WiFiClient *tcpConnection;
    PubSubClient *mqttServer;

    std::string userTopic;
    
    std::vector<std::function<void(uint8_t*)>>readCallbackList;

    mqttClient &subscribe(uint8_t *topic){
      mqttServer->subscribe((char*)topic);
      return (*this);
    }

    mqttClient &subscribe(char *topic){
      mqttServer->subscribe(topic);
      return (*this);
    }

    mqttClient &send(uint8_t *message){
      mqttServer->publish(userTopic.c_str(),(char*)message);
      return (*this);
    }

    mqttClient &send(char *message){
      mqttServer->publish(userTopic.c_str(),message);
      return (*this);
    }

    mqttClient &onData(const std::function<void(uint8_t*)>onReadCallback){
      readCallbackList.push_back(onReadCallback);
      return (*this);
    }



    mqttClient &setup(uint8_t *userName,uint8_t *password,uint8_t *topic,uint8_t *serverAddress,uint16_t serverPort=1883){

      tcpConnection=new WiFiClient;
      mqttServer=new PubSubClient(*tcpConnection);

      userTopic=(char*)topic;

      mqttServer->setServer((char*)serverAddress, serverPort);
      mqttServer->setCallback([&](char* topic,uint8_t* payload,uint32_t length){
        std::string clientPayload=(char*)payload;
        for(auto &readCallback:readCallbackList)
          readCallback((uint8_t*)clientPayload.c_str());
        return;
      });

      mqttServer->connect("tcpClient",(char*)userName,(char*)password);
      mqttServer->subscribe((char*)topic);




      return (*this);
    }

    mqttClient(){
      
    }

    ~mqttClient(){
        delete mqttServer;
        delete tcpConnection;
    }
};