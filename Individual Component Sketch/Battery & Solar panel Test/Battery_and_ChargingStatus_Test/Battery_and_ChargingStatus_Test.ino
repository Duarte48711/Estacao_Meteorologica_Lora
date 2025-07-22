// battey connected via JST
 // Usb charging throw an Solar Panel
#define BatteryPin A4
#define ChargingPin A5
#define MIN_VOLTAGE  0.1  // Tensão considerada 0% (limite de descarga seguro aproximado)
#define MAX_VOLTAGE  3.7

void setup() {

    SerialUSB.begin(9600);
    delay(100);
    SerialUSB.println("Battery Monitoring Started");
    pinMode(BatteryPin, INPUT); 

}

void loop() { 

    int analogValue = analogRead(BatteryPin); // Lê o valor analógico (0-1023)

    // Converte o valor analógico para tensão 3.3 refere-se à tensão de referência do ADC (Vref)1023.0 é o valor máximo lido pelo ADC (usado como float para divisão)
    // 11.0 é o fator de correção do divisor de tensão (1M + 100k = 1.1M, rácio 1:11)
        float batteryVoltage = analogValue * (3.3 / 1023.0) * 11.0;    
        float batteryPercentage = ((batteryVoltage - MIN_VOLTAGE) / (MAX_VOLTAGE - MIN_VOLTAGE)) * 100.0; //valor aprocimado por balização

    // Limita a percentagem entre 0 e 100% pra correção de erros
        if (batteryPercentage < 0.0) {
            batteryPercentage = 0.0;
        } else if (batteryPercentage > 100.0) {
            batteryPercentage = 100.0;
        }

    //Leitura do Estado de Carregamento
    // Lê o estado digital do pino (HIGH ou LOW)
        int chargeStatus = digitalRead(ChargingPin);

    SerialUSB.print("Battery Voltage: ");
    SerialUSB.print(batteryVoltage, 2); 
    SerialUSB.print(" V\t");

    SerialUSB.print("Estimated Percentage: ");
    SerialUSB.print(batteryPercentage, 1); 
    SerialUSB.print(" %\t");

    SerialUSB.print("Charge Status: ");
    if (chargeStatus == HIGH) {
       SerialUSB.print("Not Charging (");  
       SerialUSB.print(chargeStatus);
       SerialUSB.println(")") ;      // HIGH = no charging (pode ser carga completa, sem bateria ou sem USB(painel) )        
    } else {
        
        SerialUSB.print("Charging (");     // LOW = charging"
        SerialUSB.print(chargeStatus);
        SerialUSB.println(")");
    } 
    delay(1000);
}