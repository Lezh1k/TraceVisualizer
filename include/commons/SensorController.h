#ifndef SENSORCONTROLLER_H
#define SENSORCONTROLLER_H

#include <stdint.h>
#include <stdbool.h>
#include <QString>

//see java code
enum LogMessageType {  
  LMT_GPS_DATA,
  LMT_ACC_DATA,
  LMT_GYR_DATA,
  LMT_MAG_DATA,
  LMT_UNKNOWN
};
///////////////////////////////////////////////////////

struct LogRawData {
  char *hdr;
  int hdrLen;
  char *data;
  int dataLen;
};
///////////////////////////////////////////////////////

struct GpsData {
  double lat, lon, alt;
  double speed, bearing, hdop;
  /*
    xVel = speed * cos(bearing);
    yVel = speed * sin(bearing);
  */
};

struct AccData {
  double x, y, z;
};

struct GyrData {
  double x, y, z;
};

struct MagData {
  double x, y, z;
};

struct SensorData {
  double timestamp;
  GpsData gps;
  AccData acc;
  GyrData gyr;
  MagData mag;
};

#endif // SENSORCONTROLLER_H
