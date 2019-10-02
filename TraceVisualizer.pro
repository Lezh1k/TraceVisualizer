TEMPLATE = app

QT += webenginewidgets

INCLUDEPATH += include \
               tests/include

HEADERS += \
    include/commons/SensorData.h \
    include/commons/SensorDataController.h \
    include/commons/SensorDataParser.h \
    include/coordinates/Coordinates.h \
    include/coordinates/Geohash.h \
    include/commons/Commons.h \
    include/filters/Matrix.h \
    include/filters/Kalman.h \
    include/filters/MadgwickAHRS.h \
    include/filters/GPSAccKalman.h \
    include/mainwindow.h

SOURCES += \
    src/commons/SensorDataController.cpp \
    src/commons/SensorDataParser.cpp \
    src/coordinates/Coordinates.cpp \
    src/coordinates/Geohash.cpp \
    src/filters/Matrix.cpp \
    src/filters/Kalman.cpp \
    src/filters/MadgwickAHRS.cpp \
    src/filters/GPSAccKalman.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    tests/GeohashTest.cpp \
    tests/MatrixTest.cpp \
    tests/CoordinatesTest.cpp \
    tests/SensorDataControllerTest.cpp \
    tests/SensorDataParserTest.cpp

LIBS += -lgtest

FORMS += \
    forms/mainwindow.ui

test:DEFINES += TESTMODE
