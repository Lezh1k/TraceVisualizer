#ifndef SENSORDATAPARSER_H
#define SENSORDATAPARSER_H

#include <stdint.h>
#include <math.h>

//see java code
enum SensorDataType {
  SDT_GPS_DATA,
  SDT_ACC_DATA,
  SDT_GYR_DATA,
  SDT_MAG_DATA,
  SDT_UNKNOWN
};
///////////////////////////////////////////////////////

struct GpsData {
  double lat, lon, alt;
  double speed, bearing, hdop;
  //!todo optimize
  double xVel() {return speed * cos(bearing);}
  double yVel() {return speed * sin(bearing);}
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
  struct ByType{};
  struct ByTime{};

  uint64_t timestamp;
  SensorDataType type;
  union {
    GpsData gps;
    AccData acc;
    GyrData gyr;
    MagData mag;
  } data;

  struct LessByTimestampPredicate {
    bool operator()(const SensorData& l, const SensorData& r) const noexcept {
      if (l.timestamp == r.timestamp)
        return l.type < r.type;
      return l.timestamp < r.timestamp;
    }
  };

  struct LessByTypePredicate {
    bool operator()(const SensorData& l, const SensorData& r) const noexcept {
      if (l.type == r.type)
        return l.timestamp < r.timestamp;
      return l.type < r.type;
    }
  };
};

class SensorDataParser {
public:
  SensorDataParser() = default;
  ~SensorDataParser() = default;
  static SensorDataType parseDataString(const char *str, SensorData *sd);
};

#endif // SENSORDATAPARSER_H
