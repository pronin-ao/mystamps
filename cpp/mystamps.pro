TEMPLATE = app
TARGET = mystamps
INCLUDEPATH += .

QT += widgets core qml network webview

# Input
SOURCES += main.cpp \
    adminmodel.cpp \
    adminproxymodel.cpp \
    catalogue.cpp \
    datamanager.cpp \
    dbparser.cpp \
    delegates.cpp \
    urlimagelabel.cpp

HEADERS += \
    adminmodel.h \
    adminproxymodel.h \
    catalogue.h \
    catalogue_constants.h \
    datamanager.h \
    dbmodel.h \
    dbparser.h \
    delegates.h \
    stamp.h \
    urlimagelabel.h

RESOURCES += \
    res.qrc

ANDROID_API_VERSION = 28

QMAKE_CXXFLAGS += -std=c++17

CONFIG += qmltypes
QML_IMPORT_NAME = mystamps.data
QML_IMPORT_MAJOR_VERSION = 1


android: include(/home/pronin-ao/openssl/android_openssl/openssl.pri)

linux: {
    FORMS += \
        admin.ui
    SOURCES += \
        admin.cpp
    HEADERS += \
        admin.h
    QTWEBENGINE_CHROMIUM_FLAGS=--ignore-gpu-blacklist
}




