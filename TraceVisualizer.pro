TEMPLATE = app

QT += webenginewidgets

INCLUDEPATH += include \
               tests/include

HEADERS += \
    include/Coordinates.h \
    include/Commons.h \
    include/Matrix.h \
    include/Geohash.h \
    include/mainwindow.h \
    tests/include/GeohashTest.h \
    tests/include/MatrixTest.h \
    include/SensorController.h \
    tests/include/SensorControllerTest.h \
    tests/include/CoordinatesTest.h \
    include/Kalman.h \
    include/MadgwickAHRS.h \
    include/GPSAccKalman.h \
    include/MeanFilter.h

SOURCES += src/main.cpp \
    src/Coordinates.cpp \
    src/Matrix.cpp \
    src/Geohash.cpp \
    src/mainwindow.cpp \
    tests/src/GeohashTest.cpp \
    tests/src/MatrixTest.cpp \
    src/SensorController.cpp \
    tests/src/CoordinatesTest.cpp \
    src/Kalman.cpp \
    src/MadgwickAHRS.cpp \
    src/GPSAccKalman.cpp \
    src/MeanFilter.cpp

LIBS += -lgtest

FORMS += \
    forms/mainwindow.ui
