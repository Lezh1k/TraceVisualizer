#ifndef COMMONS_H
#define COMMONS_H

#include <math.h>
#include <assert.h>
#include <functional>

#ifdef __cplusplus
#define restrict __restrict__
#endif

#define UNUSED(x) ((void)x)

#define degree2rad(d) (d * (M_PI/180.0))
#define rad2degree(r) (r * (180.0/M_PI))
///////////////////////////////////////////////////////

inline double MilesPerHour2MeterPerSecond(double mph) {
  return 2.23694 * mph;
}
///////////////////////////////////////////////////////

inline int RandomBetween2Vals(int low, int hi) {
  assert(low <= hi);
  return (std::rand() % (hi - low)) + low;
}
///////////////////////////////////////////////////////

inline double LowPassFilter(double prev, double measured, double alpha) {
  return prev + alpha * (measured - prev);
}
///////////////////////////////////////////////////////

struct Defer {
  std::function<void()> finalize;
  Defer() = delete;
  Defer(std::function<void()> &&fin) : finalize(fin) {}
  ~Defer() { finalize(); }
};

#endif // COMMONS_H
