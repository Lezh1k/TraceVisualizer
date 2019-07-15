#ifndef SENSORDATACONTROLLER_H
#define SENSORDATACONTROLLER_H

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>

#include "SensorDataParser.h"

class SensorDataController {
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

  SensorDataController() = default;
  ~SensorDataController() = default;

  bool addLine(char *str);
  bool readFile(const char *path);
  void reset();

  const storage_bytime_t &storageByTime() const;
  const storage_bytype_t &storageByType() const;

private:
  storage_t m_data;
};

#endif // SENSORDATACONTROLLER_H
