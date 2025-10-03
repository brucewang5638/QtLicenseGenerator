QT       += core gui widgets

TARGET = QtLicenseGenerator
TEMPLATE = app

# Static linking configuration
CONFIG += static staticlib
QMAKE_LFLAGS += -static

# Platform-specific static linking
win32 {
    QMAKE_LFLAGS += -static-libgcc -static-libstdc++
    # Import static Qt plugins
    QTPLUGIN += qwindows qwindowsvistastyle
}

unix:!mac {
    QMAKE_LFLAGS += -static-libgcc -static-libstdc++
    # Import static Qt plugins for Linux
    QTPLUGIN += qxcb
}

# OpenSSL static linking configuration
win32 {
    OPENSSL_PATH = $$(OPENSSL_ROOT_DIR)
    isEmpty(OPENSSL_PATH) {
        OPENSSL_PATH = "C:/Program Files/OpenSSL"
    }

    INCLUDEPATH += $$OPENSSL_PATH/include

    # Try static libraries first
    LIBS += -L$$OPENSSL_PATH/lib -llibssl_static -llibcrypto_static
    LIBS += -lws2_32 -lcrypt32 -ladvapi32 -luser32

    # Fallback to dynamic if static not available
    !exists($$OPENSSL_PATH/lib/libssl_static.lib) {
        LIBS += -L$$OPENSSL_PATH/lib -llibssl -llibcrypto
        !exists($$OPENSSL_PATH/lib/libssl.lib) {
            LIBS += -L$$OPENSSL_PATH/lib/VC/x64/MD -llibssl -llibcrypto
        }
    }
} else {
    # Linux: Use static OpenSSL if available
    LIBS += -Wl,-Bstatic -lssl -lcrypto -Wl,-Bdynamic
    LIBS += -ldl -lpthread
}

SOURCES += main.cpp \
           mainwindow.cpp \
           cryptomanager.cpp \
           hardwareinfo.cpp

HEADERS  += mainwindow.h \
            cryptomanager.h \
            hardwareinfo.h

FORMS    += mainwindow.ui

