#include "tftpserverwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("TFtpServer");
    QApplication::setApplicationVersion("V0.1.0");
    TFtpServerWidget w;
    w.show();
    return a.exec();
}
