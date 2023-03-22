#ifndef UDPSOCKET_H
#define UDPSOCKET_H
#include "baseudp.h"
#include <QHostAddress>

class QUdpSocket;
class ClientSocket : public BaseUdp
{
public:
    ClientSocket(QUdpSocket* socket)
        : socket_(socket)
    {}

    uint32_t write(const char* data, size_t size) override;
private:
    QUdpSocket* socket_;
    QHostAddress host_;
    uint16_t port_;
};

#endif // UDPSOCKET_H
