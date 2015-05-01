#include "serverwindow.h"
#include "ui_serverwindow.h"
#include <QtNetwork>
#include "stdlib.h"
#include <iostream>
#include <QThread>
#include <QDebug>
#include "serverworker.h"

ServerWindow::ServerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerWindow)
{
    ui->setupUi(this);
    m_sslServer = new SSLServer(this);
    if (!m_sslServer->listen()) {
        QMessageBox::critical(this, tr("chatServer"),
                              tr("Unable to start the server: %1.")
                              .arg(m_sslServer->errorString()));
        close();
        return;
    }
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    ui->statusLabel->setText(tr("The server is running on\nIP: %1\nport: %2")
                         .arg(ipAddress).arg(m_sslServer->serverPort()));
    connect(ui->quitButton, SIGNAL(triggered()), this, SLOT(close()));
    connect(m_sslServer, SIGNAL(newConnection()), this, SLOT(createThread()));
}

ServerWindow::~ServerWindow()
{
    delete ui;
}

void ServerWindow::createThread(){
    QThread *thread = new QThread();
    QSslSocket *clientConnection = m_sslServer->nextPendingConnection();
    serverWorker *worker = new serverWorker(clientConnection);
    worker->moveToThread(thread);
    thread->start();
    m_threads.append(thread);
    m_workers.append(worker);

}
