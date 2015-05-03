#include "chatwindow.h"
#include "clientconnectwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    clientConnectWindow w;
    w.show();

    return a.exec();
}
