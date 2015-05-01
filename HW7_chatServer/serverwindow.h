#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <SSLServer.h>
#include <QMessageBox>
#include <QList>
#include "serverworker.h"

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
    void createThread();

private:
    Ui::ServerWindow *ui;
    SSLServer *m_sslServer;
    QList<serverWorker*> m_workers;
    QList<QThread*> m_threads;
};

#endif // SERVERWINDOW_H
