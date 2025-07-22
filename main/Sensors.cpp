    //Bibliotecas
        #include "Sensors.h"
        #include "gps.h"

    //Variaveis globais dos sensores
        Dps310 Dps310PressureSensor;
        DHT20 DHT;

    //Variaveis sensor QA
        float air_quality_reading=0;
        int dustSensorPin = 8;
        unsigned long pulseDuration;
        unsigned long startTime;
        unsigned long sampleTimeMs = 30000; // Sample time in milliseconds
        unsigned long lowPulseOccupancy = 0;
        float pulseRatio = 0;
        float dustConcentration = 0;
        int count =0;
        float concentration = 0;
        int measure = 0;  

    //Variaveis UV         
        int sensorValue;
        int  sum=0;        

    void initializeSensors() {
        Wire.begin();
        pinMode(38, OUTPUT);
        pinMode(AIR_QUALITY_PIN, INPUT);
        digitalWrite(38, HIGH);        
        DHT.begin();
        TSL2561.init();
        Dps310PressureSensor.begin(Wire);
    }              

    float readAirQuality(unsigned long sampletime_ms) {

            unsigned long duration;
            unsigned long lowPulseOccupancy = 0;
            unsigned long startTime = millis();
            while ((millis() - startTime) < sampleTimeMs) {
                duration = pulseIn(AIR_QUALITY_PIN, LOW);
                lowPulseOccupancy += duration;
            }            
            float ratio = lowPulseOccupancy / (sampleTimeMs * 1000.0); 
            float concentration = 1.1 * pow(ratio, 3) - 3.8 * pow(ratio, 2) + 520 * ratio + 0.62;
            return concentration * 0.04537;//3531.47*2.45*pow(10,-5);
              
    }

    void readUV(){
        int sensorValue;
        long sum = 0;
        for (int i = 0; i < 1024; i++)  // accumulate readings for 1024 times
        {
            sensorValue = analogRead(A2);
            sum = sensorValue + sum;
            delay(2);
        }
        long meanVal = sum / 1024;  // get mean value  
        //Serial.print("meanValue:");  //Serial.println(meanVal); // debug
        
        float sensorVout = meanVal * (3.300/ 1023.0) * 1000;
       // float uvIntensity = sensorVout * 307.0;  // Resultado em mW/m²
        // Serial.println(sensorVout); //debug
         /*
        float uvIndex = ((meanVal * 1000 / 4.3 - 83) / 21);
        float uvReal = (uvIndex - 29.27) * (11 - 2) / (500 - 29.27) + 2;
        */
       float uvReal = 0;

        if (sensorVout < 50) {
            uvReal = 0;
        }
        else if (sensorVout >= 50 && sensorVout < 227) {
            uvReal = 1;
        }
        else if (sensorVout >= 227 && sensorVout < 318) {
            uvReal = 2;
        }
        else if (sensorVout >= 318 && sensorVout < 408) {
            uvReal = 3;
        }
        else if (sensorVout >= 408 && sensorVout < 503) {
            uvReal = 4;
        }
        else if (sensorVout >= 503 && sensorVout < 606) {
            uvReal = 5;
        }
        else if (sensorVout >= 606 && sensorVout < 696) {
            uvReal = 6;
        }
        else if (sensorVout >= 696 && sensorVout < 795) {
            uvReal = 7;
        }
        else if (sensorVout >= 795 && sensorVout < 881) {
            uvReal = 8;
        }
        else if (sensorVout >= 881 && sensorVout < 976) {
            uvReal = 9;
        }
        else if (sensorVout >= 976 && sensorVout < 1079) {
            uvReal = 10;
        }
        else if (sensorVout >= 1079) {
            uvReal = 11;
        }

        uv = uvReal;
        

    }
    float readIQA(int dustSensorPin, unsigned long sampleTimeMs) {
  unsigned long startTime = millis();
  unsigned long lowPulseOccupancy = 0;

  // Acumula toda a duração dos pulsos LOW durante sampleTimeMs
  while (millis() - startTime < sampleTimeMs) {
    lowPulseOccupancy += pulseIn(dustSensorPin, LOW);
  }

  // Converte ocupação de pulso em razão (pcs/0.01cf)
  float ratio = lowPulseOccupancy / (sampleTimeMs * 10.0);

  // Fórmula do fabricante para pcs/0.01cf
  float concentrationPcs = 1.1 * pow(ratio, 3)
                         - 3.8 * pow(ratio, 2)
                         + 520  * ratio
                         + 0.62;

  // Converte para µg/m³ (~0.0477 µg/m³ por unidade de pcs/0.01cf)
  return concentrationPcs * 0.0477;
}
    String readSensors() { 
        
       // mask = "00000000"; 
          mask = "1111111";
        // DHT20 (Temperature and Humidity)
            if (millis() - DHT.lastRead() >= 1000) {
                int status = DHT.read();
                if (status == DHT20_OK) {

                    temperatura = DHT.getTemperature();

                    if(fabs(temperatura) > 65 || temperatura < -20  ){
                        mask[0] = '1';                         
                    }
                    humidade = DHT.getHumidity();
                    if( fabs(humidade) < 0.001 || fabs(humidade) > 100.0 ){
                        mask[1] = '1';
                    } 

                }else {                
                    mask[0] = '1'; 
                    mask[1] = '1'; 
                }
                //Serial.println("Debug Print:");
                //Serial.print("tmp e hum");
            }
        
        // Atmospheric Pressure
            bool pressure_ok = Dps310PressureSensor.measurePressureOnce(pressao_atmo, 7);
                if (pressure_ok) {
                    pressao_atmo = pressao_atmo;
                    if(fabs(pressao_atmo) < 0.001 || fabs(pressao_atmo) > 1300 || fabs(pressao_atmo) < 800 )  mask[2] = '1'; 
                } 
                //Serial.print(" , pressao atmo");      //debug


        // Rain Sensor 
            chuva= analogRead(RAIN_SENSOR_PIN);
            chuva=(chuva / 1023.0) * 3.3;
            if (chuva == -1.0 || chuva == 0.0 ||  chuva > 3.3 ) {
                mask[3] = '1'; 
            }
            //Serial.print(" , chuva");     //debug

        // Air Quality 
            //air_quality_reading = readAirQuality(sampleTimeMs); 
            air_quality_reading = readIQA( AIR_QUALITY_PIN , sampleTimeMs);
            if (air_quality_reading != -1.0 && air_quality_reading != 0.0 ) {
                qualidade_ar = air_quality_reading;
            } else {
                mask[4] = '1'; 
            }
            //Serial.print(" , QA");    //debug
    
        // Light Sensor 
            luz = TSL2561.readVisibleLux();        
            if(luz ==-1) luz=40000;
            if(fabs(luz) < 0.001 ){
                 mask[5] = '1'; 
            }
            //Serial.print(" , luz"); //debug

        // UV Sensor            
            
            readUV();

            if(fabs(uv) < 1 ){
                 mask[6] = '1'; 
            }
            //Serial.print(" , UV");        //debug

        // GPS 
            updateGPS();
            if(!isnan(lat) || !isnan(longi) || isinf(lat) || isinf(longi)){}
            else {
                mask[7] = '1';
            } 
            readBattery();
            //Serial.println(" , GPS");     //debug

        return mask; 
    }

    void printSensorValues() {

        Serial.println("\t Valores dos Sensores:");
        Serial.print("\t\t Temperatura (C): \t\t");
        Serial.println(temperatura);
        Serial.print("\t\t Umidade (%):     \t\t");
        Serial.println(humidade);
        Serial.print("\t\t Pressão Atmosférica (hPa): \t");
        Serial.println(pressao_atmo/100);
        Serial.print("\t\t Estado de Chuva: \t");
        
        if (chuva < 0.70) {
            Serial.print("Chuva Intensa");
        } else if (chuva <  1.50) {
            Serial.print("\tChuva Leve");
        } else {
            Serial.print("\tSem Chuva ( ");
        }
        //Serial.print(chuva); //debug
        Serial.println( ")");
        Serial.print("\t\t Qualidade do Ar (PPD42): \t");
        Serial.println(qualidade_ar);
        Serial.print("\t\t Intensidade de Luz (Lux): \t");
        Serial.println(luz);
        Serial.print("\t\t Intensidade UV (mW/cm²): \t");
        Serial.println(uv);
        Serial.print("\t\t Coordenadas (lat/log): \t");
        Serial.print(lat); Serial.print("   |    "); Serial.println(longi);
        Serial.print("\t\t Battery Status: \t\t");
        Serial.println(batteryLevel);
        Serial.print("\t\t Painel Status: \t\t");
        Serial.println(Painel);         
        Serial.print("\t\t Sensor Error Mask: \t\t");
        Serial.println(mask);
        //checkBatteryStatus();
        //checkUSBChargingStatus();
    }

    void printElapsedTime() {

        unsigned long elapsedMillis = millis();
        int hours = (elapsedMillis / 3600000);
        int minutes = (elapsedMillis % 3600000) / 60000;
        int seconds = (elapsedMillis % 60000) / 1000;

        Serial.print("Elapsed Time (hh:mm:ss): ");
            if (hours < 10) Serial.print("0");
        Serial.print(hours);
        Serial.print(":");
            if (minutes < 10) Serial.print("0");
        Serial.print(minutes);
        Serial.print(":");
            if (seconds < 10) Serial.print("0");
        Serial.println(seconds);
       
    }

    void printToLog() {

        static bool firstPrint = true;
        if (firstPrint) {
            Serial.println("Temp\tHum\tPress\tChuva\tAr\tLuz\tUV\tLat\tLong\tDate\tTime\tMask");
            firstPrint = false;
        }

        Serial.print(temperatura, 2); Serial.print("\t");
        Serial.print(humidade, 2); Serial.print("\t");
        Serial.print(pressao_atmo/100, 2); Serial.print("\t");
        if (chuva == 2) Serial.print("I");
        else if (chuva == 1) Serial.print("L");   
        else Serial.print(chuva);
        Serial.print("\t");
        Serial.print(qualidade_ar, 0); Serial.print("\t");
        Serial.print(luz, 2); Serial.print("\t");
        Serial.print(uv, 2); Serial.print("\t");

        if (gps.location.isValid()) {
            Serial.print(gps.location.lat(), 6); Serial.print("\t");
            Serial.print(gps.location.lng(), 6); Serial.print("\t");
        } else {
            Serial.print("INVALID\tINVALID\t");
        }

        if (gps.date.isValid() && gps.time.isValid()) {
            Serial.print(gps.date.year()); Serial.print("-");
            Serial.print(gps.date.month()); Serial.print("-");
            Serial.print(gps.date.day()); Serial.print("\t");
            Serial.print(gps.time.hour()); Serial.print(":");
            Serial.print(gps.time.minute()); Serial.print(":");
            Serial.print(gps.time.second());
        } else {
            Serial.print("INVALID\tINVALID");
        }
        Serial.print("\t");
        Serial.println(mask);

    }

    void printToCheck() {

        Serial.println("Temp\tHum\tPress\tChuva\tAr\tLuz\tUV\tMask");
        Serial.println("Lat\tLong\tDate\tTime");

        Serial.print(temperatura, 2); Serial.print("\t");
        Serial.print(humidade, 2); Serial.print("\t");
        Serial.print(pressao_atmo, 2); Serial.print("\t");
        if (chuva == 2) Serial.print("I");
        else if (chuva == 1) Serial.print("L");
        else Serial.print("-");
        Serial.print("\t");
        Serial.print(qualidade_ar, 0); Serial.print("\t");
        Serial.print(luz, 2); Serial.print("\t");
        Serial.print(uv, 2); Serial.print("\t");
        Serial.println(mask);

        if (gps.location.isValid()) {
            Serial.print(gps.location.lat(), 6); Serial.print("\t");
            Serial.print(gps.location.lng(), 6); Serial.print("\t");
        } else {
            Serial.print("INVALID\tINVALID\t");
        }

        if (gps.date.isValid() && gps.time.isValid()) {
            Serial.print(gps.date.year()); Serial.print("-");
            Serial.print(gps.date.month()); Serial.print("-");
            Serial.print(gps.date.day()); Serial.print("\t");
            Serial.print(gps.time.hour()); Serial.print(":");
            Serial.print(gps.time.minute()); Serial.print(":");
            Serial.println(gps.time.second());
        } else {
            Serial.print("INVALID\tINVALID\n");
        }

    }