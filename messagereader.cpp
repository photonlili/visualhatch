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

