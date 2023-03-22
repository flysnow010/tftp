#ifndef TFTPCLIENT_H
#define TFTPCLIENT_H
#include "tftpclientfile.h"
#include <QObject>
#include <QAbstractSocket>

class QUdpSocket;
class TFtpClient : public QObject
{
    Q_OBJECT
public:
    explicit TFtpClient(QObject *parent = nullptr);

    void setHostPort(QString const& host, quint16 port);
    void getFile(QString const& localFileName, QString const& remoteFileName);
    void putFile(QString const& localFileName, QString const& remoteFileName);

    bool isPut() const { return isPut_; }
public slots:
    void stopFileTransfer();
signals:
    void started();
    void progress(quint64 bytes, quint64 total);
    void error(QString const& text);
    void finished();
private slots:
    void connected();
    void connectError(QAbstractSocket::SocketError error);
    void readPendingDatagrams();
private:
    QUdpSocket* socket;
    QString host_;
    quint16 port_;
    QString localFileName_;
    QString remoteFileName_;
    TFtpClientFile::Ptr tftpFile_;
    bool isPut_ = false;
};

#endif // TFTPCLIENT_H
