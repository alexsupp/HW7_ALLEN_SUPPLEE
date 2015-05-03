#ifndef CLIENTCONNECTWINDOW_H
#define CLIENTCONNECTWINDOW_H

#include <QWidget>
#include <QString>

namespace Ui {
class clientConnectWindow;
}

class clientConnectWindow : public QWidget
{
    Q_OBJECT

public:
    explicit clientConnectWindow(QWidget *parent = 0);
    QString getText();
    ~clientConnectWindow();

public slots:

signals:
    void tryConnect(QString, QString, QString);

private slots:
    void on_pushButton_clicked();
    void on_lineEdit_returnPressed();
    void on_lineEdit_textChanged(const QString &arg1);
    void on_lineEdit_2_returnPressed();
    void on_lineEdit_3_returnPressed();

private:
    QString getServer();
    QString getPort();
    QString getUserName();

    Ui::clientConnectWindow *ui;
};

#endif // CLIENTCONNECTWINDOW_H
