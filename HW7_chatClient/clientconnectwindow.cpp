#include "clientconnectwindow.h"
#include "ui_clientconnectwindow.h"
#include <QDebug>

clientConnectWindow::clientConnectWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::clientConnectWindow)
{
    ui->setupUi(this);
}

clientConnectWindow::~clientConnectWindow()
{
    delete ui;
}

void clientConnectWindow::on_pushButton_clicked()
{
    if(ui->lineEdit->text().trimmed().isEmpty())
        return;

    QString server, port, name;
    server = getServer();
    port = getPort();
    name = getUserName();

    //if(name.isEmpty())
      //  name = A LOCAL ADDRESS

    emit tryConnect(getServer(), getPort(), getUserName());
}

void clientConnectWindow::on_lineEdit_returnPressed()
{
    on_pushButton_clicked();
}

void clientConnectWindow::on_lineEdit_2_returnPressed()
{
    on_pushButton_clicked();
}

void clientConnectWindow::on_lineEdit_3_returnPressed()
{
    on_pushButton_clicked();
}

void clientConnectWindow::on_lineEdit_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->pushButton->setEnabled(false);
    else
        ui->pushButton->setEnabled(true);
}


QString clientConnectWindow::getServer()
{
    return ui->lineEdit->text().trimmed();
}

QString clientConnectWindow::getPort()
{
    return ui->lineEdit_2->text().trimmed();
}

QString clientConnectWindow::getUserName()
{
    return ui->lineEdit_3->text().trimmed();
}
