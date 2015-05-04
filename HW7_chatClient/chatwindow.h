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

signals:
    void sendMessage(QString, QString);

private slots:
    void on_sendButton_clicked();
    void on_lineEditMessage_textChanged(const QString &arg1);

private:
    Ui::chatWindow *ui;
    QString m_fromUser;
};

#endif // CHATWINDOW_H
