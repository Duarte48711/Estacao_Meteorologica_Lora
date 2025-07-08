#ifndef GPS_H
#define GPS_H

#include <TinyGPS++.h>

extern TinyGPSPlus gps;

void parseGPSLine(String line);
float parseLongitude(String raw, char dir);
float parseLatitude(String raw, char hemi);
void updateGPS();

#endif // GPS_H