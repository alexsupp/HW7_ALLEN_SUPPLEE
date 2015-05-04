#include "chatwindow.h"
#include "clientcontroller.h"
#include "clientconnectwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    clientController c ;

    return a.exec();
}
