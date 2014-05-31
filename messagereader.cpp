#include "messagereader.h"
#include <QDebug>

MessageReader::MessageReader(QObject *parent):QObject(parent)
{

}



Reader4100::Reader4100(QObject *parent):MessageReader(parent)
{
    m_type = ENUM_ANSWER_RDT_PARAM_4100;
}

void Reader4100::read(QByteArray *data)
{
    //qDebug() << "Msg content:" << QString(*data);
    QString msg = QString(*data);
    QStringList msgList = msg.split("*");
    if( msgList.at(1) == "4001")
    {
        qDebug() << "Error: " << msg;
    }
    else
        qDebug() << "Success: " << msg;
}

Reader4101::Reader4101(QObject *parent) : MessageReader(parent)
{
    m_type = ENUM_ANSWER_RDT_LOGIN_4101;
}

void Reader4101::read(QByteArray *data)
{
    QString msg = QString(*data);
    QStringList msgList = msg.split("*");
    //qDebug() << msgList;

    if( msgList.at(1) != "0" )
    {
        qDebug() << "Error: " << msg;
    }
    else
    {
        //qobject_cast<HatchMessenger*>(parent)->sendHeartBeat("RHX","RHX123");
        qDebug() << "Success: " << msg;

    }
}

