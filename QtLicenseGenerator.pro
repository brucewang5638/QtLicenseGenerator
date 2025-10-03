QT       += core gui widgets

TARGET = QtLicenseGenerator
TEMPLATE = app

# OpenSSL configuration
win32 {
    INCLUDEPATH += "C:/Program Files/OpenSSL/include"
    LIBS += -L"C:/Program Files/OpenSSL/lib" -llibssl -llibcrypto
} else {
    LIBS += -lssl -lcrypto
}

SOURCES += main.cpp \
           mainwindow.cpp \
           cryptomanager.cpp \
           hardwareinfo.cpp

HEADERS  += mainwindow.h \
            cryptomanager.h \
            hardwareinfo.h

FORMS    += mainwindow.ui

