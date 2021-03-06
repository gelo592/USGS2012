QT+= testlib

CONFIG += testcase debug

TARGET = runTests

DEPENDPATH += . \
              ../main \

SOURCES +=  ../main/model/configuration.cpp \
            ../main/model/hydrofile.cpp \
            ../main/model/status.cpp \

INCLUDEPATH += ../main/model

# Input
HEADERS +=  GridTests.h \
            ReducedGridTests.h \
            ConfigTests.h \
            HydroFileTests.h \
            StatusTests.h \

SOURCES +=  TestMain.cpp \
            GridTests.cpp \
            ReducedGridTests.cpp \
            ConfigTests.cpp \
            HydroFileTests.cpp \
            StatusTests.cpp \
