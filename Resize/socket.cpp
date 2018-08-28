#include "socket.h"

TcpClient::TcpClient(QObject *parent) :
    QObject(parent)
{
}
TcpClient::~TcpClient()
{
    if(socket != nullptr)
    {
        socket->close();
        delete socket;
    }
}

void TcpClient::doSend(const QString &hostIP, quint16 port, const QString &msg)
{
    socket = new QTcpSocket(this);
    message = msg;

    connect(socket, SIGNAL(connected()),this, SLOT(connectedSuccess()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));

    // this is not blocking call
    socket->connectToHost(hostIP, port);

    // we need to wait...
    if(socket->waitForConnected(10000))
    {
        std::cerr << "Error: " << socket->errorString().toStdString() << std::endl;
    }
}

void TcpClient::connectedSuccess()
{
    std::cout << "Connected..." << std::endl;
    socket->write(message.toStdString().c_str());
    socket->waitForBytesWritten(5000);
}

void TcpClient::disconnected()
{
    std::cout << "Disconnected..." << std::endl;
    socket->flush();
    socket->close();
}

void TcpClient::bytesWritten(qint64 bytes)
{
    //std::cout << bytes << " bytes written..."  << std::endl;
    socket->flush();
    socket->close();
}
