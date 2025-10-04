QT       += core gui widgets

TARGET = QtLicenseGenerator
TEMPLATE = app

# OpenSSL configuration
win32 {
    OPENSSL_ROOT = $$(OPENSSL_ROOT_DIR)
    isEmpty(OPENSSL_ROOT) {
        OPENSSL_ROOT = "C:/Program Files/OpenSSL"
    }
    INCLUDEPATH += $$OPENSSL_ROOT/include
    LIBS += -L$$OPENSSL_ROOT/lib -llibssl -llibcrypto
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

