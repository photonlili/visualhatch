#include "messagereader.h"
#include <QDebug>

extern User user;
extern QList<PORT_COLOR_INFO> portColorList;
extern QList<CtnInfo> ctnList;

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
//        qDebug() << "Success: " << msg;
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
//        qDebug() << "Success: 4101"<< msg;
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
    //qDebug() << msgList;

    if( msgList.at(1) != "0")
    {
        qDebug() << "Error: " << msg;
    }
    else
    {
        /// RDTID*0*POW*VESSEL*CRANEID*BUNDLEID
 //       qDebug() << "Success: 4110 " << msg;
        user.powName = msgList.at(2);
        user.vesselRef = msgList.at(3);
        user.craneId = msgList.at(4);
        user.bundleId = msgList.at(5);
    }
}

void Reader4130::read(QByteArray *data)
{
    QString msg = QString(*data);
    QStringList msgList = msg.split("*");

    //RDTID*VesselClass*RHUserName*CRUserName*BDUserName*portcolorInfo*
    user.vesselClass = msgList.at(1);
    user.userName = msgList.at(2);
    user.craneName = msgList.at(3);
    user.bundleName = msgList.at(4);
    QString colorInfo = msgList.at(5);

    int portNum = colorInfo.left(2).toInt();
    //qDebug() << "port num:" << portNum;

    for( int i = 0; i < portNum; i++ )
    {
        PORT_COLOR_INFO info;
        info.port = colorInfo.mid(2+12*i,3);
        info.red = colorInfo.mid(5+12*i,3).toInt();
        info.green = colorInfo.mid(8+12*i,3).toInt();
        info.blue = colorInfo.mid(11+12*i,3).toInt();
        portColorList.push_back(info);
    }

//    qDebug() << "Success: 4130" << msg;

    foreach(PORT_COLOR_INFO info, portColorList)
    {
    //    qDebug() << info.port << info.red << info.green << info.blue;
    }

}

void Reader4167::read(QByteArray *data)
{
    QString msg = QString(*data);
    QStringList msgList = msg.split("*");
    //qDebug() << msg;

    if( msgList.at(1) != "0")
    {
        qDebug() << "Error: " << msg;
    }
    else
    {
        //success

        int ctnCount = msgList.at(5).toInt();
        msgList.pop_front(); //0
        msgList.pop_front(); //1
        msgList.pop_front(); //2
        msgList.pop_front(); //3
        msgList.pop_front(); //4
        msgList.pop_front(); //5

        static bool readNew = false;
        if( readNew )
        {
            ctnList.clear();
            readNew = false;
        }

        for( int i = 0; i < ctnCount; i++ )
        {
            readContainer(msgList.at(i));
        }

        if( msgList.last() == QString("1") )
            readNew = true;
        //qDebug() << msgList;
    }

    qDebug() << ctnList.size();
}

void Reader4167::readContainer( const QString &data )
{
    //qDebug() << data;
    CtnInfo ctn;

    //qDebug() << "Ctn size: " << data.size();
    ctn.ContainerID = data.mid( 0, 12 );
    ctn.EquipType = data.mid( 12, 4 );

    if( data.at(16) == '0' )
    {
        ctn.BigCtn = 'N';
        ctn.HighCube = 'N';
    }
    else if( data.at(16) == '1' )
    {
        ctn.BigCtn = 'N';
        ctn.HighCube = 'Y';
    }
    else if( data.at(16) == '2' )
    {
        ctn.BigCtn = 'Y';
        ctn.HighCube = 'N';
    }
    else if( data.at(16) == '3' )
    {
        ctn.BigCtn = 'Y';
        ctn.HighCube = 'Y';
    }

    ctn.Category = data.at(17);

    ctn.Weight = data.mid(18,6).toInt();

    ctn.Status = data.at(24);

    ctn.RFFlag = data.at(25);

    QChar mp = data.at(26);
    int temp = data.mid( 27, 4 ).toInt()*10;
    if( mp == '+' ) ctn.ReeferTemp = temp;
    else if( mp == '-' ) ctn.ReeferTemp = -temp;
    else ctn.ReeferTemp = 0;

    ctn.TempUnit = data.at(31);

    ctn.HazardClasses = data.mid( 32, 4 );
    ctn.HazardFlag = 'N';
    if( ctn.HazardClasses.size() )
        ctn.HazardFlag = 'Y';

    ctn.ioOD_T = data.mid(36,4).toInt();
    ctn.ioOD_R = data.mid(40,4).toInt();
    ctn.ioOD_L = data.mid(44,4).toInt();
    ctn.ioOD_F = data.mid(48,4).toInt();
    ctn.ioOD_B = data.mid(52,4).toInt();

    ctn.ODFlag = 'N';
    if( ctn.ioOD_B != 0 ||
        ctn.ioOD_F != 0 ||
        ctn.ioOD_L != 0 ||
        ctn.ioOD_R != 0 ||
        ctn.ioOD_T != 0 )
        ctn.ODFlag = 'Y';

    ctn.DMFlag = data.at(56);

    ctn.DschPort = data.mid(57,3);
    ctn.LoadPort = data.mid(60,3);
    ctn.LineOperator = data.mid(63,3);
    ctn.Mode = data.at(66);
    ctn.CurrentLocType = data.at(67);
    ctn.CurrentPos = data.mid(68,12);
    ctn.FromLocType = data.at(80);
    ctn.FromPos = data.mid(81,12);
    ctn.ToLocType = data.at(93);
    ctn.ToPos = data.mid(94,12);

    QString twin = data.mid(106,12);
    if(twin.at(0) != '%')
    {
        ctn.TwinCtnID = twin;
    }
    else
        ctn.TwinCtnID = "";

    ctn.MidPosLocType = data.at(118);
    ctn.MidPos = data.mid(119,12);

    ctn.JobDone = data.at(131);

    ctn.GroupCode = data.mid(132,4);
    ctn.Orphan = data.at(136);

    ctn.CraneId = data.mid(137,8);
    ctn.Time = data.mid(145,16);

    ctn.DLFlag = data.at(161);

    ctnList.push_back(ctn);



   // qDebug() << ctn.ContainerID << ctn.EquipType << ctn.BigCtn << ctn.HighCube << ctn.Category << ctn.Weight
   //          << ctn.Status << ctn.RFFlag << ctn.ReeferTemp << ctn.TempUnit << ctn.HazardClasses << ctn.HazardFlag
   //          << ctn.ODFlag;


}
