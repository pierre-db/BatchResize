#include "socket.h"

///////////////////////
//Client
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

/////////////////////////////
//Server
TcpServer::TcpServer(QObject *parent) :
    QObject(parent)
{
    socket = nullptr; // need to explicitly declare it to avoid crash on destruction
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

TcpServer::~TcpServer()
{
    if(socket != nullptr)
    {
        //socket->close();//necessary?
        delete socket;
    }
    if(server != nullptr)
    {
        //server->close();
        delete server;
    }
}

void TcpServer::startServer(const QString &ipConnect, quint16 port)
{
    QHostAddress ipAddress;
    ipAddress.setAddress(ipConnect);//QHostAddress::LocalHost

    if(!server->listen(ipAddress, port))
    {
        std::cerr << "Server could not start!" << std::endl;
        emit serverFailStart();
    }
    else
    {
        std::cout << "Server on" << std::endl;
    }
}

void TcpServer::restartServer(const QString &ipConnect, quint16 port)
{
    server->close();
    if(socket != nullptr)
    {
        socket->flush();
        socket->close();
    }
    QHostAddress ipAddress;
    ipAddress.setAddress(ipConnect);//QHostAddress::LocalHost

    if(!server->listen(ipAddress, port))
    {
        std::cerr << "Server could not restart!" << std::endl;
        emit serverFailStart();
    }
    else
    {
        std::cout << "Server restarted" << std::endl;
    }
}

void TcpServer::newConnection()
{

    socket = server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
    socket->waitForReadyRead(1000);

   //Inline version
   /* QTcpSocket *socket = server->nextPendingConnection();
    QString line;

        socket->waitForReadyRead(1000);
        line =  socket->readAll();
        std::cout << "Received: " << line.toStdString() << std::endl;// readLine().toStdString().c_str() << std::endl;
        socket->close();
        model->appendRow(new QStandardItem(QIcon(ICON_WAIT_PATH), line.toStdString().c_str()));

        socket->close();*/
}

void TcpServer::readyRead()
{

        if(socket->bytesAvailable() == 0)
        {
             std::cout << "Empty socket " << std::endl;// readLine().toStdString().c_str() << std::endl;
             socket->close();
             return;
        }
        else
        {
            QString line;
            line =  socket->readAll();
            socket->close();
            emit receivedSocket(line);
        }

}
