#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>

namespace Ui {
class chatWindow;
}

class chatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit chatWindow(QString name, QWidget *parent = 0);
    ~chatWindow();
    void setMessage(QString msg);

public slots:
    void on_rcvDisconnect(QString msg);

signals:
    void sendMessage(QString, QString);
    void tryDisconnect(QString, QString);

private slots:
    void on_sendButton_clicked();
    void on_lineEditMessage_textChanged(const QString &arg1);
    void closeEvent(QCloseEvent *e);

private:
    Ui::chatWindow *ui;
    QString m_fromUser;
    bool m_isDC;
};

#endif // CHATWINDOW_H
