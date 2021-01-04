TEMPLATE = app
TARGET = mystamps
INCLUDEPATH += .

QT += widgets core

# Input
SOURCES += main.cpp \
    datamanager.cpp \
    dbparser.cpp

HEADERS += \
    datamanager.h \
    dbmodel.h \
    dbparser.h

RESOURCES += \
    res.qrc

ANDROID_API_VERSION = 28

QMAKE_CXXFLAGS += -std=c++17
