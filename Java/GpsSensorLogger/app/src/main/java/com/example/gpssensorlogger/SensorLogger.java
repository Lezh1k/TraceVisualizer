package com.example.gpssensorlogger;

import android.annotation.SuppressLint;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

public class SensorLogger implements SensorEventListener, IDataLogger {

    private abstract class SensorWrapper {
        Integer type;
        Sensor sensor;
        public SensorWrapper(Integer type, Sensor sensor) {
            this.type = type;
            this.sensor = sensor;
        }
        public abstract void Log(SensorEvent event);
    }

    private class SensorAccWrapper extends SensorWrapper {
        public SensorAccWrapper(Integer type, Sensor sensor) {
            super(type, sensor);
        }

        @SuppressLint("DefaultLocale")
        @Override
        public void Log(SensorEvent event) {
            String logStr = String.format("%d%d ACC : x=%f, y=%f, z=%f",
                    LogMessageType.LMT_ACC_DATA.ordinal(), TimeController.time(), event.values[0],
                    event.values[1], event.values[2]);
            LogController.Instance().Log(logStr); //%lf ACC : x=%lf, y=%lf, z=%lf
        }
    }

    private class SensorGyrWrapper extends SensorWrapper {
        public SensorGyrWrapper(Integer type, Sensor sensor) {
            super(type, sensor);
        }
        @SuppressLint("DefaultLocale")
        @Override
        public void Log(SensorEvent event) {
            String logStr = String.format("%d%d GYR : x=%f, y=%f, z=%f",
                    LogMessageType.LMT_GYR_DATA.ordinal(), TimeController.time(),
                    event.values[0], event.values[1], event.values[2]);
            LogController.Instance().Log(logStr); //%lf GYR : x=%lf, y=%lf, z=%lf
        }
    }

    private class SensorMagWrapper extends SensorWrapper{
        public SensorMagWrapper(Integer type, Sensor sensor) {
            super(type, sensor);
        }
        @SuppressLint("DefaultLocale")
        @Override
        public void Log(SensorEvent event) {
            String logStr = String.format("%d%d MAG : x=%f, y=%f, z=%f",
                    LogMessageType.LMT_MAG_DATA.ordinal(), TimeController.time(),
                    event.values[0], event.values[1], event.values[2]);
            LogController.Instance().Log(logStr); //%lf MAG : x=%lf, y=%lf, z=%lf
        }
    }
    ////////////////////////////////////////////////////////////////////////

    private SensorManager m_sensorManager;
    private boolean m_sensorsEnabled;

    private SensorWrapper[] m_wrappers = {
            new SensorAccWrapper(Sensor.TYPE_ACCELEROMETER, null),
            new SensorGyrWrapper(Sensor.TYPE_GYROSCOPE, null),
            new SensorMagWrapper(Sensor.TYPE_MAGNETIC_FIELD, null)
    };

    public SensorLogger(SensorManager sensorManager) {
        this.m_sensorManager = sensorManager;
        m_sensorsEnabled = true;
        for (SensorWrapper sw : m_wrappers) {
            sw.sensor = m_sensorManager.getDefaultSensor(sw.type);
            m_sensorsEnabled &= sw.sensor != null;
        }
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        boolean found = false;
        SensorWrapper sw = null;
        for (int i = 0; i < m_wrappers.length; ++i) {
            if (m_wrappers[i].type != event.sensor.getType())
                continue;
            sw = m_wrappers[i];
            found = true;
            break;
        }
        if (!found) return;

        sw.Log(event);
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
        //do nothing
    }

    @Override
    public boolean start() {
        boolean result = m_sensorsEnabled;
        for (SensorWrapper sw : m_wrappers) {
            m_sensorManager.unregisterListener(this, sw.sensor);
            result &= m_sensorManager.registerListener(this, sw.sensor, SensorManager.SENSOR_DELAY_GAME);
        }
        return result;
    }

    @Override
    public boolean stop() {
        for (SensorWrapper sw : m_wrappers) {
            m_sensorManager.unregisterListener(this, sw.sensor);
        }
        return true;
    }
}
