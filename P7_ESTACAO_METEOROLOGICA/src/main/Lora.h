#ifndef LORA_H
#define LORA_H

  #include <LoRaWan.h>
  #include "DataHandling.h"
  #include "Variables.h"

  #define DEVEUI "70B3D57ED006F0F8"
  #define APPEUI "8CF9572000079A3E"
  #define APPKEY "2B7E151628AED2A6ABF7158809CF4F3C"

  extern char buffer[256];

  void initializeLoRa();
  void sendData();
  void printToLog();

#endif
