#ifndef TFTPSERVERWIDGET_H
#define TFTPSERVERWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class TFtpServerWidget; }
QT_END_NAMESPACE

class TFtpServer;
class TFtpServerWidget : public QWidget
{
    Q_OBJECT

public:
    TFtpServerWidget(QWidget *parent = nullptr);
    ~TFtpServerWidget();

private slots:
    void selectTFtpDir();
    void setCurrentDir(QString const& path);
    void onBindError();
    void onStartFile(QString const&transferId, QString const& fileName);
    void onProgress(QString const&transferId, quint64 bytes, quint64 total);
    void onStopFile(QString const&transferId);
private:
    void saveSettinggs();
    void loadSettings();
private:
    Ui::TFtpServerWidget *ui;
    TFtpServer* tftpServer;
};
#endif // TFTPSERVERWIDGET_H
