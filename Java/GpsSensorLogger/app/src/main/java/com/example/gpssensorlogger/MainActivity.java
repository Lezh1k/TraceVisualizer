package com.example.gpssensorlogger;

import android.Manifest;
import android.content.pm.PackageManager;
import android.hardware.SensorManager;
import android.location.LocationManager;
import android.os.Bundle;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {
    private GPSLogger m_gpsLogger;
    private SensorLogger m_sensorLogger;

    private Button m_btnGPS;
    private boolean m_gpsOn = false;

    private Button m_btnSensors;
    private boolean m_sensorsOn = false;

    private boolean changeLoggerState(IDataLogger logger, boolean on,
                                      Button btn, String txt) {
        on = !on;
        if (on) {
            on = logger.start();
            if (!on)
                return on;
            btn.setText(String.format("%s stop", txt));
        } else {
            btn.setText(String.format("%s start", txt));
            logger.stop();
        }
        return on;
    }

    private void btnGps_onClick(View v) {
        m_gpsOn = changeLoggerState(m_gpsLogger, m_gpsOn, m_btnGPS, "GPS");
    }

    private void btnSensors_onClick(View v) {
        m_sensorsOn = changeLoggerState(m_sensorLogger, m_sensorsOn, m_btnSensors, "Sensors");
    }

    private static final int MY_PERMISSIONS_REQUEST_ACCESS_COARSE_LOCATION = 1;
    private static final int MY_PERMISSIONS_REQUEST_ACCESS_FINE_LOCATION = 2;
    private static final int MY_PERMISSIONS_REQUEST_WRITE_EXTERNAL_STORAGE = 3;
    private static final int MY_PERMISSIONS_REQUEST_READ_EXTERNAL_STORAGE = 4;

    private class PermissionContainer {
        String permissionStr;
        int permissionCode;

        public PermissionContainer(String permissionStr, int permissionCode) {
            this.permissionStr = permissionStr;
            this.permissionCode = permissionCode;
        }
    }

    private static void CheckPermission(MainActivity ctx, PermissionContainer c) {
        if (ActivityCompat.checkSelfPermission(ctx, c.permissionStr) ==
                PackageManager.PERMISSION_GRANTED) {
            return;
        }

        if (!ActivityCompat.shouldShowRequestPermissionRationale(ctx, c.permissionStr)) {
            // No explanation needed; request the permission
            ActivityCompat.requestPermissions(ctx, new String[]{c.permissionStr}, c.permissionCode);
            return;
        }
        // Show an explanation to the user *asynchronously* -- don't block
        // this thread waiting for the user's response! After the user
        // sees the explanation, try again to request the permission.
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        LocationManager lm = (LocationManager) getSystemService(LOCATION_SERVICE);
        SensorManager sm = (SensorManager) getSystemService(SENSOR_SERVICE);
        m_gpsLogger = new GPSLogger(lm);
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

        PermissionContainer[] containers = {
                new PermissionContainer(Manifest.permission.ACCESS_FINE_LOCATION,
                        MY_PERMISSIONS_REQUEST_ACCESS_COARSE_LOCATION),
                new PermissionContainer(Manifest.permission.ACCESS_FINE_LOCATION,
                        MY_PERMISSIONS_REQUEST_ACCESS_FINE_LOCATION),
                new PermissionContainer(Manifest.permission.ACCESS_FINE_LOCATION,
                        MY_PERMISSIONS_REQUEST_WRITE_EXTERNAL_STORAGE),
                new PermissionContainer(Manifest.permission.ACCESS_FINE_LOCATION,
                        MY_PERMISSIONS_REQUEST_READ_EXTERNAL_STORAGE),
        };

        for (PermissionContainer c : containers) {
            CheckPermission(this, c);
        }

        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        //!todo implement some protection
    }
}
