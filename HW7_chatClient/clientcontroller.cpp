#include "clientcontroller.h"
#include <QtNetwork>
#include <QMessageBox>


clientController::clientController(QObject *parent) : QObject(parent)
{
    m_w = new clientConnectWindow();
    connect(m_w, SIGNAL(tryConnect(QString,QString,QString)), this, SLOT(on_tryConnect(QString,QString,QString)));

    m_w->show();
}

clientController::~clientController()
{

}


void clientController::on_tryConnect(QString server, QString port, QString name)
{
    m_secureSocket = new QSslSocket(this);


    // special slot to handle errors with the certificates
    // in particular the fact that they are self-signed
    connect(m_secureSocket, SIGNAL(sslErrors(QList<QSslError>)), this,
            SLOT(handleSSLError(QList<QSslError>)));
    connect(m_secureSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    m_secureSocket->connectToHostEncrypted(server, port.toInt());

    if (!m_secureSocket->waitForEncrypted(3000)){
        QMessageBox::critical(m_w, "ERROR", "Error: Couldn't connect to host");
        return;
    }
    displayCertificateWindow();

    if(name.isEmpty())
        name=m_secureSocket->peerAddress().toString();

    QString msg = "0\n" + name + "\n";
    m_secureSocket->write(msg.toUtf8());
    //QSslCertificate peerCertificate = secureSocket->peerCertificate();
    //qDebug() << "Peer Certificate is: " << endl;
    //qDebug() << peerCertificate << endl;
    //QSslCertificate localCertificate = secureSocket->localCertificate();
    //qDebug() << "Local Certificate is: " << endl;
    //qDebug() << localCertificate << endl;
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


void clientController::on_msgRcvd()
{

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
        int state = QString::fromUtf8(client->readLine()).trimmed().toInt(&isValid,10);
        if (!isValid){
            qDebug() << "first readline is not a number";
        }
        QString toUser;
        QString fromUser;
        QString msg;
        switch (state){
        case 0: // new client joining
            /*toUser = client->readLine().trimmed();
            emit updateClients(toUser);
            m_users[client] = toUser;
            emit newMessage(QString(toUser + " has joined."));
            sendUserList();*/
            break;
        case 1: // send username list
            break;
        case 2: // message
            toUser = client->readLine().trimmed();
            msg = client->readAll().trimmed();
            fromUser = m_users.value(client);
            emit newMessage(QString(fromUser+"->"+toUser+": "+msg));
            m_users.key(toUser)->write(QString(fromUser+": "+msg).toUtf8());
            break;
        case 3: // disconnect
            toUser = client->readLine().trimmed();
            fromUser = m_users[client];
            //msg = client->readAll().trimmed();
            m_users.key(toUser)->write(QString(fromUser+" has disconnected!").toUtf8());
            break;
        default:
            qWarning() << "Got bad message from client:" << client->peerAddress().toString() << client->readAll();
        }
    }
}
