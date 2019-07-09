package com.example.gpssensorlogger;

import com.elvishew.xlog.LogConfiguration;
import com.elvishew.xlog.LogLevel;
import com.elvishew.xlog.XLog;
import com.elvishew.xlog.printer.AndroidPrinter;
import com.elvishew.xlog.printer.Printer;
import com.elvishew.xlog.printer.file.FilePrinter;
import com.elvishew.xlog.printer.file.backup.FileSizeBackupStrategy;
import com.elvishew.xlog.printer.file.naming.DateFileNameGenerator;

public class LogController {
    private static final LogController INSTANCE = new LogController();

    private LogController() {

        LogConfiguration config = new LogConfiguration.Builder()
                .logLevel(LogLevel.ALL)
                .tag("0")
                .build();

        Printer androidPrinter = new AndroidPrinter();
        Printer filePrinter = new FilePrinter
                .Builder("/sdcard/xlog/")
                .fileNameGenerator(new DateFileNameGenerator())
                .backupStrategy(new FileSizeBackupStrategy(2 * 1024 * 1024))
                .build();

        XLog.init(config, androidPrinter/* ,filePrinter*/);
    }

    public static LogController Instance() {
        return INSTANCE;
    }

    public void Log(String str) {
        XLog.d(str);
    }
}
