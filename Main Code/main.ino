    // Bibliotecas

        //Bibliotecas Sensores
            #include <Arduino.h>    
            #include "DHT20.h"
            #include <Digital_Light_TSL2561.h>
            #include <Dps310.h>
            #include "TinyGPS++.h"

        //Bibliotecas Sistema
            #include "Moduloalimentacao.h" // Uncomment if you have this file
            #include "Sensors.h" // This header should contain function prototypes and extern declarations
            #include "Lora.h" // Uncomment if you have this file
            #include "Variables.h" 

    // Definição das variáveis globais 
        TinyGPSPlus gps;
        float temperatura = 0.0;
        float humidade = 0.0;
        float pressao_atmo = 0.0;
        float chuva = 0; 
        float qualidade_ar = 0;
        float luz = 0;
        float uv = 0.0;
        float lat = 0.0;
        float longi = 0.0;
        float batteryLevel;
        float Painel;
        String mask = "0000000"; // Use Arduino String consistently

    //Variaveis Auxiliares
        unsigned long startMillis;
        unsigned long lastSendTime = 0;
        unsigned long lastSendTime1 = 0;
        const unsigned long sendInterval = 180000 * 5; // timer de envio de dados
        const unsigned long sendInterval1 = 10000; //timer de leitura de dados
    
    bool start = true;

    
    void setup() {

        unsigned long starttime = millis();
        // delay(1000);

        Serial.println("======================================================================");
        Serial.println("SETUP=START===========================================================");


            startMillis = millis();
            Serial.begin(9600);
            Serial2.begin(9600);

            initializeSensors();
            initializeLoRa();

        Serial.println("SETUP=END=============================================================");
        Serial.println("======================================================================");

            Serial.println();
                //mask = readSensors();
                //printToLog(); // leituras nao funcionam no Setup
            Serial.println();

        Serial.println("======================================================================");
        Serial.println("LOOP==================================================================");
    }

                      
    void loop() {
        unsigned long currentTime = millis();
        

        //if (currentTime - lastSendTime >= sendInterval1 || lastSendTime == 0) {

            Serial.print("===================================  ");
                printElapsedTime();
                mask = readSensors();

                if(start) {
                sendData();
                start=false;
                }
                
                if (currentTime - lastSendTime1 >= sendInterval || lastSendTime1 == 0) {  
                   sendData();
                   lastSendTime1 = currentTime;
                } else { 
                    //Serial.print("Sensor Error Mask: ");
                    //Serial.println(mask); 
                }                
                lastSendTime = currentTime;
                printSensorValues();
                Serial.println();
                Serial.println("======================================================================");
       // }
    }