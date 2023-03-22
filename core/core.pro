QT -= gui
QT += network

TEMPLATE = lib
DESTDIR = ../../lib
CONFIG += staticlib

CONFIG(debug, debug|release) {
    TARGET = tftpd
}
else {
    TARGET = tftp
}

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    baseudp.cpp \
    tftp.cpp \
    tftpclientfile.cpp \
    tftpserverfile.cpp

HEADERS += \
    baseudp.h \
    tftp.h \
    tftpclientfile.h \
    tftpserverfile.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
