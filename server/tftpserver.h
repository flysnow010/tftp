#ifndef TFTPSERVER_H
#define TFTPSERVER_H
#include "tftpserverfile.h"
#include <QObject>

class QUdpSocket;
class TFtpServer:  public QObject
{
Q_OBJECT
public:
    TFtpServer(QObject *parent = nullptr);

    void setFilePath(QString const& filePath);
    void start();
    void stop();
signals:
    void bindError();
    void startFile(QString const&transferId, QString const& fileName);
    void progress(QString const&transferId, quint64 bytes, quint64 total);
    void statusText(QString const& text);
    void stopFile(QString const&transferId);

private slots:
    void readPendingDatagrams();
private:

private:
    QUdpSocket* socket;
    QString filePath_;
    TFtpFileManager::Ptr fileManager_;
    const uint16_t TFTP_PORT = 69;
};

#endif // TFTPSERVER_H
