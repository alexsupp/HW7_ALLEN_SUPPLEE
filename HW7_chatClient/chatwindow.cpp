#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <QString>
#include <QCloseEvent>

chatWindow::chatWindow(QString name, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::chatWindow)
{
    ui->setupUi(this);
    m_fromUser = name;
    this->setWindowTitle("Chat with " + m_fromUser);
    m_isDC = false;
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

void chatWindow::on_rcvDisconnect(QString msg)
{
    this->setMessage(msg);
    ui->lineEditMessage->setEnabled(false);
    ui->sendButton->setEnabled(false);
    m_isDC=true;
}

void chatWindow::on_lineEditMessage_textChanged(const QString &arg1)
{
    ui->msgSizeLabel->setText(QString::number(arg1.size())+"/255");
}

void chatWindow::setMessage(QString msg)
{
    ui->textEditMessages->append(msg);
}

void chatWindow::closeEvent(QCloseEvent *e)
{
    if(!m_isDC){
        QString msg = "Chat session disconnected.";
        emit tryDisconnect(m_fromUser, msg.trimmed());
        ui->lineEditMessage->setEnabled(false);
        ui->sendButton->setEnabled(false);
        m_isDC = true;
        setMessage(msg.trimmed());
        e->ignore();
    }
    else{
        this->deleteLater();
        e->accept();
    }
}
