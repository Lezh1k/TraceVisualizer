package com.example.gpssensorlogger;

import com.orhanobut.logger.AndroidLogAdapter;
import com.orhanobut.logger.FormatStrategy;
import com.orhanobut.logger.Logger;
import com.orhanobut.logger.PrettyFormatStrategy;

public class LogController {
    private static final LogController INSTANCE = new LogController();

    private LogController() {

        FormatStrategy formatStrategy = PrettyFormatStrategy.newBuilder()
                .showThreadInfo(false)  // (Optional) Whether to show thread info or not. Default true
                .methodCount(0)         // (Optional) How many method line to show. Default 2
                .tag("1")
                .build();

        Logger.addLogAdapter(new AndroidLogAdapter(formatStrategy));
    }

    public static LogController Instance() {
        return INSTANCE;
    }

    public void Log(String str) {
        Logger.d(str);
    }
}
