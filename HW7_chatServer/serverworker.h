#ifndef SERVERWORKER_H
#define SERVERWORKER_H

#include <QObject>
#include <QDebug>
#include <QSslSocket>

class serverWorker : public QObject
{
    Q_OBJECT
public:
    explicit serverWorker(QSslSocket *clientConnection, QObject *parent = 0);

signals:

public slots:

};

#endif // SERVERWORKER_H
