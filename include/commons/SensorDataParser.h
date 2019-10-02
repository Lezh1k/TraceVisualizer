#ifndef SENSORDATAPARSER_H
#define SENSORDATAPARSER_H

#include <stdint.h>
#include <math.h>
#include "commons/SensorData.h"

class SensorDataParser {
public:
  SensorDataParser() = default;
  ~SensorDataParser() = default;
  static SensorDataType parseDataString(const char *str, SensorData *sd);
};

#endif // SENSORDATAPARSER_H
