#ifndef CLIENTCONTROLLER_H
#define CLIENTCONTROLLER_H

#include <QObject>
#include <QtNetwork>
#include <QTextEdit>
#include <clientconnectwindow.h>

class clientController : public QObject
{
    Q_OBJECT
public:
    explicit clientController(QObject *parent = 0);
    ~clientController();

signals:

public slots:
    void on_tryConnect(QString, QString, QString);
    void handleSSLError(QList<QSslError> errorList);
    void on_msgRcvd();
    void readyRead();

private:
    QString getCertificateString(const QSslCertificate &cert);
    QTextEdit certificateWindow;

    clientConnectWindow *m_w;
    QSslSocket *m_secureSocket;

    void displayCertificateWindow();
};

#endif // CLIENTCONTROLLER_H
