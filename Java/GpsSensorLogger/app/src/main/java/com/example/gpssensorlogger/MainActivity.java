package com.example.gpssensorlogger;

import android.hardware.SensorManager;
import android.location.LocationManager;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {
    private GPSLogger m_gpsLogger;
    private SensorLogger m_sensorLogger;
    private Button m_btnGPS;
    private Button m_btnSensors;

    private void btnGps_onClick(View v) {
        Log.d("HOHO", v.toString());
    }

    private void btnSensors_onClick(View v) {
        Log.d("HEHE", v.toString());
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        LocationManager lm = (LocationManager) getSystemService(LOCATION_SERVICE);
        SensorManager sm = (SensorManager) getSystemService(SENSOR_SERVICE);
        m_gpsLogger = new GPSLogger(lm, this);
        m_sensorLogger = new SensorLogger(sm);

        m_btnGPS = findViewById(R.id.btnGPS);
        m_btnSensors = findViewById(R.id.btnSensors);

        m_btnGPS.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                btnGps_onClick(v);
            }
        });

        m_btnSensors.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                btnSensors_onClick(v);
            }
        });

        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
    }
}
