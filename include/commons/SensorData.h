#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <math.h>
#include <stdint.h>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>


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

  SensorData() : timestamp(0), type(SDT_UNKNOWN) {}
  SensorData(uint64_t ts, SensorDataType t) : timestamp(ts), type(t) {}

  struct LessByTimestampPredicate {
    bool operator()(const SensorData &l, const SensorData &r) const noexcept {
      if (l.timestamp == r.timestamp)
        return l.type < r.type;
      return l.timestamp < r.timestamp;
    }
  };

  struct LessByTypePredicate {
    bool operator()(const SensorData &l, const SensorData &r) const noexcept {
      if (l.type == r.type)
        return l.timestamp < r.timestamp;
      return l.type < r.type;
    }    
  };
};

#define bmi boost::multi_index
  using sensor_data_storage_t = boost::multi_index_container<SensorData,
    bmi::indexed_by<
      bmi::ordered_non_unique<
        bmi::tag<SensorData::ByType>, bmi::identity<SensorData>, SensorData::LessByTypePredicate
      >,
      bmi::ordered_non_unique<
        bmi::tag<SensorData::ByTime>, bmi::identity<SensorData>, SensorData::LessByTimestampPredicate
      >
    >
  >;
#undef bmi
  using sensor_data_storage_bytime_t = sensor_data_storage_t::index<SensorData::ByTime>::type;
  using sensor_data_storage_bytime_iter_t = sensor_data_storage_bytime_t::iterator;

  using sensor_data_storage_bytype_t = sensor_data_storage_t::index<SensorData::ByType>::type;
  using sensor_data_storage_bytype_iter_t = sensor_data_storage_bytype_t::iterator;

#endif // SENSORDATA_H
