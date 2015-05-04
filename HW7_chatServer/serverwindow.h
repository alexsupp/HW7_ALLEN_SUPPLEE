#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <SSLServer.h>
#include <QMessageBox>
#include <QList>
#include <QSet>
#include <QString>
#include <QThread>

namespace Ui {
class ServerWindow;
}

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerWindow(QWidget *parent = 0);
    ~ServerWindow();

private slots:
    //void createThread();
    void updateClients(QString user);

private:
    Ui::ServerWindow *ui;
    SSLServer *m_sslServer;
    QSet<QString> m_users;
    QThread* m_sslThread;
    //QList<serverWorker*> m_workers;
    //QList<QThread*> m_threads;
};

#endif // SERVERWINDOW_H
