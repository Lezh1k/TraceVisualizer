#ifndef SENSORDATACONTROLLER_H
#define SENSORDATACONTROLLER_H

#include "SensorDataParser.h"
#include "SensorData.h"

class SensorDataController {
public:
  SensorDataController() = default;
  ~SensorDataController() = default;

  bool addLine(char *str);
  bool readFile(const char *path);
  void reset();

  std::pair<sensor_data_storage_bytype_iter_t, sensor_data_storage_bytype_iter_t> typeRange(SensorDataType sdt);

  const sensor_data_storage_bytime_t &storageByTime() const;
  const sensor_data_storage_bytype_t &storageByType() const;

private:
  sensor_data_storage_t m_data;
};

#endif // SENSORDATACONTROLLER_H
