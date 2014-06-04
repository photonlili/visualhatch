#ifndef SLOTGROUP_H
#define SLOTGROUP_H

#include <QObject>
#include <QList>
#include <QString>
#include "slot.h"


class SlotGroup : public QObject
{
    Q_OBJECT
public:
    explicit SlotGroup(QObject *parent = 0);


    QString vesselRef;
    unsigned short bayNo;
    unsigned short bayLayer;

    QList<QList<Slot*>> columns;

signals:

public slots:

};

#endif // SLOTGROUP_H
