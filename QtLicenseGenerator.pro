QT       += core gui widgets

TARGET = QtLicenseGenerator
TEMPLATE = app

# Static linking for Windows only (Qt on Linux typically uses shared libs)
win32 {
    CONFIG += static
    QMAKE_LFLAGS += -static -static-libgcc -static-libstdc++
}

# OpenSSL configuration
win32 {
    OPENSSL_PATH = $$(OPENSSL_ROOT_DIR)
    isEmpty(OPENSSL_PATH) {
        OPENSSL_PATH = "C:/Program Files/OpenSSL"
    }

    INCLUDEPATH += $$OPENSSL_PATH/include
    LIBS += -L$$OPENSSL_PATH/lib -llibssl -llibcrypto

    # Try alternative paths if not found
    !exists($$OPENSSL_PATH/lib/libssl.lib) {
        LIBS += -L$$OPENSSL_PATH/lib/VC/x64/MD -llibssl -llibcrypto
    }
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

