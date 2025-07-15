

    #include "Moduloalimentacao.h"

    const float MIN_VOLTAGE = 3.0;
    const float MAX_VOLTAGE = 5.5;

    void readBattery() {

        int analogVoltageReading = analogRead(BatteryVoltagePin);
        int chargeDigitalStatus = digitalRead(ChargingStatusPin);

        float pinVoltage = analogVoltageReading * ( 3.3 / 1023.0 );
        float batteryVoltage = pinVoltage * 11;
        Serial.println(batteryVoltage);
        float estimatedPercentage = (batteryVoltage - MIN_VOLTAGE) / (MAX_VOLTAGE - MIN_VOLTAGE) * 100.0;

        if (estimatedPercentage < 0.0) {
            estimatedPercentage = 0.0;
        } else if (estimatedPercentage > 100.0) {
            estimatedPercentage = 100.0;
        }

        batteryLevel = estimatedPercentage;
        Painel = chargeDigitalStatus;

    }

    // Check battery status
    void checkBatteryStatus() {
        
        int  a= analogRead(BatteryVoltagePin);
        float voltage = a/1023.0*3.3*11.0;        // there's an 1M and 100k resistor divider
        Serial.print(voltage, 2);
        Serial.print('\t');
        
        int painelStatus = digitalRead(ChargingStatusPin);
        Serial.print(painelStatus, 2);
        Serial.println('\t');
        batteryLevel = voltage*20;
        if(batteryLevel > 100){
            batteryLevel =100;
        }        
        Painel = painelStatus;


        // Print voltage
        /*

        // Determine battery status based on thresholds
        if (voltage < 1.2) { // Low battery threshold (example: 3.0V is critical for 3.7V battery)
            Serial.println("\t\t Battery:\t Low!");
        } else if (voltage >= 1.2 && voltage <= 2.4) { // Mid-range battery (example range)
            Serial.println("\t\t Battery:\t Medium.");
        } else if (voltage >= 2.4 && voltage <= 3.6) { // Mid-range battery (example range)
            Serial.println("\t\t Battery:\t High.");
        } else if (voltage >= 3.6 && voltage <= 3.7){ // Fully charged threshold
            Serial.println("\t\t Battery:\t Full.");
        }*/
    }


    // Check USB charging status
    void checkUSBChargingStatus() {

        pinMode(A5 , INPUT);
        int usbCharging = analogRead(ChargingStatusPin); // Read USB charge pin
        Serial.println("\t\t" + usbCharging);
       /* if (usbCharging > 512) { // Example threshold (adjust for your setup)
            Serial.println(" USB is connected and charging.");
        } else {
            Serial.println(" USB is not charging.");
        }*/
    }
