QT       += core gui network
TARGET = tftpclient

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    clientsocket.cpp \
    main.cpp \
    tftpclient.cpp \
    tftpclientwidget.cpp

HEADERS += \
    clientsocket.h \
    tftpclient.h \
    tftpclientwidget.h

FORMS += \
    tftpclientwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../lib -ltftp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../lib -ltftpd

INCLUDEPATH += $$PWD/../core
DEPENDPATH += $$PWD/../core
RC_ICONS = client.ico
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/libtftp.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/libtftpd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/tftp.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/tftpd.lib
