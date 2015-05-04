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
    QThread *thread = new QThread();

    // Add to list so that we can find it with
    // nextConnection
    m_clients.insert(secureSocket,thread);

    // We need to read in the local certificate and 
    // and the private key that we generated 
    // with openssl.  Read the README to see
    // how these are generated.
    secureSocket->setLocalCertificate("cacert.pem");
    secureSocket->setPrivateKey("privkey.pem");

    // check that the certificate / private key are not null
    if (secureSocket->localCertificate().isNull()) {
       qDebug() << "WARNING: The local certificate appears to be null!\n"<<secureSocket->privateKey().type();
    }
    if (secureSocket->privateKey().isNull()) {
       qDebug() << "WARNING: The private key appears to be null!\n"<<secureSocket->privateKey().type();
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

    qDebug() << "New client from:" << secureSocket->peerAddress().toString();

    connect(secureSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(secureSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    secureSocket->moveToThread(thread);
    thread->start();

}

void SSLServer::readyRead()
{
    qDebug() << "in readyRead";
    QSslSocket *client = (QSslSocket*)sender();
    while(client->canReadLine())
    {
        //qDebug() << client->readAll();fromUtf8(client->readLine()).trimmed()
        bool isValid = false;
        int state = QString::fromUtf8(client->readLine()).trimmed().toInt(&isValid,10);
        if (!isValid){
            qDebug() << "first readline is not a number";
        }
        QString toUser;
        QString fromUser;
        QString msg;
        switch (state){
        case 0: // new client joining
            toUser = client->readLine().trimmed();
            if (m_usernames.contains(toUser)){
                client->write("4\nUsername already in use!\n");
                qDebug() << "Username already in use!";
                return;
            }
            client->write("0\n");
            emit updateClients(toUser);
            m_users[client] = toUser;
            m_usernames.insert(toUser);
            emit newMessage(QString(toUser + " has joined."));
            sendUserList();
            break;
        case 1: // send username list
            sendUserList();
            break;
        case 2: // message
            toUser = client->readLine().trimmed();
            msg = client->readAll().trimmed();
            fromUser = m_users.value(client);
            emit newMessage(QString(fromUser+"->"+toUser+": "+msg));
            m_users.key(toUser)->write(QString("2\n"+fromUser+"\n"+msg+'\n').toUtf8());
            break;
        case 3: // disconnect
            toUser = client->readLine().trimmed();
            fromUser = m_users[client];
            //msg = client->readAll().trimmed();
            m_users.key(toUser)->write(QString("3\n"+fromUser+" has disconnected!\n").toUtf8());
            break;
        default:
            qWarning() << "Got bad message from client:" << client->peerAddress().toString() << client->readAll();
        }
    }
}

void SSLServer::disconnected()
{
    QSslSocket *client = (QSslSocket*)sender();
    qDebug() << "Client disconnected:" << client->peerAddress().toString();

    QThread *thread = m_clients[client];
    m_clients.remove(client);

    QString user = m_users[client];
    m_usernames.remove(user);
    m_users.remove(client);

    sendUserList();
    emit newMessage(QString(user + " has left.\n").toUtf8());
    emit updateClients(user);
    client->deleteLater();
}

void SSLServer::sendUserList()
{
    QStringList userList;
    foreach(QString user, m_users.values())
        userList << user.trimmed();

    foreach(QSslSocket *client, m_clients.keys()){
        QString msg = "1\n";
        msg.append(userList.join(","));
        msg.append("\n");
        //QString("1" +'\n' + userList.join(",") + "\n");
        qDebug() << "SERVER MSG: " << msg << "\n";
        client->write(msg.toUtf8());
    }
}

