#include <stdint.h>
#include <assert.h>
#include "Geohash.h"

static uint64_t interleave(uint64_t x, uint64_t y) {
  x = (x | (x << 16)) & 0x0000ffff0000ffff;
  x = (x | (x << 8)) & 0x00ff00ff00ff00ff;
  x = (x | (x << 4)) & 0x0f0f0f0f0f0f0f0f;
  x = (x | (x << 2)) & 0x3333333333333333;
  x = (x | (x << 1)) & 0x5555555555555555;

  y = (y | (y << 16)) & 0x0000ffff0000ffff;
  y = (y | (y << 8)) & 0x00ff00ff00ff00ff;
  y = (y | (y << 4)) & 0x0f0f0f0f0f0f0f0f;
  y = (y | (y << 2)) & 0x3333333333333333;
  y = (y | (y << 1)) & 0x5555555555555555;

  return x | (y << 1);

  //use pdep instructions
//  return _pdep_u64(x, 0x5555555555555555) | _pdep_u64(y, 0xaaaaaaaaaaaaaaaa);
}
///////////////////////////////////////////////////////

uint64_t GeohashEncodeU64(double lat, double lon, int prec) {
  uint64_t ilat, ilon;
  lat = lat/180.0 + 1.5;
  lon = lon/360.0 + 1.5;
  ilat = *(reinterpret_cast<uint64_t*>(&lat));
  ilon = *(reinterpret_cast<uint64_t*>(&lon));
  ilat >>= 20;
  ilon >>= 20;
  ilat &= 0x00000000ffffffff;
  ilon &= 0x00000000ffffffff;
  return interleave(ilat, ilon) >> ((GEOHASH_MAX_PRECISION-prec)*5);
}
///////////////////////////////////////////////////////
