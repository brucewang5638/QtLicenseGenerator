QT       += core gui widgets

TARGET = QtLicenseGenerator
TEMPLATE = app

# Add OpenSSL linking for Linux
LIBS += -lssl -lcrypto

SOURCES += main.cpp \
           mainwindow.cpp \
           cryptomanager.cpp \
           hardwareinfo.cpp

HEADERS  += mainwindow.h \
            cryptomanager.h \
            hardwareinfo.h

FORMS    += mainwindow.ui

