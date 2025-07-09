// PPD42NS Dust Sensor Configuration
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

void setup() {  
  Serial.begin(9600);
  Serial.println("Setup Start");
  pinMode(8, INPUT);
  startTime = millis();
  
}
void loop() {
  unsigned long duration;
  unsigned long lowPulseOccupancy = 0;
  unsigned long startTime = millis();

  while ((millis() - startTime) < sampleTimeMs) {
    duration = pulseIn(dustSensorPin, LOW);
    lowPulseOccupancy += duration;
  }
  Serial.print("( ");
  Serial.print(measure++);
  Serial.print(" , ");
  
  float ratio = lowPulseOccupancy / (sampleTimeMs * 10.0); // as per Seeed formula
  float concentration = 1.1 * pow(ratio, 3) - 3.8 * pow(ratio, 2) + 520 * ratio + 0.62;

  //Serial.print("Dust concentration = ");
  Serial.print(concentration * 0.0477, 2);
  if(measure%5==0){
      Serial.println(" )");
  }
  else{
      Serial.print(" )");
  }
  //Serial.println(" µg/m³");

  //delay(1000); // optional delay before the next cycle
}/*
void loop() {

   pulseDuration = pulseIn(dustSensorPin, LOW,sampleTimeMs);
    lowPulseOccupancy += pulseDuration;
    if ((millis() - startTime) > sampleTimeMs) {
        pulseRatio = lowPulseOccupancy / (sampleTimeMs * 10.0); 
        concentration = (1.1 * pow(pulseRatio, 3) - 3.8 * pow(pulseRatio, 2) + 520 * pulseRatio + 0.62) * 0.1; 
        Serial.print("Dust concentration = ");
        Serial.print(concentration);
        Serial.println(" micrograms/m^3");
        Serial.println("\n");
        Serial.println(" micrograms/m^3");
        Serial.println("\n");

        
        lowPulseOccupancy = 0;
        startTime = millis();
      
    if(fabs(concentration) > 0.01){
        Serial.print((double) concentration,2);
        //Serial.print(" µg/m³");
        count++;
        if(count == 10){
          count = 0;
          Serial.println();
        }
        else{
          Serial.print(", ");
        }
    }
    else{
      Serial.println("");
      Serial.print("error");
    }

    lowPulseOccupancy = 0;
    startTime = millis();
  }
}
*/