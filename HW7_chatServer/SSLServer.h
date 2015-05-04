#ifndef SSLSERVER_H
#define SSLSERVER_H

#include <QTcpServer>
#include <QObject>
#include <QSslSocket>
#include <QSslKey>
#include <QThread>
#include <QList>
#include <QSet>
#include <QMap>
#include <QString>

class SSLServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit SSLServer(QObject *parent = 0);
    // Since we have a new version of incomingComing connection, we have
    // to keep track of the SSL sockets created and return them with a
    // a revised version of nextPendingConnection.
    //QSslSocket *nextPendingConnection();
signals:
    void updateClients(QString);
    void newMessage(QString);
private slots:
    void readyRead();
    void disconnected();
    void sendUserList();
protected:
    // override of QTcpServer::incomingConnection(), see documentation 
    // for QSslSocket.  
    void incomingConnection(qintptr socketDescriptor);
private:
    // a list to keep track of the sockets that we have created
    QHash<QSslSocket *,QThread*> m_clients;
    QMap<QSslSocket*,QString> m_users;
};

#endif // SSLSERVER_H
