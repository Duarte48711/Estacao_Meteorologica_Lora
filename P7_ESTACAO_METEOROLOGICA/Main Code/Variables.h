#ifndef VARIABLES_H
#define VARIABLES_H

  // Pin Definitions
    #define AIR_QUALITY_PIN 8
    #define UV_SENSOR_PIN A2
    #define RAIN_SENSOR_PIN A3

    #define BatteryVoltagePin A4
    #define ChargingStatusPin A5

    #include <Arduino.h>   

  //declarations of classes
    class DHT20;
    class TinyGPSPlus;

  // Global variables declarations
    extern DHT20* dht;
    extern float temperatura;
    extern float humidade;
    extern float pressao_atmo;
    extern float chuva;
    extern float qualidade_ar;
    extern float uv;
    extern float luz;
    extern float lat;
    extern float longi;
    extern float batteryLevel;
    extern float Painel;
    extern String mask;
    extern TinyGPSPlus gps;
    extern unsigned long startMillis;
    extern unsigned long starttime;
    extern char buffer[256];

#endif // VARIABLES_H