#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <QFile>

#include "SensorController.h"
#include "GPSAccKalman.h"
#include "Coordinates.h"
#include "MeanFilter.h"

static bool parseGpsData(const char *str, SensorData *sd);
static bool parseAccData(const char *str, SensorData *sd);
static bool parseGyrData(const char *str, SensorData *sd);
static bool parseMagData(const char *str, SensorData *sd);

//don't change order here. it's related to LogMessageType
static bool (*parsers[])(const char*, SensorData*) = {
  parseGpsData,
  parseAccData,
  parseGyrData,
  parseMagData
};

bool parseGpsData(const char *str, SensorData *sd) {
  int tt = sscanf(str, "%lf GPS : pos lat=%lf, lon=%lf, alt=%lf, hdop=%lf, speed=%lf, bearing=%lf",
                  &sd->timestamp,
                  &sd->gps.lat,
                  &sd->gps.lon,
                  &sd->gps.alt,
                  &sd->gps.hdop,
                  &sd->gps.speed,
                  &sd->gps.bearing);
  return tt == 7;
}
///////////////////////////////////////////////////////

bool parseAccData(const char *str, SensorData *sd) {
  AccData *acc = &sd->acc;
  int tt = sscanf(str, "%lf ACC : x=%lf, y=%lf, z=%lf",
                  &sd->timestamp,
                  &acc->x,
                  &acc->y,
                  &acc->z);
  return tt == 4;
}
///////////////////////////////////////////////////////

bool parseGyrData(const char *str, SensorData *sd) {
  GyrData *gyr = &sd->gyr;
  int tt = sscanf(str, "%lf GYR : x=%lf, y=%lf, z=%lf",
                  &sd->timestamp,
                  &gyr->x,
                  &gyr->y,
                  &gyr->z);
  return tt == 4;
}
///////////////////////////////////////////////////////

bool parseMagData(const char *str, SensorData *sd) {
  MagData *mag = &sd->mag;
  int tt = sscanf(str, "%lf MAG : x=%lf, y=%lf, z=%lf",
                  &sd->timestamp,
                  &mag->x,
                  &mag->y,
                  &mag->z);
  return tt == 4;
}
///////////////////////////////////////////////////////

LogMessageType
SensorControllerParseDataString(const char *str, SensorData *sd) {
  int pi = -1;  
  pi = str[0] - '0';
  if (pi < 0 || pi >= LMT_UNKNOWN)
    return LMT_UNKNOWN;
  return parsers[pi](str+1, sd) ? static_cast<LogMessageType>(pi) : LMT_UNKNOWN;
}
//////////////////////////////////////////////////////////////////////////
