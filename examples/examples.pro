TEMPLATE = app
TARGET = examples
DEPENDPATH += .
INCLUDEPATH += . ../libusc
CONFIG -= app_bundle
QT += widgets xml
# Input
HEADERS += MainWindow.h \
           QxChannelStatus.h \
           ConfigurationFile.h
FORMS += MainWindow.ui
SOURCES += main.cpp \
           MainWindow.cpp \
           QxChannelStatus.cpp \
           ConfigurationFile.cpp

macx {
    LIBS += -L/opt/local/lib -lusb-1.0 -L../libusc -lusc
    INCLUDEPATH += /opt/local/include/libusb-1.0
}
