#include "commons/SensorDataController.h"
#include "commons/Commons.h"
#include <cstring>
#include <stdio.h>

bool SensorDataController::addLine(char *str) {
static const int TAGLEN = 9;
static const char *LOGTAG = "TRACETAG|"; //see java code (XLOG settings)
  char *sub = std::strstr(str, LOGTAG);
  if (sub == nullptr)
    return false;
  sub += TAGLEN;
  for (char *tmp = sub; *tmp; ++tmp) {
    if (*tmp != ',')
      continue;
    *tmp = '.';
  }
  SensorData sd;
  sd.type = SensorDataParser::parseDataString(sub, &sd);
  if (sd.type == SDT_UNKNOWN)
    return false;
  auto ir = m_data.insert(sd);
  return ir.second;
}
///////////////////////////////////////////////////////

bool SensorDataController::readFile(const char *path) {
  FILE* fp = fopen(path, "r");
  if (!fp)
    return false;
  Defer deferFile(std::bind(fclose, fp)); //[fp]() { fclose(fp); }
  char *line = static_cast<char*>(malloc(256));
  Defer deferLine(std::bind(free, line));
  bool res = true;
  size_t len = 0;
  while ((getline(&line, &len, fp)) != -1)
    res &= addLine(line); //just try to add all lines to storage
  return res;
}
///////////////////////////////////////////////////////

void SensorDataController::reset() {
  m_data.erase(m_data.begin(), m_data.end());
}
///////////////////////////////////////////////////////

const SensorDataController::storage_bytime_t &SensorDataController::storageByTime() const {
  return m_data.get<SensorData::ByTime>();
}
///////////////////////////////////////////////////////

const SensorDataController::storage_bytype_t &SensorDataController::storageByType() const {
  return m_data.get<SensorData::ByType>();
}
///////////////////////////////////////////////////////
