TEMPLATE = app

QT += webenginewidgets

INCLUDEPATH += include \
               tests/include

HEADERS += \
    include/coordinates/Coordinates.h \
    include/coordinates/Geohash.h \
    include/commons/Commons.h \
    include/commons/SensorController.h \
    include/filters/Matrix.h \
    include/filters/Kalman.h \
    include/filters/MadgwickAHRS.h \
    include/filters/GPSAccKalman.h \
    include/mainwindow.h

SOURCES += \
    src/coordinates/Coordinates.cpp \
    src/coordinates/Geohash.cpp \
    src/commons/SensorController.cpp \
    src/filters/Matrix.cpp \
    src/filters/Kalman.cpp \
    src/filters/MadgwickAHRS.cpp \
    src/filters/GPSAccKalman.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    tests/GeohashTest.cpp \
    tests/MatrixTest.cpp \
    tests/CoordinatesTest.cpp \
    tests/SensorControllerTest.cpp

LIBS += -lgtest
#LIBS += -ltcmalloc

FORMS += \
    forms/mainwindow.ui
