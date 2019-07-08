#include <QDebug>
#include <QFile>
#include <vector>
#include <QRegExp>
#include <math.h>

#include "coordinates/Coordinates.h"
#include "coordinates/Geohash.h"
#include "commons/Commons.h"
#include "commons/SensorController.h"

#define EARTH_RADIUS (6371.0 * 1000.0) // meters
#define ACTUAL_GRAVITY 9.80665

/*
 * http://wiki.gis-lab.info/w/%D0%9F%D0%B5%D1%80%D0%B5%D1%81%D1%87%D0%B5%D1%82_%D0%BA%D0%BE%D0%BE%D1%80%D0%B4%D0%B8%D0%BD%D0%B0%D1%82_%D0%B8%D0%B7_Lat/Long_%D0%B2_%D0%BF%D1%80%D0%BE%D0%B5%D0%BA%D1%86%D0%B8%D1%8E_%D0%9C%D0%B5%D1%80%D0%BA%D0%B0%D1%82%D0%BE%D1%80%D0%B0_%D0%B8_%D0%BE%D0%B1%D1%80%D0%B0%D1%82%D0%BD%D0%BE
 */
#define major_axis 6378137.0
#define flattening (1.0 / 298.257223563)
#define minor_axis ((1 - flattening) * major_axis)

//https://en.wikipedia.org/wiki/Great-circle_distance
static double geoDistanceMeters(double lon1, double lat1,
                                double lon2, double lat2) {

#if COORDINATES_HIGH_ACCURACY==0
  double deltaLon = Degree2Rad(lon2 - lon1);
  double deltaLat = Degree2Rad(lat2 - lat1);
  double a = pow(sin(deltaLat / 2.0), 2.0) +
             cos(Degree2Rad(lat1))*
             cos(Degree2Rad(lat2))*
             pow(sin(deltaLon / 2.0), 2.0);
  double c = 2.0 * atan2(sqrt(a), sqrt(1.0-a));
  return EARTH_RADIUS * c;
#else


  double f1 = Degree2Rad(lat1), l1 = Degree2Rad(lon1);
  double f2 = Degree2Rad(lat2), l2 = Degree2Rad(lon2);
  double L = l2 - l1;
  double tanU1 = (1-flattening) * tan(f1);
  double cosU1 = 1 / sqrt((1.0 + tanU1*tanU1));
  double sinU1 = tanU1 * cosU1;
  double tanU2 = (1-flattening) * tan(f2);
  double cosU2 = 1 / sqrt((1.0 + tanU2*tanU2));
  double sinU2 = tanU2 * cosU2;

  double sinl, cosl;
  double sinSqs, sins=0.0, coss=0.0, sig=0.0, sina, cosSqa=0.0, cos2sigM=0.0, C;
  double l = L, ll;
  int iterations = 1000;

  do {
      sinl = sin(l);
      cosl = cos(l);
      sinSqs = (cosU2*sinl) * (cosU2*sinl) + (cosU1*sinU2-sinU1*cosU2*cosl) * (cosU1*sinU2-sinU1*cosU2*cosl);
      if (sinSqs == 0.0)
        break; // co-incident points
      sins = sqrt(sinSqs);
      coss = sinU1*sinU2 + cosU1*cosU2*cosl;
      sig = atan2(sins, coss);
      sina = cosU1 * cosU2 * sinl / sins;
      cosSqa = 1 - sina*sina;
      cos2sigM = (cosSqa != 0.0) ? (coss - 2*sinU1*sinU2/cosSqa) : 0.0; // equatorial line: cosSqα=0 (§6)
      C = flattening/16*cosSqa*(4+flattening*(4-3*cosSqa));
      ll = l;
      l = L + (1-C) * flattening * sina * (sig + C*sins*(cos2sigM+C*coss*(-1+2*cos2sigM*cos2sigM)));
      if (abs(l) > M_PI)
        return 0.0;
  } while (abs(l-ll) > 1e-12 && iterations--);

  if (!iterations)
    return 0.0;

  double uSq = cosSqa * (major_axis*major_axis - minor_axis*minor_axis) / (minor_axis*minor_axis);
  double A = 1 + uSq/16384*(4096+uSq*(-768+uSq*(320-175*uSq)));
  double B = uSq/1024 * (256+uSq*(-128+uSq*(74-47*uSq)));
  double dsig = B*sins*(cos2sigM+B/4*(coss*(-1+2*cos2sigM*cos2sigM)-
                                  B/6*cos2sigM*(-3+4*sins*sins)*(-3+4*cos2sigM*cos2sigM)));

  double s = minor_axis*A*(sig-dsig);
  return s;
#endif
}
//////////////////////////////////////////////////////////////////////////

double CoordLongitudeToMeters(double lon) {
  double distance = geoDistanceMeters(lon, 0.0, 0.0, 0.0);
  return distance * (lon < 0.0 ? -1.0 : 1.0);
}
//////////////////////////////////////////////////////////////////////////

double CoordLatitudeToMeters(double lat) {
  double distance = geoDistanceMeters(0.0, lat, 0.0, 0.0);
  return distance * (lat < 0.0 ? -1.0 : 1.0);
}
//////////////////////////////////////////////////////////////////////////

static geopoint_t getPointAhead(geopoint_t point,
                                double distance,
                                double azimuthDegrees) {
#if COORDINATES_HIGH_ACCURACY==0
  geopoint_t res;
  double radiusFraction = distance / EARTH_RADIUS;
  double bearing = Degree2Rad(azimuthDegrees);
  double lat1 = Degree2Rad(point.Latitude);
  double lng1 = Degree2Rad(point.Longitude);

  double lat2_part1 = sin(lat1) * cos(radiusFraction);
  double lat2_part2 = cos(lat1) * sin(radiusFraction) * cos(bearing);
  double lat2 = asin(lat2_part1 + lat2_part2);

  double lng2_part1 = sin(bearing) * sin(radiusFraction) * cos(lat1);
  double lng2_part2 = cos(radiusFraction) - sin(lat1) * sin(lat2);
  double lng2 = lng1 + atan2(lng2_part1, lng2_part2);
  lng2 = fmod(lng2 + 3.0*M_PI, 2.0*M_PI) - M_PI;

  res.Latitude = Rad2Degree(lat2);
  res.Longitude = Rad2Degree(lng2);
  return res;
#else
  double t1 = Degree2Rad(point.Latitude);
  double l1 = Degree2Rad(point.Longitude);
  double a1 = Degree2Rad(azimuthDegrees);
  double sina1 = sin(a1);
  double cosa1 = cos(a1);

  double tanU1 = (1-flattening) * tan(t1);
  double cosU1 = 1 / sqrt((1 + tanU1*tanU1));
  double sinU1 = tanU1 * cosU1;
  double sig1 = atan2(tanU1, cosa1);
  double sina = cosU1 * sina1;
  double cosSqa = 1 - sina*sina;
  double uSq = cosSqa * (major_axis*major_axis - minor_axis*minor_axis) / (minor_axis*minor_axis);
  double A = 1 + uSq/16384*(4096+uSq*(-768+uSq*(320-175*uSq)));
  double B = uSq/1024 * (256+uSq*(-128+uSq*(74-47*uSq)));
  double cos2sigM, sinSig, cosSig, dsig;
  double sig = distance / (minor_axis*A);
  double ddsig;

  int iterations = 100;

  do {
    cos2sigM = cos(2*sig1 + sig);
    sinSig = sin(sig);
    cosSig = cos(sig);
    dsig = B*sinSig*(cos2sigM+B/4*(cosSig*(-1+2*cos2sigM*cos2sigM)-
                             B/6*cos2sigM*(-3+4*sinSig*sinSig)*(-3+4*cos2sigM*cos2sigM)));
    ddsig = sig;
    sig = distance / (minor_axis*A) + dsig;
  } while (abs(sig-ddsig) > 1e-12 && iterations--);

  assert(iterations); //hack!

  double x = sinU1*sinSig - cosU1*cosSig*cosa1;
  double l = atan2(sinSig*sina1, cosU1*cosSig - sinU1*sinSig*cosa1);
  double C = flattening/16*cosSqa*(4+flattening*(4-3*cosSqa));
  double L = l - (1-C) * flattening * sina *
          (sig + C*sinSig*(cos2sigM+C*cosSig*(-1+2*cos2sigM*cos2sigM)));
  double l2 = fmod((l1+L+3*M_PI) , (2*M_PI)) - M_PI;  // normalise to -180..+180

  geopoint_t res;
  res.Latitude = Rad2Degree(atan2(sinU1*cosSig + cosU1*sinSig*cosa1,
                                 (1-flattening)*sqrt(sina*sina + x*x)));
  res.Longitude = Rad2Degree(l2);
  return res;
#endif
}
//////////////////////////////////////////////////////////////////////////

static geopoint_t pointPlusDistanceEast(geopoint_t point, double distance) {
  return getPointAhead(point, distance, 90.0);
}

static geopoint_t pointPlusDistanceNorth(geopoint_t point, double distance) {
  return getPointAhead(point, distance, 0.0);
}
//////////////////////////////////////////////////////////////////////////

geopoint_t CoordMetersToGeopoint(double lonMeters,
                            double latMeters) {
  geopoint_t point = {0.0, 0.0};
  geopoint_t pointEast = pointPlusDistanceEast(point, lonMeters);
  geopoint_t pointNorthEast = pointPlusDistanceNorth(pointEast, latMeters);
  return pointNorthEast;
}
//////////////////////////////////////////////////////////////////////////

double CoordDistanceBetweenPointsMeters(double lat1, double lon1,
                                        double lat2, double lon2) {
  return geoDistanceMeters(lon1, lat1, lon2, lat2);
}
//////////////////////////////////////////////////////////////////////////

double CoordCaclulateDistance(const std::vector<geopoint_t> &lst) {
  double distance = 0.0;
  double llon, llat;

  if (lst.empty() || lst.size() == 1)
    return 0.0;

  llon = lst[0].Longitude;
  llat = lst[0].Latitude;

  for (auto pp = lst.begin()+1; pp != lst.end(); ++pp) {
    distance += CoordDistanceBetweenPointsMeters(llat, llon,
                                                 pp->Latitude, pp->Longitude);
    llat = pp->Latitude;
    llon = pp->Longitude;
  }
  return distance;
}
//////////////////////////////////////////////////////////////////////////
