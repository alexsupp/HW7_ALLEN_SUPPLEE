#ifndef CLIENTCONTROLLER_H
#define CLIENTCONTROLLER_H

#include <QObject>
#include <QSet>
#include <QtNetwork>
#include <QTextEdit>
#include <clientconnectwindow.h>
#include "userlistwindow.h"
#include "chatwindow.h"

class clientController : public QObject
{
    Q_OBJECT
public:
    explicit clientController(QObject *parent = 0);
    ~clientController();

signals:
    void userListChanged(QStringList);
    void newMessage(QString, QString);
    void rcvDisconnect(QString, QString);

public slots:
    void on_tryConnect(QString, QString, QString);
    void handleSSLError(QList<QSslError> errorList);
    void on_tryStartChat(QString);
    void on_newMessage(QString, QString);
    void on_sendMessage(QString, QString);
    void on_rcvDisconnect(QString, QString);
    void on_tryDisconnect(QString, QString);
    void readyRead();

private:
    QString getCertificateString(const QSslCertificate &cert);
    QTextEdit certificateWindow;
    QString m_clientName;

    QMap<QString, chatWindow*> m_chatList;

    clientConnectWindow *m_w;
    userListWindow *m_userWindow;
    QSslSocket *m_secureSocket;
    chatWindow *m_chatWindow;

    void displayCertificateWindow();
};

#endif // CLIENTCONTROLLER_H
