#include <gtest/gtest.h>
#include "commons/SensorController.h"
#include <QString>


TEST (sensor_controller, add_line) {
  SensorController sc;
  char str1[] = "1562745944868|D|0|186582721781950 ACC : x=-0,164003 y=1,801637 z=9,825804";
  char str2[] = "1562745944871|D|0|186582733744409 ACC : x=-0.179565 y=1.821988 z=9.835381";
  char str3[] = "WRONG_TAG|186582754954450 ACC : x=-0,175974 y=1,770512 z=9,867702";

  ASSERT_TRUE(sc.addLine(str1));
  ASSERT_TRUE(sc.addLine(str2));
  ASSERT_FALSE(sc.addLine(str3));
}
