#include "commons/SensorDataController.h"
#include "commons/Commons.h"
#include <cstring>
#include <stdio.h>
#include <utility>

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
  Defer deferFile([fp](){fclose(fp);});
  char *line = new char[256];
  Defer deferLine([line](){delete[] line;});
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

std::pair<sensor_data_storage_bytype_iter_t, sensor_data_storage_bytype_iter_t> SensorDataController::typeRange(SensorDataType sdt) {
  SensorData dummyLow(0, sdt);
  SensorData dummyHig(UINT64_MAX, sdt);
  sensor_data_storage_bytype_iter_t l = storageByType().lower_bound(dummyLow);
  sensor_data_storage_bytype_iter_t r = storageByType().upper_bound(dummyHig);
  return std::make_pair(l, r);
}
///////////////////////////////////////////////////////

const sensor_data_storage_bytime_t &SensorDataController::storageByTime() const {
  return m_data.get<SensorData::ByTime>();
}
///////////////////////////////////////////////////////

const sensor_data_storage_bytype_t &SensorDataController::storageByType() const {
  return m_data.get<SensorData::ByType>();
}
///////////////////////////////////////////////////////
