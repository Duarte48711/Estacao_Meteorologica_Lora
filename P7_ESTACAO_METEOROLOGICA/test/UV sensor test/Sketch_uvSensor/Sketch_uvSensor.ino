

void setup() {
  Serial.begin(9600);
  digitalWrite(38, HIGH);
}

void loop() {
  int sensorValue;
  long sum = 0;
  for (int i = 0; i < 1024; i++)  // accumulate readings for 1024 times
  {
    sensorValue = analogRead(A2);
    //Serial.print(sensorValue);
    sum = sensorValue + sum;
    delay(2);
  }
  long meanVal = sum / 1024;  // get mean value  
  Serial.print("meanValue:");
  Serial.println(meanVal);
  float sensorVout = meanVal * (3.300/ 1023.0);
  Serial.print("The current voltage is:");
  Serial.println(sensorVout);
  if(sensorVout >= 0.1){
  Serial.print("The current UV index is:");
  float uv= (sensorVout *1000 / 4.3 - 83) / 21;
  Serial.print("Before Sacaling:"); 
  Serial.println(uv); 
  }
  delay(20);
}