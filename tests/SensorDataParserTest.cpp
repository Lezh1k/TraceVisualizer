#include <gtest/gtest.h>
#include "commons/SensorDataParser.h"

TEST (sensor_parser, parse) {
  SensorData sd;
  char str1[] = "186582721781950 ACC : x=-0.164003 y=1.801637 z=9.825804";
  char str2[] = "186582733744409 ACC : x=-0.179565 y=1.821988 z=9.835381";
  char str3[] = "86582754954450 ACC : x=-0.175974 y=1.770512 z=9.867702";

  ASSERT_EQ(SensorDataParser::parseDataString(str1, &sd), SDT_ACC_DATA);
  ASSERT_EQ(SensorDataParser::parseDataString(str2, &sd), SDT_ACC_DATA);
  ASSERT_EQ(SensorDataParser::parseDataString(str3, &sd), SDT_UNKNOWN);
}
///////////////////////////////////////////////////////
