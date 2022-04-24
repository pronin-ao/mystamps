TEMPLATE = app
TARGET = mystamps
INCLUDEPATH += .

QT += widgets core qml network webview

# Input
SOURCES += main.cpp \
    catalogue.cpp \
    datamanager.cpp \
    dbmodel.cpp \
    dbparser.cpp \
    urlimagelabel.cpp

HEADERS += \
    catalogue.h \
    catalogue_constants.h \
    data_constants.h \
    datamanager.h \
    dbmodel.h \
    dbparser.h \
    stamp.h \
    urlimagelabel.h

RESOURCES += \
    res.qrc

ANDROID_API_VERSION = 28

QMAKE_CXXFLAGS += -std=c++17

CONFIG += qmltypes
QML_IMPORT_NAME = mystamps.data
QML_IMPORT_MAJOR_VERSION = 1

CONFIG += admin


android {
    include(/home/pronin-ao/openssl/android_openssl/openssl.pri)
    CONFIG -= admin
}

CONFIG(admin): {
    message(Building with admin.)
    FORMS += \
        admin.ui
    SOURCES += \
        delegates.cpp \
        adminmodel.cpp \
        adminproxymodel.cpp \
        admin.cpp
    HEADERS += \
        delegates.h \
        adminmodel.h \
        adminproxymodel.h \
        admin.h
    QTWEBENGINE_CHROMIUM_FLAGS=--ignore-gpu-blacklist
} else {
    message(Building without admin.)
}




