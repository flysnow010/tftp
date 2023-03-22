#include "tftpclientwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TFtpClientWidget w;
    w.show();
    return a.exec();
}
