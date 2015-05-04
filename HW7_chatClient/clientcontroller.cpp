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

    m_secureSocket->connectToHostEncrypted(server, port.toInt());

    if (!m_secureSocket->waitForEncrypted()){
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
