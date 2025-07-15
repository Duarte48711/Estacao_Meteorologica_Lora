Generated c++
#include Wire.h
#include Dps310.h

Dps310 sensor;

void setup() {

  Serial.begin(9600);
  Wire.begin();
  delay(1000);
  

  if (sensor.begin(Wire) == DPS310_SUCCEEDED) {
    Serial.println(Sensor DPS310 inicializado com sucesso.);
  } else {
    Serial.println(Erro ao inicializar o sensor DPS310. Verifique as conexoes I2C.);
    while(1);
  }
  Serial.println(-------------------------------------------------);
  Serial.println("Testing pressure e temperature ( High Precision Barometric Pressure Sensor DPS310)");
  Serial.println(-------------------------------------------------);
}

void loop() {
  
  float pressure = 0;
  float temperature = 0;
  bool pressure_ok = false;
  bool temp_ok = false;
  int oversampling_rate = 7;

  pressure_ok = sensor.measurePressureOnce(pressure, oversampling_rate);
  temp_ok = sensor.measureTempOnce(temperature, oversampling_rate);

  if (pressure_ok && temp_ok) {
    Serial.print(Pressao (hPa)     );
    Serial.print(pressure  100.0, 2);
    Serial.print(tTemperatura (C) );
    Serial.println(temperature, 2);
  } else {
    Serial.println("Falha na leitura do sensor.");
  }
  Serial.println(-------------------------------------------------);

  delay(2000);
  
}
