#include "Lora.h"

char buffer[256];

void initializeLoRa() {

    SerialUSB.begin(115200);
    while(!SerialUSB);
    lora.init();
    memset(buffer, 0, 256);
    lora.getVersion(buffer, 256, 1);
    SerialUSB.print(buffer); 
    memset(buffer, 0, 256);
    lora.getId(buffer, 256, 1);
    SerialUSB.print(buffer);
    lora.setKey( APPEUI, DEVEUI , APPKEY);
    lora.setDeciveMode(LWOTAA);
    lora.setDataRate(DR0, EU868);
    lora.setChannel(0, 868.1);
    lora.setChannel(1, 868.3);
    lora.setChannel(2, 868.5);
    lora.setReceiceWindowFirst(0, 868.1);
    lora.setReceiceWindowSecond(869.5, DR3);
    lora.setDutyCycle(false);
    lora.setJoinDutyCycle(false);
    lora.setPower(14);
    while(!lora.setOTAAJoin(JOIN));
}

void sendData() {

    float maskValue = strtol(mask.c_str(), nullptr, 2);
    // Original float array
    float payload[12] = { 
        temperatura,
        humidade,
        pressao_atmo,
        chuva,
        qualidade_ar,
        luz,
        uv ,
        lat,
        longi, //not long due to long type of variable
        batteryLevel,
        Painel,
        maskValue,      
    };

    // Serialize the float array to bytes
        uint8_t* bytes = serializeFloatArray(payload, 12);
        unsigned int messageSize = 10 * sizeof(float); // Calculate size in bytes

        // Measure the time taken for communication
        unsigned long startTime = millis(); // Start time
        bool result = lora.transferPacket(bytes, messageSize);
        unsigned long endTime = millis(); // End time

    if (result) {
        short length;
        short rssi;

        // Clear the buffer
        memset(buffer, 0, 256);

        // Receive any response from the network
        length = lora.receivePacket(buffer, 256, &rssi);

        if (length) {
            SerialUSB.print("Length is: ");
            SerialUSB.print(length);
            SerialUSB.print("\t RSSI is: ");
            SerialUSB.println(rssi);
            /*SerialUSB.print("Data is: ");
            for (unsigned char i = 0; i < length; i++) {
                SerialUSB.print("0x");
                SerialUSB.print(buffer[i], HEX);
                SerialUSB.print(" ");
            }*/
            SerialUSB.println();
        }

        SerialUSB.println("Communication successful!");
        SerialUSB.print("Time taken (ms): ");
        SerialUSB.print(endTime - startTime); // Time elapsed
        SerialUSB.print("\t Message size (bytes): ");
        SerialUSB.println(messageSize); // Size of message sent
        delete[] bytes;

    } else {
        SerialUSB.println("Failed to send data!");
        SerialUSB.print("Time taken (ms): ");
        SerialUSB.print(endTime - startTime); // Time elapsed even in failure
        SerialUSB.print("\t Message size (bytes): ");
        SerialUSB.println(messageSize); // Confirm size even in failure
        delete[] bytes;
    }
}

