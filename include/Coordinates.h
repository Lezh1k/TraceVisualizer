#ifndef COORDINATES_H
#define COORDINATES_H

#include <vector>
class QString;

#define COORDINATES_HIGH_ACCURACY 1

struct geopoint_t {
  double Latitude, Longitude;
  geopoint_t() : Latitude(0.0), Longitude(0.0) {
  }
  geopoint_t(double lat, double lon) : Latitude(lat), Longitude(lon) {
  }
};

/*todo write tests*/
double CoordDistanceBetweenPointsMeters(double lat1, double lon1,
                                        double lat2, double lon2);
double CoordLongitudeToMeters(double lon);
double CoordLatitudeToMeters(double lat);
geopoint_t CoordMetersToGeopoint(double lonMeters,
                                 double latMeters);

double CoordCaclulateDistance(const std::vector<geopoint_t> &lst);

#endif // COORDINATES_H
