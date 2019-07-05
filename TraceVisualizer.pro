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
    include/SensorController.h \
    include/Kalman.h \
    include/MadgwickAHRS.h \
    include/GPSAccKalman.h \
    include/MeanFilter.h

SOURCES += src/main.cpp \
    src/Coordinates.cpp \
    src/Matrix.cpp \
    src/Geohash.cpp \
    src/mainwindow.cpp \
    src/Kalman.cpp \
    src/MadgwickAHRS.cpp \
    src/GPSAccKalman.cpp \
    src/SensorController.cpp \
    src/MeanFilter.cpp \
    tests/GeohashTest.cpp \
    tests/MatrixTest.cpp \
    tests/CoordinatesTest.cpp \

LIBS += -lgtest

FORMS += \
    forms/mainwindow.ui
