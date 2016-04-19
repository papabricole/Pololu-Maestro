TEMPLATE = lib
TARGET = usc
DEPENDPATH += .
INCLUDEPATH += .
CONFIG -= qt

# Input
HEADERS += libusc.h
SOURCES += libusc.c

macx {
    LIBS += -L/opt/local/lib -lusb-1.0
    INCLUDEPATH += /opt/local/include/libusb-1.0
}
