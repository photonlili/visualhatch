#ifndef SLOT_H
#define SLOT_H

#include <QObject>

class Slot : public QObject
{
    Q_OBJECT
    Q_PROPERTY( bool visiable)
public:
    explicit Slot(bool visiable = false, QObject *parent = 0);

    bool isVisiable;

signals:

public slots:

};


#endif // SLOT_H
