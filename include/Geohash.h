#ifndef GEOHASH_H
#define GEOHASH_H

#include <stdint.h>
static const int GEOHASH_MAX_PRECISION = 12;
uint64_t GeohashEncodeU64(double lat, double lon, int prec);

#endif // GEOHASH_H
