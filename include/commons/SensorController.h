#ifndef SENSORCONTROLLER_H
#define SENSORCONTROLLER_H

#include <stdint.h>
#include <QString>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>

//see java code
enum SensorDataType {
  SDT_GPS_DATA,
  SDT_ACC_DATA,
  SDT_GYR_DATA,
  SDT_MAG_DATA,
  SDT_UNKNOWN
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
  struct ByTimestamp{};
  struct ByType{};

  double timestamp;
  SensorDataType type;
  union {
    GpsData gps;
    AccData acc;
    GyrData gyr;
    MagData mag;
  } data;
};

class SensorController {
private:
  static const char *LOGTAG;
#define bmi boost::multi_index
  using storage_t = boost::multi_index_container<SensorData,
    bmi::indexed_by<
      bmi::ordered_non_unique<
        bmi::tag<SensorData::ByType>, bmi::member<SensorData, SensorDataType, &SensorData::type>
      >,
      bmi::ordered_non_unique<
        bmi::tag<SensorData::ByTimestamp>, bmi::member<SensorData, double, &SensorData::timestamp>
      >
    >
  >;
#undef bmi
  storage_t m_data;
public:
  SensorController() = default;
  ~SensorController() = default;

  bool addLine(char *str);
};

#endif // SENSORCONTROLLER_H
