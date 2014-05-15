#ifndef HATCHMESSENGER_H
#define HATCHMESSENGER_H

#include <QObject>

class HatchMessenger : public QObject
{
    Q_OBJECT
public:
    explicit HatchMessenger(QObject *parent = 0);

signals:

public slots:

};

#endif // HATCHMESSENGER_H
