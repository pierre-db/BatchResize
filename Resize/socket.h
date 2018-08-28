#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <iostream>
#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QAbstractSocket>

#define DEFAULT_PORT 5002

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

#endif
