package com.example.gpssensorlogger;

public interface IDataLogger {
    boolean start();
    boolean stop();

    public enum LogMessageType {
        LMT_GPS_DATA,
        LMT_ACC_DATA,
        LMT_GYR_DATA,
        LMT_MAG_DATA,
    }
}
