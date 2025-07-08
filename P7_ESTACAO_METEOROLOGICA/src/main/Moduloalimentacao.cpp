

    #include "Moduloalimentacao.h"
    

    // Check battery status
    void checkBatteryStatus() {
        
        float referenceVoltage = 3.7; 
        float voltageDividerRatio = 1.0;
        // Read raw analog value
        int batteryVoltage = analogRead(batteryPin);

        // Convert raw analog reading to actual voltage
        float voltage = (batteryVoltage * referenceVoltage / 1023.0) * voltageDividerRatio;

        // Print voltage
        Serial.print(" Battery Voltage: ");
        Serial.print(voltage);
        Serial.println(" V");

        // Determine battery status based on thresholds
        if (voltage < 1.2) { // Low battery threshold (example: 3.0V is critical for 3.7V battery)
            Serial.println("\t\t Battery:\t Low!");
        } else if (voltage >= 1.2 && voltage <= 2.4) { // Mid-range battery (example range)
            Serial.println("\t\t Battery:\t Medium.");
        } else if (voltage >= 2.4 && voltage <= 3.6) { // Mid-range battery (example range)
            Serial.println("\t\t Battery:\t High.");
        } else if (voltage >= 3.6 && voltage <= 3.7){ // Fully charged threshold
            Serial.println("\t\t Battery:\t Full.");
        }
    }


    // Check USB charging status
    void checkUSBChargingStatus() {
        pinMode(A5 , INPUT);
        int usbCharging = analogRead(chargingPin); // Read USB charge pin
        Serial.println("\t\t" + usbCharging);
       /* if (usbCharging > 512) { // Example threshold (adjust for your setup)
            Serial.println(" USB is connected and charging.");
        } else {
            Serial.println(" USB is not charging.");
        }*/
    }
