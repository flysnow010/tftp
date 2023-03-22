#include "clientsocket.h"
#include <QUdpSocket>


uint32_t ClientSocket::write(const char* data, size_t size)
{
    return socket_->write(data, size);
}
