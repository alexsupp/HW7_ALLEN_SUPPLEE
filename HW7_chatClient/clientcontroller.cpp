#include "clientcontroller.h"
#include <QtNetwork>
#include <QMessageBox>


clientController::clientController(QObject *parent) : QObject(parent)
{
    m_w = new clientConnectWindow();
    m_userWindow = new userListWindow();

    connect(m_w, SIGNAL(tryConnect(QString,QString,QString)), this, SLOT(on_tryConnect(QString,QString,QString)));


    connect(this, SIGNAL(userListChanged(QStringList)), m_userWindow, SLOT(updateUserList(QStringList)));
    connect(m_userWindow, SIGNAL(tryChatConnect(QString)), this, SLOT(on_tryStartChat(QString)));
    connect(this, SIGNAL(newMessage(QString,QString)), this, SLOT(on_newMessage(QString,QString)));
    connect(this, SIGNAL(rcvDisconnect(QString, QString)), this, SLOT(on_rcvDisconnect(QString,QString)));

    m_w->show();
}

clientController::~clientController()
{
    m_secureSocket->close();
}


void clientController::on_tryConnect(QString server, QString port, QString name)
{
    m_secureSocket = new QSslSocket(this);


    // special slot to handle errors with the certificates
    // in particular the fact that they are self-signed
    connect(m_secureSocket, SIGNAL(sslErrors(QList<QSslError>)), this,
            SLOT(handleSSLError(QList<QSslError>)));


    m_secureSocket->connectToHostEncrypted(server, port.toInt());

    if (!m_secureSocket->waitForEncrypted(3000)){
        QMessageBox::critical(m_w, "ERROR", "Error: Couldn't connect to host");
        return;
    }
    //displayCertificateWindow();

    if(name.isEmpty())
        name=m_secureSocket->peerAddress().toString();

    QString msg = "0\n" + name + "\n";
    m_secureSocket->write(msg.toUtf8());

    if(!m_secureSocket->waitForReadyRead(15000)){
        QMessageBox timeOut;
        timeOut.setText("The server timed out on this request try again");
        timeOut.exec();
        return;
    }

    //qDebug() << client->readAll();fromUtf8(client->readLine()).trimmed()
    bool isValid = false;
    int state = QString::fromUtf8(m_secureSocket->readLine()).trimmed().toInt(&isValid,10);
    if (!isValid){
        qDebug() << "first readline is not a number for the namecheck";
    }

    if(state == 4){//on error;
        QMessageBox dupName;
        dupName.setText(m_secureSocket->readLine().trimmed());
        dupName.exec();
        return;
    }

    else if(state == 0){//on success join
        m_w->close();

        m_secureSocket->flush();
        m_clientName = name;

        msg = m_secureSocket->readLine();
        if(!msg.trimmed().compare("1")){
            msg = m_secureSocket->readLine();
            QStringList names = msg.split(",");
            names.last().remove('\n');
            emit userListChanged(names);
        }

        connect(m_secureSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
        m_userWindow->setWindowTitle(name.trimmed() + "'s user list");
        m_userWindow->show();
    }

    else
        qDebug()<<"a bad state was received trying to connect\n";
}

void clientController::on_tryStartChat(QString name)
{
    QString msg = "2\n" + name + "\nNew chat with " + m_clientName + "\n";
    m_secureSocket->write(msg.trimmed().toUtf8());
    m_chatWindow = new chatWindow(name);

    m_chatList[name] = m_chatWindow;

    connect(m_chatWindow, SIGNAL(sendMessage(QString, QString)), this, SLOT(on_sendMessage(QString, QString)));
    connect(m_chatWindow, SIGNAL(tryDisconnect(QString,QString)), this, SLOT(on_tryDisconnect(QString, QString)));

    m_chatWindow->setMessage("New chat with " + name);

    m_chatWindow->show();
}

void clientController::on_newMessage(QString otherUser, QString msg)
{
    if(!m_chatList.contains(otherUser)){
        m_chatWindow = new chatWindow(otherUser);
        m_chatList[otherUser] = m_chatWindow;

        connect(m_chatWindow, SIGNAL(sendMessage(QString, QString)), this, SLOT(on_sendMessage(QString, QString)));
        connect(m_chatWindow, SIGNAL(tryDisconnect(QString,QString)), this, SLOT(on_tryDisconnect(QString, QString)));

        m_chatList[otherUser]->setMessage(msg);
    }

    else if(!msg.trimmed().isEmpty()){
        msg.prepend(otherUser + ": ");
        m_chatList[otherUser]->setMessage(msg);
    }

    if(m_chatWindow->isHidden())
        m_chatWindow->show();

}

void clientController::on_rcvDisconnect(QString otherUser, QString msg)
{
    m_chatWindow = m_chatList[otherUser];
    m_chatList.remove(otherUser);
    m_chatWindow->on_rcvDisconnect(msg);
}

void clientController::on_tryDisconnect(QString otherUser, QString msg)
{
    msg.prepend("3\n" + otherUser + "\n");
    m_secureSocket->write(msg.trimmed().toUtf8());
    m_chatList.remove(otherUser);
}

void clientController::on_sendMessage(QString otherUser, QString msg)
{
    msg.prepend("2\n" + otherUser + "\n");
    m_secureSocket->write(msg.trimmed().toUtf8());
}

QString clientController::getCertificateString(const QSslCertificate &cert)
{
    QString certInfo;
    certInfo += "Issuer Org: ";
    certInfo += cert.issuerInfo(QSslCertificate::Organization).join(" ") + QString("\n");
    certInfo += "Common Name: ";
    certInfo += cert.issuerInfo(QSslCertificate::CommonName).join(" ") + "\n";
    certInfo += "Effective Date: ";
    certInfo += cert.effectiveDate().toString() + "\n";
    certInfo += "Expiry Date: ";
    certInfo += cert.expiryDate().toString() + "\n";
    certInfo += "Public Key: ";
    certInfo += cert.publicKey().toPem() + "\n";
    certInfo += "Serial Number: ";
    certInfo += cert.serialNumber() + "\n";
    return certInfo;
}

void clientController::displayCertificateWindow()
{
    certificateWindow.setWindowTitle("Certificate Information");
    certificateWindow.setMinimumSize(400,300);
    QString peerCertificateInformation("Peer Certificate Information:\n");
    QSslCertificate peerCertificate = m_secureSocket->peerCertificate();
    peerCertificateInformation += getCertificateString(peerCertificate);
    certificateWindow.append(peerCertificateInformation);
    QString localCertificateInformation("Local Certificate Information:\n");
    QSslCertificate localCertificate = m_secureSocket->localCertificate();
    localCertificateInformation += getCertificateString(localCertificate);
    certificateWindow.append(localCertificateInformation);
    certificateWindow.show();
}

void clientController::handleSSLError(QList<QSslError> errorList)
{
    foreach ( QSslError error, errorList) {
       qDebug() << "Neet to handle SSL error:" << error;
    }
    m_secureSocket->ignoreSslErrors();
}

void clientController::readyRead()
{
    // We'll loop over every (complete) line of text that the server has sent us:
    while(m_secureSocket->canReadLine())
    {
        //qDebug() << client->readAll();fromUtf8(client->readLine()).trimmed()
        bool isValid = false;
        QString msg = QString::fromUtf8(m_secureSocket->readLine()).trimmed();
        qDebug() << "THIS IS THE MSG: " << msg << "\n";
        int state = msg.toInt(&isValid,10);
        if (!isValid){
            qDebug() << "first readline is not a number";

        }
        QString toUser;
        QString fromUser;
        QStringList names;
        switch (state){
        case 0: // new client joining
            /*toUser = client->readLine().trimmed();
            emit updateClients(toUser);
            m_users[client] = toUser;
            emit newMessage(QString(toUser + " has joined."));
            sendUserList();*/
            break;
        case 1: // receive username list
            msg = m_secureSocket->readLine();
            names = msg.split(",");
            names.last().remove('\n');
            emit userListChanged(names);
            break;
        case 2: // message
            fromUser = m_secureSocket->readLine().trimmed();
            msg = m_secureSocket->readAll().trimmed();
            emit newMessage(fromUser, msg);
            break;
        case 3: // disconnect
            fromUser = m_secureSocket->readLine().trimmed();
            //m_chatList.value(fromUser)->deleteLater();
            msg = m_secureSocket->readAll().trimmed();
            emit rcvDisconnect(fromUser, msg);
            //msg = "User " + fromUser + " has disconnected from this chat session";
            //emit a signal to show disconnect on window like connect
            //emit newMessage(fromUser, msg);
            break;
        default:
            qWarning() << "Got bad message from client:" << m_secureSocket->peerAddress().toString() << m_secureSocket->readAll();
        }
    }
}
