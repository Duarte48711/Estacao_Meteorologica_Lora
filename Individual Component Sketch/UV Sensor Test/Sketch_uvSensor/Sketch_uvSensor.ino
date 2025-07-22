#define UV_PIN A2

void setup() {
  Serial.begin(9600);
  digitalWrite(38, HIGH);
}

void loop() {
  
  int sensorValue;
  long sum = 0;
  for (int i = 0; i < 1024; i++)  // accumulate readings for 1024 times
  {
    sensorValue = analogRead(UV_PIN);
    //Serial.print(sensorValue);
    sum = sensorValue + sum;
    delay(2);
  }
  long meanVal = sum / 1024;  // get mean value  
  Serial.println("----------------------------------------------------------------------");
  Serial.print("meanValue:");
  Serial.println(meanVal);


  float sensorVout = meanVal * (3.300/ 1023.0);
  //Serial.print("The current voltage is:");
  //Serial.println(sensorVout);

   // if(sensorVout >= 0.1){
  Serial.print("UV index1 from datahsheet:");
  float uv= (sensorVout *1000 / 4.3 - 83) / 21;
  Serial.println(uv/25); 

  Serial.print("UV index from SEED:");
  float uvIntensity = sensorVout * 307.0;  // Resultado em mW/m²
  Serial.println(uvIntensity);   

  Serial.print("uv scale catalin:");
  float uvReal = (uv - 29.27) * (11 - 2) / (500 - 29.27) + 2;
  Serial.println(uvReal);
  //}
  //delay(20);
}