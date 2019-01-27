#include "socket.h"

TcpClient::TcpClient(QObject *parent) :
    QObject(parent)
{
}
TcpClient::~TcpClient()
{
    /*if(socket != nullptr)
    {
        socket->close();
        delete socket;
    }*/
}

void TcpClient::doSend(const QString &hostIP, quint16 port, const QString &msg)
{
    socket = new QTcpSocket(this);
    message = msg;

    connect(socket, &QTcpSocket::connected,this, &TcpClient::connectedSuccess);
    connect(socket, &QTcpSocket::disconnected,this, &TcpClient::disconnected);
    connect(socket, &QTcpSocket::bytesWritten,this, &TcpClient::bytesWritten);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),this, &TcpClient::errorDetected);

    // this is not blocking call
    socket->connectToHost(hostIP, port);
    socket->waitForConnected(10000);

    // we need to wait...
    /*if(!socket->waitForConnected(10000))//10000
    {
        std::cerr << "Error: " << socket->errorString().toStdString() << std::endl;
        emit finished();
    }*/
}

void TcpClient::errorDetected()
{
    std::cerr << "Error: " << socket->errorString().toStdString() << std::endl;
    //emit finished();
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
    //emit finished();
}

void TcpClient::bytesWritten(qint64 bytes)
{
    //std::cout << bytes << " bytes written..."  << std::endl;
    socket->flush();
    socket->close();
}
