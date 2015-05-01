#include "serverworker.h"

serverWorker::serverWorker(QSslSocket* clientConnection, QObject *parent) :
    QObject(parent)
{

    if (!clientConnection->waitForEncrypted(1000)){
        qDebug() << "Waited for 1 second for encryption handshake without success";
        return;
    }
    qDebug() << "Successfully waited for secure handshake...";
    //connect(clientConnection, SIGNAL(disconnected()),
    //        clientConnection, SLOT(deleteLater()));
    //dclientConnection->write(block);
    //clientConnection->disconnectFromHost();
}
