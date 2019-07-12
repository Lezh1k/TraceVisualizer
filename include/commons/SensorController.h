#ifndef SENSORCONTROLLER_H
#define SENSORCONTROLLER_H

#include <stdint.h>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/global_fun.hpp>

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

inline bool operator==(const SensorData &l, const SensorData &r) {
  return &l == &r ||
      (l.timestamp == r.timestamp &&
      l.type == r.type);
}
inline bool operator!=(const SensorData &l, const SensorData &r) {return !(l == r);}


class SensorController {
public:
#define bmi boost::multi_index
  using storage_t = boost::multi_index_container<SensorData,
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
  using storage_bytime_t = storage_t::index<SensorData::ByTime>::type;
  using storage_bytime_iter_t = storage_bytime_t::iterator;

  using storage_bytype_t = storage_t::index<SensorData::ByType>::type;
  using storage_bytype_iter_t = storage_bytype_t::iterator;

  SensorController() = default;
  ~SensorController() = default;

  bool addLine(char *str);
  void reset();

  const storage_bytime_t &storageByTime() const;
  const storage_bytype_t &storageByType() const;
private:
  storage_t m_data;
};

#endif // SENSORCONTROLLER_H
