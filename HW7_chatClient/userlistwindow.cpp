#include "userlistwindow.h"
#include "ui_userlistwindow.h"

userListWindow::userListWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::userListWindow)
{
    ui->setupUi(this);
}

userListWindow::~userListWindow()
{
    delete ui;
}

void userListWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    emit tryChatConnect(item->text());
}
void userListWindow::on_pushButton_clicked()
{
    on_listWidget_itemDoubleClicked(ui->listWidget->currentItem());
}

void userListWindow::updateUserList(QStringList l){
    ui->listWidget->clear();
    ui->listWidget->addItems(l);
}
