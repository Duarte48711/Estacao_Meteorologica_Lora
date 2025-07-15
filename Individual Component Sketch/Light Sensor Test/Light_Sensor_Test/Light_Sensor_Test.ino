
#include <Wire.h>
#include <Digital_Light_TSL2561.h>

bool start = true;

void setup() {
    Serial.begin(9600);
    Wire.begin();
    delay(1000);
}

void loop() {
    if(start){
        Serial.println("=================================================");
        Serial.println("Teste Sensor de Luz ( Grove - Digital Light Sensor v1.1 )");
        Serial.println("=================================================");
        TSL2561.init();
        if (true) {
            Serial.println("Sensor TSL2561 inicializado com sucesso.");
        } else {
            Serial.println("Erro ao inicializar o sensor TSL2561. Verifique as conexões I2C.");
            while(1);
        }
        Serial.println("-------------------------------------------------");
        Serial.println("A imprimir valores de luz:");
        Serial.println("-------------------------------------------------");

        start = false;
    }

    uint16_t IR_raw;
    uint16_t full_raw;
    float lux_calculated;

    full_raw = TSL2561.readFSpecLuminosity();
    IR_raw = TSL2561.readIRLuminosity();

    lux_calculated = TSL2561.readVisibleLux();

    Serial.print("Full Spectrum (Raw): ");
    Serial.println(full_raw);
    Serial.print("InfraRed (Raw):      "); // full spectrum
    Serial.println(IR_raw);
    Serial.print("Visible (Calculado): ");
    Serial.println(full_raw - IR_raw);     // visivel por aproximação
    Serial.print("Lux (Calculado):     ");
    Serial.println(lux_calculated, 2);     // visivel por algoritmo da biblioteca do sensor
    Serial.println("-------------------------------------------------");

    delay(2000);
}
