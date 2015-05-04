#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <QString>

chatWindow::chatWindow(QString name, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::chatWindow)
{
    ui->setupUi(this);
    m_fromUser = name;
    this->setWindowTitle("Chat with " + m_fromUser);
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
    emit sendMessage(m_fromUser, msg);
}

void chatWindow::on_lineEditMessage_textChanged(const QString &arg1)
{
    ui->msgSizeLabel->setText(QString::number(arg1.size())+"/255");
}

void chatWindow::setMessage(QString msg)
{
    ui->textEditMessages->append(msg);
}
