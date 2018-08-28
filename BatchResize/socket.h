#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <iostream>
#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QAbstractSocket>

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = 0);
    ~TcpClient();

    void doSend(const QString &hostIP, quint16 port, const QString &msg);

signals:

public slots:
    void connectedSuccess();
    void disconnected();
    void bytesWritten(qint64 bytes);
    //void readyRead();

private:
    QTcpSocket *socket;
    QString message;

};


class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);
    ~TcpServer();

    void startServer(const QString &ipConnect, quint16 port);

signals:
    void receivedSocket(const QString &line);

public slots:
    void newConnection();
    void readyRead();

private:
    QTcpServer *server;
    QTcpSocket *socket;
};

#endif
