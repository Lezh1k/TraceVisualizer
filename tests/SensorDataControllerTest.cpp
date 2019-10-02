#include <gtest/gtest.h>
#include "commons/SensorDataController.h"
#include <QString>

TEST (sensor_controller, by_time) {
  SensorDataController sc;
  char strAcc1[] = "1562745944868|D|TRACETAG|186582721781950 ACC : x=-0,164003 y=1,801637 z=9,825804";
  char strAcc2[] = "1562745944871|D|TRACETAG|186582733744409 ACC : x=-0.179565 y=1.821988 z=9.835381";
  char strGyr1[] = "1562745948173|D|TRACETAG|286586036283535 GYR : x=-0,002131 y=0,001065 z=-0,003196";
  char strGyr2[] = "1562745948151|D|TRACETAG|286586014277327 GYR : x=0,001065 y=-0,002131 z=-0,003196";
  char strMag1[] = "1562745948151|D|TRACETAG|386586013990994 MAG : x=110,400002 y=-77,800003 z=38,100002";
  char strMag2[] = "1562745948195|D|TRACETAG|386586058143702 MAG : x=110,400002 y=-77,800003 z=38,000000";
  char *items[] = {strAcc1, strAcc2, strGyr1, strGyr2, strMag1, strMag2, nullptr};

  for (char **tmp = &items[0]; *tmp; ++tmp)
    ASSERT_TRUE(sc.addLine(*tmp));

  auto &byTime = sc.storageByTime();
  std::vector<SensorData> tv;
  tv.reserve(byTime.size());
  for (auto it : byTime)
    tv.push_back(it);

  for (size_t i = 0; i < tv.size() - 1; ++i) {
    SensorData next = tv[i+1];
    ASSERT_GE(next.timestamp, tv[i].timestamp);
  }
}

TEST (sensor_controller, by_type) {
  SensorDataController sc;
  char str1[] = "1562745944868|D|TRACETAG|186582721781950 ACC : x=-0,164003 y=1,801637 z=9,825804";
  char str2[] = "1562745948173|D|TRACETAG|286586036283535 GYR : x=-0,002131 y=0,001065 z=-0,003196";
  char str3[] = "1562745948151|D|TRACETAG|386586013990994 MAG : x=110,400002 y=-77,800003 z=38,100002";
  char str4[] = "1562745944871|D|TRACETAG|186582733744409 ACC : x=-0.179565 y=1.821988 z=9.835381";
  char str5[] = "1562745948195|D|TRACETAG|386586058143702 MAG : x=110,400002 y=-77,800003 z=38,000000";
  char str6[] = "1562745948151|D|TRACETAG|286586014277327 GYR : x=0,001065 y=-0,002131 z=-0,003196";
  char *items[] = {str1, str4, str2, str6, str3, str5, nullptr};

  for (char **tmp = &items[0]; *tmp; ++tmp)
    ASSERT_TRUE(sc.addLine(*tmp));

  auto &byType = sc.storageByType();
  std::vector<SensorData> tv;
  tv.reserve(byType.size());
  for (auto it : byType)
    tv.push_back(it);

  for (size_t i = 0; i < tv.size() - 1; ++i) {
    SensorData next = tv[i+1];
    ASSERT_GE(next.type, tv[i].type);
    if (next.type != tv[i].type)
      continue;
    ASSERT_GE(next.timestamp, tv[i].timestamp);
  }  

  SensorData dummyLow(0, SDT_GPS_DATA);
  SensorData dummyHig(UINT64_MAX, SDT_GPS_DATA);

  sensor_data_storage_bytype_iter_t l = byType.lower_bound(dummyLow);
  sensor_data_storage_bytype_iter_t r = byType.upper_bound(dummyHig);

  std::cout << "*****************************************" << std::endl;
  for (; l != r; ++l)
    std::cout << l->type << std::endl;
  std::cout << "*****************************************" << std::endl;
}

TEST (sensor_controller, read_file) {
  SensorDataController sc;
  const char *path = "/home/lezh1k/test_data/trace_vis/2019-07-10";
  sc.readFile(path);
}
