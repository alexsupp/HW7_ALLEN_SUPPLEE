#ifndef USERLISTWINDOW_H
#define USERLISTWINDOW_H

#include <QWidget>
#include <QListWidgetItem>
#include <QStringList>

namespace Ui {
class userListWindow;
}

class userListWindow : public QWidget
{
    Q_OBJECT

public:
    explicit userListWindow(QWidget *parent = 0);
    ~userListWindow();

public slots:
    void updateUserList(QStringList);

signals:
    void tryChatConnect(QString);

private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_pushButton_clicked();

private:
    Ui::userListWindow *ui;

};

#endif // USERLISTWINDOW_H
