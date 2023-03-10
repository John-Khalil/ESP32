// #ifndef HTTP_H
// #define HTTP_H

// #include"consoleLogger.h"

// #ifdef ESP32
//     #include <WiFi.h>
//     #include <ESP32HTTPClient.h>
// #endif

// #ifdef ESP8266
//     #include <ESP8266WiFi.h>
//     #include <ESP8266HTTPClient.h>
// #endif

// namespace HTTP{
//     HTTPClient http;

//     unsigned char *fetch(unsigned char *url){
//         http.begin((char*)url);
//         http.GET();
//         unsigned char *payload=(unsigned char*)http.getString().c_str();
//         http.end();
//         return payload;
//     }

//     unsigned char *fetch(char *url){
//         http.begin((char*)url);
//         http.GET();
//         unsigned char *payload=(unsigned char*)http.getString().c_str();
//         // console.log("from inside -> ",payload);
//         http.end();
//         return payload;
//     }
// }


// #endif 
