#ifndef SSLSERVER_H
#define SSLSERVER_H

#include <QTcpServer>
#include <QSslSocket>
#include <QSslKey>
#include <QList>

class SSLServer : public QTcpServer
{
    Q_OBJECT

public:
    SSLServer(QObject *parent = 0);
    // Since we have a new version of incomingComing connection, we have
    // to keep track of the SSL sockets created and return them with a
    // a revised version of nextPendingConnection.
    QSslSocket *nextPendingConnection();
protected:
    // override of QTcpServer::incomingConnection(), see documentation 
    // for QSslSocket.  
    void incomingConnection(qintptr socketDescriptor);
private:
    // a list to keep track of the sockets that we have created
    QList<QSslSocket *> m_secureSocketList;
};

#endif // SSLSERVER_H
