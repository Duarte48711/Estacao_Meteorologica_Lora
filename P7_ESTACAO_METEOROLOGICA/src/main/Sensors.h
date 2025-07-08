#ifndef SENSORS_H
#define SENSORS_H

  #include "Variables.h"
  #include "DataHandling.h"
  #include "Moduloalimentacao.h"
  #include "gps.h"
  #include <Arduino.h>
  #include <Wire.h>
  #include "DHT20.h"
  #include <Digital_Light_TSL2561.h>
  #include <Dps310.h>

  void initializeSensors();
  float readAirQuality(unsigned long sampletime_ms);
  String readSensors();
  void printElapsedTime();
  void printSensorValues();
  void printToLog();
  void printToCheck();

#endif // SENSORS_H