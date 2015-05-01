#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <QString>

chatWindow::chatWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::chatWindow)
{
    ui->setupUi(this);
}

chatWindow::~chatWindow()
{
    delete ui;
}

void chatWindow::on_sendButton_clicked()
{
    QString msg = ui->lineEditMessage->text();

    if(msg.isEmpty())
        return;

    ui->textEditMessages->append("you: " + msg);
    ui->lineEditMessage->clear();
}

void chatWindow::on_lineEditMessage_textChanged(const QString &arg1)
{
    ui->msgSizeLabel->setText(QString::number(arg1.size())+"/255");
}
