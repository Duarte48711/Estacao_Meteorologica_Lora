    // Include Libraries
    #include <Arduino.h>
    #include "Variables.h" // Assuming this declares extern variables including mask
    #include "DHT20.h"
    #include <Digital_Light_TSL2561.h>
    #include <Dps310.h>
    #include "TinyGPS++.h"
    #include "Moduloalimentacao.h" // Uncomment if you have this file
    #include "Sensors.h" // This header should contain function prototypes and extern declarations
    #include "Lora.h" // Uncomment if you have this file


    // Definição das variáveis globais 
    TinyGPSPlus gps;
    float temperatura = 0.0;
    float humidade = 0.0;
    float pressao_atmo = 0.0;
    float chuva = 0; // Assuming 0, 1, or 2
    float qualidade_ar = 0;
    float luz = 0;
    float uv = 0.0;
    float lat = 0.0;
    float longi = 0.0;
    String mask = "0000000"; // Use Arduino String consistently
    unsigned long startMillis;
    unsigned long lastSendTime = 0;
    unsigned long lastSendTime1 = 0;
    const unsigned long sendInterval = 180000;
    const unsigned long sendInterval1 = 10000;

    extern void initializeLoRa();
    extern void sendData();
    extern void checkBatteryStatus();
    extern void checkUSBChargingStatus();
    extern void displayInfo();

    /*const char *gpsStream =
                            "$GPRMC,045103.000,A,3014.1984,N,09749.2872,W,0.67,161.46,030913,,,A*7C\r\n"
                            "$GPGGA,045104.000,3014.1985,N,09749.2873,W,1,09,1.2,211.6,M,-22.5,M,,0000*62\r\n"
                            "$GPRMC,045200.000,A,3014.3820,N,09748.9514,W,36.88,65.02,030913,,,A*77\r\n"
                            "$GPGGA,045201.000,3014.3864,N,09748.9411,W,1,10,1.2,200.8,M,-22.5,M,,0000*6C\r\n"
                            "$GPRMC,045251.000,A,3014.4275,N,09749.0626,W,0.51,217.94,030913,,,A*7D\r\n"
                            "$GPGGA,045252.000,3014.4273,N,09749.0628,W,1,09,1.3,206.9,M,-22.5,M,,0000*6F\r\n";
                            */
    void setup() {
        unsigned long starttime = millis();
        delay(1000);
        Serial.println("======================================================================");
        Serial.println("SETUP=START===========================================================");


            startMillis = millis();
            Serial.begin(9600);
            Serial2.begin(9600);
           // while (*gpsStream)
                //if (gps.encode(*gpsStream++))

            initializeSensors();
            initializeLoRa();

        Serial.println("SETUP=END=============================================================");
        Serial.println("======================================================================");

            Serial.println();
            printToLog();
            Serial.println();

        Serial.println("======================================================================");
        Serial.println("LOOP==================================================================");
    }

    void loop() {
        unsigned long currentTime = millis();
        

        if (currentTime - lastSendTime >= sendInterval1 || lastSendTime == 0) {

            //Serial.print("===================================  ");
             //   printElapsedTime();
                mask = readSensors();

                if (currentTime - lastSendTime1 >= sendInterval || lastSendTime == 0) {   //mask == "0000000") {
                   //sendData();
                   lastSendTime1 = currentTime;
                } else { 
                    Serial.print("Sensor Error Mask: ");
                    Serial.println(mask); 
                }
                printToLog();
                //printSensorValues();
                lastSendTime = currentTime;
                
            //Serial.println();
            //Serial.println("======================================================================");
        }
    }