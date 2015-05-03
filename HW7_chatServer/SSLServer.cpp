#include <QDebug>
#include "SSLServer.h"

// For the documentation: Suppose you create an SSL server class as a subclass
// of QTcpServer. You would override QTcpServer::incomingConnection() with 
// something like the example below, which first constructs an
// instance of QSslSocket and then calls setSocketDescriptor() to set 
// the new socket's descriptor to
// the existing one passed in. It then initiates the SSL handshake by calling
// startServerEncryption().

SSLServer::SSLServer(QObject *parent) : QTcpServer(parent)
{
	 qDebug() << "Constructing SSLServer Object";
}

void SSLServer::incomingConnection(qintptr socketDescriptor)
{
    // On an incoming connection we want
    // to create a new secure socket. 
    QSslSocket *secureSocket = new QSslSocket;

    // Add to list so that we can find it with
    // nextConnection
    m_clients.insert(secureSocket);

    // We need to read in the local certificate and 
    // and the private key that we generated 
    // with openssl.  Read the README to see
    // how these are generated.
    secureSocket->setLocalCertificate("cacert.pem");
    secureSocket->setPrivateKey("privkey.pem");

    // check that the certificate / private key are not null
    if (secureSocket->localCertificate().isNull()) {
       qDebug() << "WARNING: The local certificate appears to be null! ";
    }
    if (secureSocket->privateKey().isNull()) {
       qDebug() << "WARNING: The private key appears to be null! ";
    }

    // debug message on success
    qDebug() << "Created the SSL socket, Read local cert. / private key files";

    // From incoming connection we obtain the socket descriptor,
    // we associate this with our new SSL socket
    secureSocket->setSocketDescriptor(socketDescriptor);

    // Begin encryption.  Note from the documentation
    // all the key stuff must be done prior to doing this.
    secureSocket->startServerEncryption();
    qDebug() << "Started encryption for new secure socket";

    connect(secureSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(secureSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

/*QSslSocket *SSLServer::nextPendingConnection()
{
    QSslSocket *secureSocket = NULL;
    if (m_clients.isEmpty()) {
        qDebug() << "nextPendingConnection: ERROR: Why is this list empty??";
    } else {
        secureSocket = m_clients.last();
        m_clients.removeLast();
    }
    return secureSocket;
}*/

void SSLServer::readyRead()
{
    QSslSocket *client = (QSslSocket*)sender();
    while(client->canReadLine())
    {
        QString line = QString::fromUtf8(client->readLine()).trimmed();
        qDebug() << "Read line:" << line;

        QRegExp meRegex("^/me:(.*)$");

        if(meRegex.indexIn(line) != -1)
        {
            QString user = meRegex.cap(1);
            m_users[client] = user;
            foreach(QSslSocket *client, m_clients)
                client->write(QString("Server:" + user + " has joined.\n").toUtf8());
            sendUserList();
            emit newMessage(QString("Server:" + user + " has joined.\n"));
            emit updateClients(user);
        }
        else if(m_users.contains(client))
        {
            QString message = line;
            QString user = m_users[client];
            qDebug() << "User:" << user;
            qDebug() << "Message:" << message;

            foreach(QSslSocket *otherClient, m_clients)
                otherClient->write(QString(user + ":" + message + "\n").toUtf8());
            emit newMessage(QString(user + ":" + message + "\n"));
        }
        else
        {
            qWarning() << "Got bad message from client:" << client->peerAddress().toString() << line;
        }
    }
}

void SSLServer::disconnected()
{
    QSslSocket *client = (QSslSocket*)sender();
    qDebug() << "Client disconnected:" << client->peerAddress().toString();

    m_clients.remove(client);

    QString user = m_users[client];
    m_users.remove(client);

    sendUserList();
    foreach(QSslSocket *client, m_clients)
        client->write(QString("Server:" + user + " has left.\n").toUtf8());
    emit newMessage(QString("Server:" + user + " has left.\n").toUtf8());
    emit updateClients(user);
}

void SSLServer::sendUserList()
{
    QStringList userList;
    foreach(QString user, m_users.values())
        userList << user;

    foreach(QSslSocket *client, m_clients)
        client->write(QString("/m_users:" + userList.join(",") + "\n").toUtf8());
}

