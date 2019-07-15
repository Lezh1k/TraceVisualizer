#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>

#include "commons/SensorDataParser.h"
#include "filters/GPSAccKalman.h"
#include "coordinates/Coordinates.h"

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
  int tt = sscanf(str, "%" PRIu64 " GPS : pos lat=%lf lon=%lf alt=%lf hdop=%lf speed=%lf bearing=%lf",
                  &sd->timestamp,
                  &sd->data.gps.lat,
                  &sd->data.gps.lon,
                  &sd->data.gps.alt,
                  &sd->data.gps.hdop,
                  &sd->data.gps.speed,
                  &sd->data.gps.bearing);
  return tt == 7;
}
///////////////////////////////////////////////////////

bool parseAccData(const char *str, SensorData *sd) {
  AccData *acc = &sd->data.acc;
  int tt = sscanf(str, "%" PRIu64 " ACC : x=%lf y=%lf z=%lf",
                  &sd->timestamp,
                  &acc->x,
                  &acc->y,
                  &acc->z);
  return tt == 4;
}
///////////////////////////////////////////////////////

bool parseGyrData(const char *str, SensorData *sd) {
  GyrData *gyr = &sd->data.gyr;
  int tt = sscanf(str, "%" PRIu64 " GYR : x=%lf y=%lf z=%lf",
                  &sd->timestamp,
                  &gyr->x,
                  &gyr->y,
                  &gyr->z);
  return tt == 4;
}
///////////////////////////////////////////////////////

bool parseMagData(const char *str, SensorData *sd) {
  MagData *mag = &sd->data.mag;
  int tt = sscanf(str, "%" PRIu64 " MAG : x=%lf y=%lf z=%lf",
                  &sd->timestamp,
                  &mag->x,
                  &mag->y,
                  &mag->z);
  return tt == 4;
}
///////////////////////////////////////////////////////

SensorDataType SensorDataParser::parseDataString(const char *str, SensorData *sd) {
  int pi = -1;
  pi = str[0] - '0';
  if (pi < 0 || pi >= SDT_UNKNOWN)
    return SDT_UNKNOWN;
  return parsers[pi](str+1, sd) ? static_cast<SensorDataType>(pi) : SDT_UNKNOWN;
}
///////////////////////////////////////////////////////
