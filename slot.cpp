#include "slot.h"

Slot::Slot(bool visiable, QObject *parent) :
    QObject(parent)
{
    isVisiable = visiable;
}
