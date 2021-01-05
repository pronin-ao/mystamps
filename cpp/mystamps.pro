TEMPLATE = app
TARGET = mystamps
INCLUDEPATH += .

QT += widgets core qml

# Input
SOURCES += main.cpp \
    catalogue.cpp \
    datamanager.cpp \
    dbparser.cpp

HEADERS += \
    catalogue.h \
    datamanager.h \
    dbmodel.h \
    dbparser.h

RESOURCES += \
    res.qrc

ANDROID_API_VERSION = 28

QMAKE_CXXFLAGS += -std=c++17

CONFIG += qmltypes
QML_IMPORT_NAME = mystamps.data
QML_IMPORT_MAJOR_VERSION = 1
