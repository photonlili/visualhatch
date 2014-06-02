#include "messagereader.h"
#include <QDebug>

extern User user;
extern QList<PORT_COLOR_INFO> portColorList;

MessageReader::MessageReader(QObject *parent):QObject(parent)
{

}



Reader4100::Reader4100(QObject *parent):MessageReader(parent)
{
    m_type = ENUM_ANSWER_RDT_PARAM_4100;
}

void Reader4100::read(QByteArray *data)
{
    //RDTID*0*CHEKind*RDTType*RDTComMode*STATUS_INFO*RDTParameter*OnSelectedWIRefNo * RDTStatus*CheBit*OperatingMode*hosttime

    //qDebug() << "Msg content:" << QString(*data);
    QString msg = QString(*data);
    QStringList msgList = msg.split("*");
    if( msgList.at(1) == "4001")
    {
        qDebug() << "Error: " << msg;
    }
    else
    {
        qDebug() << "Success: " << msg;
    }
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
        //RDTID*0*LOGINTYPE*USERID*PASSWORD
        qDebug() << "Success: 4101"<< msg;
        user.rdtId = msgList.at(0);
        user.userId = msgList.at(3);
    }
}

Reader4110::Reader4110(QObject *parent) : MessageReader(parent)
{
    m_type = ENUM_ANSWER_RDT_POINT_AND_VESSEL_LOGIN_4110;
}

void Reader4110::read(QByteArray *data)
{
    QString msg = QString(*data);
    QStringList msgList = msg.split("*");

    if( msgList.at(1) != "0")
    {
        qDebug() << "Error: " << msg;
    }
    else
    {
        /// RDTID*0*POW*VESSEL*CRANEID*BUNDLEID
        qDebug() << "Success: 4110 " << msg;
        user.powName = msgList.at(2);
        user.powName = msgList.at(4);
        user.craneId = msgList.at(5);
        user.bundleId = msgList.at(6);
    }
}

void Reader4130::read(QByteArray *data)
{
    QString msg = QString(*data);
    QStringList msgList = msg.split("*");

    //RDTID*VesselClass*RHUserName*CRUserName*BDUserName*portcolorInfo*
    user.vesselRef = msgList.at(1);
    user.userName = msgList.at(2);
    user.craneName = msgList.at(3);
    user.bundleName = msgList.at(4);
    QString colorInfo = msgList.at(5);

    int portNum = colorInfo.left(2).toInt();
    qDebug() << "port num:" << portNum;

    for( int i = 0; i < portNum; i++ )
    {
        PORT_COLOR_INFO info;
        info.port = colorInfo.mid(2+12*i,3);
        info.red = colorInfo.mid(5+12*i,3).toInt();
        info.green = colorInfo.mid(8+12*i,3).toInt();
        info.blue = colorInfo.mid(11+12*i,3).toInt();
        portColorList.push_back(info);
    }

    qDebug() << "Success: 4130" << msg;

    foreach(PORT_COLOR_INFO info, portColorList)
    {
        qDebug() << info.port << info.red << info.green << info.blue;
    }

}
