#include "tftpclient.h"
#include "clientsocket.h"
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDebug>

TFtpClient::TFtpClient(QObject *parent)
    : QObject(parent)
    , socket(new QUdpSocket(this))
{
    connect(socket, &QUdpSocket::readyRead,
            this, &TFtpClient::readPendingDatagrams);
    connect(socket, &QUdpSocket::connected,
           this, &TFtpClient::connected);
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
           this, SLOT(connectError(QAbstractSocket::SocketError)));
}

void TFtpClient::setHostPort(QString const& host, quint16 port)
{
    host_ = host;
    port_ = port;
}

void TFtpClient::getFile(QString const& localFileName, QString const& remoteFileName)
{
    isPut_ = false;
    localFileName_ = localFileName;
    remoteFileName_ = remoteFileName;
    socket->connectToHost(host_, port_);
}

void TFtpClient::putFile(QString const& localFileName, QString const& remoteFileName)
{
    isPut_ = true;
    localFileName_ = localFileName;
    remoteFileName_ = remoteFileName;
    socket->connectToHost(host_, port_);
}

void TFtpClient::stopFileTransfer()
{
    socket->disconnectFromHost();
}

void TFtpClient::connected()
{
    ClientSocket* udp = new ClientSocket(socket);
    tftpFile_ = TFtpClientFile::Ptr(new TFtpClientFile(udp));
    bool isOK = true;
    if(isPut_)
        isOK = tftpFile_->putFile(localFileName_.toStdString(),
                           remoteFileName_.toStdString(), TFtp::BINARY);
    else
        isOK = tftpFile_->getFile(localFileName_.toStdString(),
                           remoteFileName_.toStdString(), TFtp::BINARY);
    if(!isOK)
        emit error("Local File not Found");
    else
        emit started();
}

void TFtpClient::connectError(QAbstractSocket::SocketError)
{
    emit error("Connect host is failed");
}

void TFtpClient::readPendingDatagrams()
{
    while (socket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = socket->receiveDatagram();
        QByteArray d = datagram.data();
        if(tftpFile_)
        {
            tftpFile_->process((uint8_t *)d.data(), d.size());
            emit progress(tftpFile_->file_bytes(), tftpFile_->filesize());
            if(tftpFile_->is_finished())
            {
                if(tftpFile_->is_error())
                    emit error(QString::fromStdString(tftpFile_->error_msg()));
                else
                    emit finished();
            }
        }
    }
}
