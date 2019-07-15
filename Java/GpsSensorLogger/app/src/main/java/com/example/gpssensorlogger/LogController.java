package com.example.gpssensorlogger;

import android.os.Environment;

import com.elvishew.xlog.LogConfiguration;
import com.elvishew.xlog.LogLevel;
import com.elvishew.xlog.XLog;
import com.elvishew.xlog.printer.AndroidPrinter;
import com.elvishew.xlog.printer.Printer;
import com.elvishew.xlog.printer.file.FilePrinter;
import com.elvishew.xlog.printer.file.backup.FileSizeBackupStrategy;
import com.elvishew.xlog.printer.file.naming.DateFileNameGenerator;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

public class LogController {
    private static final LogController inst = new LogController();

    private LogController() {
        LogConfiguration config = new LogConfiguration.Builder()
                .logLevel(LogLevel.ALL)
                .tag("TRACETAG")
                .build();

        List<Printer> lstPrinters = new ArrayList<Printer>();
        lstPrinters.add(new AndroidPrinter());

        //Create Folder
        String sd = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOCUMENTS).toString();
        sd += "/TraceVisualizer";
        File folder = new File(sd);
        boolean exists = folder.exists();
        if (!exists)
            exists = folder.mkdirs();

        if (exists) {
            String extStorageDirectory = folder.toString();
            Printer filePrinter = new FilePrinter
                    .Builder(extStorageDirectory)
                    .fileNameGenerator(new DateFileNameGenerator())
                    .backupStrategy(new FileSizeBackupStrategy(2 * 1024 * 1024))
                    .build();
            lstPrinters.add(filePrinter);
        }

        Printer[] arrPrinters = new Printer[lstPrinters.size()];
        XLog.init(config, lstPrinters.toArray(arrPrinters));
    }

    public static LogController Instance() {
        return inst;
    }

    public void Log(String str) {
        XLog.d(str);
    }
}
