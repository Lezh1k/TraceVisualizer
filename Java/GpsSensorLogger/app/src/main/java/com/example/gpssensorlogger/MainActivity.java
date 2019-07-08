package com.example.gpssensorlogger;

import android.hardware.SensorManager;
import android.location.LocationManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import com.orhanobut.logger.Logger;

public class MainActivity extends AppCompatActivity {
    private GPSLogger m_gpsLogger;
    private SensorLogger m_sensorLogger;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        LocationManager lm = (LocationManager) getSystemService(LOCATION_SERVICE);
        SensorManager sm = (SensorManager) getSystemService(SENSOR_SERVICE);
        m_gpsLogger = new GPSLogger(lm, this);
        m_sensorLogger = new SensorLogger(sm);
    }
}
