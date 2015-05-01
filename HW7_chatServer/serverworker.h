#ifndef SERVERWORKER_H
#define SERVERWORKER_H

#include <QObject>

class serverWorker : public QObject
{
    Q_OBJECT
public:
    explicit serverWorker(QObject *parent = 0);

signals:

public slots:

};

#endif // SERVERWORKER_H
