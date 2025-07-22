

    #include "Moduloalimentacao.h"

    const float MIN_VOLTAGE = 3.3;
    const float MAX_VOLTAGE = 4.2;

    int estimateBatteryPercent(float v) {

        if (v >= 4.20) return 100;
        else if (v >= 4.10) return 90;
        else if (v >= 4.00) return 80;
        else if (v >= 3.90) return 70;
        else if (v >= 3.80) return 60;
        else if (v >= 3.70) return 50;
        else if (v >= 3.60) return 40;
        else if (v >= 3.50) return 30;
        else if (v >= 3.40) return 20;
        else if (v >= 3.30) return 10;
        else return 0; // below 3.3V, battery is nearly empty
    }

    void readBattery() {

        pinMode(ChargingStatusPin, INPUT);

        int analogVoltageReading = analogRead(BatteryVoltagePin);
        int chargeDigitalStatus = digitalRead(ChargingStatusPin);

        float pinVoltage = analogVoltageReading * ( 3.3 / 1023.0 )*11;        
        float estimatedPercentage = estimateBatteryPercent( pinVoltage);
        //Serial.print(pinVoltage);  Serial.print(" | "); Serial.println(estimatedPercentage);     

        batteryLevel = estimatedPercentage;
        Painel = chargeDigitalStatus;

    }

   