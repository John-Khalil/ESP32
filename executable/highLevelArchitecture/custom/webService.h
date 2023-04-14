#include <stdint.h>
#include <functional>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <type_traits>
// #include "stringFunctions.cpp"
#include "consoleLogger.h"

#include <ESPAsyncWebServer.h>

namespace web{

class service{

    public:

        std::vector<std::function<void(uint8_t*)>>readCallbackList;    
        
        AsyncWebServer *server;
        AsyncWebSocket *ws; 


        #define HTTP_ACK "ACK"
        std::string httpResponse=HTTP_ACK;

        service &onRead(const std::function<void(uint8_t*)>onReadCallback){
            readCallbackList.push_back(onReadCallback);
            return (*this);
        }

        service &httpSetResponse(uint8_t* data){
            httpResponse=(char*)data;
            return (*this);
        }

        service &httpSetResponse(char* data){
            httpResponse=data;
            return (*this);
        }

        service &httpSetResponse(std::string data){
            httpResponse=data;
            return (*this);
        }

        service &write(uint8_t* data){

            return (*this);
        }

        service &write(char* data){

            return (*this);
        }

        service &write(std::string data){

            return (*this);
        }



        service &setup(uint16_t port,uint8_t* path=(uint8_t*)"/"){
            server=new AsyncWebServer(port);
            ws=new AsyncWebSocket((char*)path);

            ws->onEvent([&](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
                if (type == WS_EVT_CONNECT) {

                } else if (type == WS_EVT_DISCONNECT) {

                } else if (type == WS_EVT_DATA) {
                    
                }
            });
            server->addHandler(ws);

            server->on((char*)path, HTTP_GET, [&](AsyncWebServerRequest *request){
                // Send a response back to the client
                request->send(200, "text/plain",httpResponse.c_str());
                httpResponse=HTTP_ACK;
            });

            server->begin();

            return (*this);
        }

        service(uint16_t port,uint8_t* path=(uint8_t*)"/"){
            
        }

        service(uint16_t port,char* path="/"){
            
        }

        service(uint16_t port,std::string path="/"){
            
        }

        ~service(){
            delete server;
            delete ws;
        }


};

}