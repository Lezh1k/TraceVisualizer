package com.example.gpssensorlogger;

import android.Manifest;
import android.annotation.SuppressLint;
import android.content.Context;
import android.content.pm.PackageManager;
import android.location.GpsStatus;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.support.v4.app.ActivityCompat;

import com.orhanobut.logger.Logger;

public class GPSLogger implements LocationListener, IDataLogger {
    private LocationManager m_locationManager;
    private Context m_context;

    public GPSLogger(LocationManager locationManager, Context context) {
        m_locationManager = locationManager;
        m_context = context;
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

        @SuppressLint("DefaultLocale") String logStr =
                String.format("%d%d GPS : lat=%f, lon=%f, alt=%f, hdop=%f, speed=%f, bearing=%f",
                        LogMessageType.LMT_GPS_DATA.ordinal(), timestamp, lat, lon, alt, hdop, speed, bearing);
        Logger.i(logStr); //todo!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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

    @Override
    public boolean start() {
        if (ActivityCompat.checkSelfPermission(m_context, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            return false;
        }
        m_locationManager.removeGpsStatusListener((GpsStatus.Listener) this);
        m_locationManager.addGpsStatusListener((GpsStatus.Listener) this);
        m_locationManager.removeUpdates(this);
        m_locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 1000, 1, this );
        return true;
    }

    @Override
    public boolean stop() {
        m_locationManager.removeGpsStatusListener((GpsStatus.Listener) this);
        m_locationManager.removeUpdates(this);
        return true;
    }
}
