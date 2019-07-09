package com.example.gpssensorlogger;

import android.annotation.SuppressLint;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;

public class GPSLogger implements LocationListener, IDataLogger {
    private LocationManager m_locationManager;

    public GPSLogger(LocationManager locationManager) {
        m_locationManager = locationManager;
    }

    @Override
    public void onLocationChanged(Location loc) {
        if (loc == null) return;
        if (loc.isFromMockProvider()) return;

        double lat = loc.getLatitude();
        double lon = loc.getLongitude();
        double alt = loc.getAltitude();
        double hdop = loc.getAccuracy();
        double speed = loc.getSpeed();
        double bearing = loc.getBearing();
        long timestamp = TimeController.time();

        @SuppressLint("DefaultLocale")
        String logStr = String.format("%d%d GPS : lat=%f, lon=%f, alt=%f, hdop=%f, speed=%f, bearing=%f",
                        LogMessageType.LMT_GPS_DATA.ordinal(), timestamp, lat, lon, alt, hdop, speed, bearing);
        LogController.Instance().Log(logStr);
    }

    @Override
    public void onStatusChanged(String provider, int status, Bundle extras) {
        //do nothing
    }

    @Override
    public void onProviderEnabled(String provider) {
        //do nothing
    }

    @Override
    public void onProviderDisabled(String provider) {
        //do nothing
    }

    private static final int gpsMinTimeMs = 1000;
    private static final int gpsMinDistanceMeter = 1;


    @SuppressLint("MissingPermission")
    @Override
    public boolean start() {
        m_locationManager.removeUpdates(this);
        m_locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, gpsMinTimeMs,
                gpsMinDistanceMeter, this );
        return true;
    }

    @Override
    public boolean stop() {
        m_locationManager.removeUpdates(this);
        return true;
    }
}
