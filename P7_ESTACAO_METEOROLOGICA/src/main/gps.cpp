#include "gps.h"
#include "LoRaWan.h"
#include "Variables.h"

String   nmeaLine = "";
bool     collecting = false;
bool     hasFix = false; 

// ── NMEA parsing helpers ───────────────────────────────────────
float parseLatitude(String raw, char hemi) {
  // DDMM.MMMM
  float d = raw.substring(0, 2).toFloat();
  float m = raw.substring(2).toFloat();
  float v = d + m/60.0;
  return (hemi=='S') ? -v : v;
}

float parseLongitude(String raw, char dir) {
  // DDDMM.MMMM
  float d = raw.substring(0, 3).toFloat();
  float m = raw.substring(3).toFloat();
  float v = d + m/60.0;
  return (dir=='W') ? -v : v;
}

// ── Extract fields, update lat/lon/hasFix ─────────────────────
void parseGPSLine(String line) {
  // split on commas
  String F[12];
  int idx = 0, start = 0;
  for (int i=0; i<line.length() && idx<12; i++) {
    if (line[i]==',' || i==line.length()-1) {
      F[idx++] = line.substring(start, i);
      start     = i+1;
    }
  }

  String latRaw, lonRaw;
  char   latDir, lonDir;

  if (line.startsWith("$GPGGA")) {
    // $GPGGA,hhmmss.ss, lat, N/S, lon, E/W, fix, ...
    latRaw = F[2];  latDir = F[3].charAt(0);
    lonRaw = F[4];  lonDir = F[5].charAt(0);
    hasFix = (F[6].toInt() > 0);
  }
  else { // $GPRMC
    // $GPRMC,hhmmss.ss,A/V, lat, N/S, lon, E/W, ...
    hasFix = (F[2] == "A");
    latRaw = F[3];  latDir = F[4].charAt(0);
    lonRaw = F[5];  lonDir = F[6].charAt(0);
  }

  if (hasFix && latRaw.length() && lonRaw.length()) {
    lat = parseLatitude(latRaw, latDir);
    longi = parseLongitude(lonRaw, lonDir);
  }
}

void updateGPS(){
while (Serial2.available()) {
    char c = Serial2.read();    
//Serial.write(c);

    // start a new sentence
    if (c == '$') {
      nmeaLine    = "$";
      collecting  = true;
    }
    // build up until CR/LF
    else if (collecting) {
      if (c == '\n' || c == '\r') {
        collecting = false;
        // dispatch only GGA or RMC
        if (nmeaLine.startsWith("$GPGGA") || nmeaLine.startsWith("$GPRMC")) {
          parseGPSLine(nmeaLine);
        }
      }
      else {
        nmeaLine += c;
      }
    }
  }
  delay(10);
}