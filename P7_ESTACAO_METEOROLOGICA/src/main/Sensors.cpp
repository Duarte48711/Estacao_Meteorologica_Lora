    #include "Sensors.h"
    #include "gps.h"

    Dps310 Dps310PressureSensor;
    DHT20 DHT;


    void initializeSensors() {
        Wire.begin();
        pinMode(38, OUTPUT);
        pinMode(AIR_QUALITY_PIN, INPUT);
        digitalWrite(38, HIGH);
        pinMode(AIR_QUALITY_PIN, INPUT);
        DHT.begin();

        TSL2561.init();
        Dps310PressureSensor.begin(Wire);
    }     
            unsigned long duration;
            unsigned long starttime;
            unsigned long lowpulseoccupancy = 0;
            float ratio = 0;
            float concentration = 0;
            unsigned long sampletime_ms = 30000;
            float air_quality_reading = 0.0;

    float readAirQuality(unsigned long sampletime_ms) {
        duration = pulseIn(AIR_QUALITY_PIN, LOW, sampletime_ms);
        lowpulseoccupancy = lowpulseoccupancy+duration;

            ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=>100
            concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
            lowpulseoccupancy = 0;
            starttime = millis();

           /* Serial.print(lowpulseoccupancy);
            Serial.print(",");
            Serial.print(ratio);
            Serial.print(",");
            Serial.println(concentration);*/
            return concentration/ 0.0283168 ;        
    }
    void uvScaller(float vout){
        if (vout < 50)
            {
                uv = 0;
            }
            else if (vout < 227)
            {
                uv = 1;
            }
            else if (vout < 318)
            {
                uv = 2;
            }
            else if (vout < 408)
            {
                uv = 3;
            }
            else if (vout < 503)
            {
                uv = 4;
            }
            else if (vout < 606)
            {
                uv = 5;
            }
            else if (vout < 696)
            {
                uv = 6;
            }
            else if (vout < 795)
            {
                uv = 7;
            }
            else if (vout < 881)
            {
                uv = 8;
            }
            else if (vout < 976)
            {
                uv = 9;
            }
            else if (vout < 1079)
            {
                uv = 10;
            }
            else
            {
                uv = 11;
            }
    }

    String readSensors() { 

        mask = "00000000"; // Reset mask String

        // DHT20 (Temperature and Humidity) - Mask bits 6 and 5 (Index 0 and 1)
            if (millis() - DHT.lastRead() >= 1000) {
                int status = DHT.read();
                if (status == DHT20_OK) {

                    temperatura = DHT.getTemperature();

                    if(fabs(temperatura) < 0.001 ){
                        mask[0] = '1'; // Set char at index 0
                        
                    }
                    humidade = DHT.getHumidity(); // Read humidity only if DHT read is OK
                    if( fabs(humidade) < 0.001 ){
                    mask[1] = '1'; // Set char at index 1 
                    } 

                }else {                
                    mask[0] = '1'; // Set char at index 0
                    mask[1] = '1'; // Set char at index 1
                }
            }
        
        // Atmospheric Pressure - Mask bit 4 (Index 2)
            bool pressure_ok = Dps310PressureSensor.measurePressureOnce(pressao_atmo, 7);
                if (pressure_ok) {
                    pressao_atmo = pressao_atmo;
                    if(fabs(pressao_atmo) < 0.001 ) 
                    mask[2] = '1'; 
                } /*else {
                    mask[2] = '1'; // Set char at index 2 for failure
                }*/


        // Rain Sensor - Mask bit 3 (Index 3) - Not implemented, remains '0'

            chuva= analogRead(A3);
            chuva=(chuva / 1023.0) * 3.3;
            //Serial.print("Chuva: ");
            //Serial.println(chuva*100);

        // Air Quality - Mask bit 2 (Index 4) 
        //Serial.println("Debug airquaility:");
       /*air_quality_reading = readAirQuality(sampletime_ms);        
         
            if (air_quality_reading != -1.0 || air_quality_reading != 0.0 ) {
                qualidade_ar = air_quality_reading;
            } else {
                mask[4] = '1'; // Set char at index 4 because reading was -1.0
            }*/

        // Light Sensor - Mask bit 1 (Index 5)
            luz = TSL2561.readVisibleLux();
            if(fabs(luz) < 0.001 ){
                 mask[5] = '1'; // Set char at index 5
            }

        // UV Sensor - Mask bit 0 (Index 6)
            float sensorValue;
            float  sum=0;
            for(int i=0;i<1024;i++)// accumulate readings for 1024 times
            {
                sensorValue= (float) analogRead(A2);
                //Serial.print("sensor reading converted: ");
                //Serial.println(sensorValue);
                sum=sensorValue+sum;
                delay(2);
            }
            float meanVal = sum/1024.0;  // get mean value
            //Serial.print("ug/m^3: ");
            //Serial.println(meanVal);
            meanVal = meanVal * (4980/1024.0);
            //Serial.print("ug/m^3 (converted): ");
           // Serial.println(meanVal);
            uvScaller(meanVal);
            
           
            if(fabs(uv) < 0.001 ){
                 mask[6] = '1'; // Set char at index 6
            }

        // GPS 
            updateGPS();
            if(!isnan(lat) || !isnan(longi) || isinf(lat) || isinf(longi)){}
            else {
                mask[7] = '1';
            } 

        return mask; // Return the Arduino String
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
            Serial.println("Chuva Intensa");
        } else if (chuva <  1.50) {
            Serial.println("\tChuva Leve");
        } else {
            Serial.println("\tSem Chuva");
        }
        Serial.print("\t\t Qualidade do Ar (PPD42): \t");
        Serial.println(qualidade_ar);
        Serial.print("\t\t Intensidade de Luz (Lux): \t");
        Serial.println(luz);
        Serial.print("\t\t Intensidade UV (mW/cm²): \t");
        Serial.println(uv);
        Serial.print("\t\t Coordenadas (lat/log): \t");
        Serial.print(lat); Serial.print("   |    "); Serial.println(longi);           
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