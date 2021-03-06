#include "hatchmessenger.h"

#include <QDebug>
#include <QTimer>
#include <QVariant>
#include <QThread>
#include <QString>
#include <QList>
#include <QFile>
#include <QDir>

#include "slotgroup.h"



//const QString HOST = "192.168.0.118";
const QString HOST = "192.168.1.4";


QList<CtnInfo> ctnList;
QList<SlotGroup*> slotGroups;

User user;

QList<PORT_COLOR_INFO> portColorList;
CShipData shipData;

HatchMessenger::HatchMessenger(QObject *parent) :
    QObject(parent)
{
    client = new QTcpSocket(this);
    client->setReadBufferSize(0);

    user.sharedPath = "/Users/lixu/Downloads/ntos/NGB-data/NGB_Shared/";

    initReaderList();


    connect(client, &QTcpSocket::readyRead, this, &HatchMessenger::recvMessage);
    connect(client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionError(QAbstractSocket::SocketError)));
    connect(client, &QAbstractSocket::stateChanged,this, &HatchMessenger::stateChanged);

    connect(client, &QTcpSocket::disconnected, this, &HatchMessenger::connectToHost);


    connect(client, &QAbstractSocket::connected, [=](){

        //sendMessage4000("rh01");
        //sendMessage4069();
        //sendMessage4067();
        //sendHeartBeat();
        sendHeartBeat("RHX","RHX123");
    });

    connectToHost();

    //loadShipData(&shipData, getShipFileName(QString("35TJ")));
    //loadSlotGroups();

}

void HatchMessenger::initReaderList()
{
    readerList.push_back(new Reader4100);
    readerList.push_back(new Reader4101);
    readerList.push_back(new Reader4110);
    readerList.push_back(new Reader4130);
    readerList.push_back(new Reader4167);
}

bool HatchMessenger::splitMessage(QByteArray *data)
{
    QDataStream msgStream(data,QIODevice::ReadWrite);
    msgStream.setByteOrder(QDataStream::LittleEndian);

    //char end = 255;

    EC_HEADER header = {0,0,0,0};
    msgStream >> header.load_len;
    msgStream >> header.msgType;
    msgStream >> header.msgId;
    msgStream >> header.sevId;

    QByteArray msgBodyArray = data->remove(0,sizeof(EC_HEADER));
    msgBodyArray = msgBodyArray.left(msgBodyArray.count()-1);

    qDebug() << "Reader Msg type:" << header.msgType;

    foreach(MessageReader* reader, readerList)
    {
        if( reader->getMsgType() == header.msgType )
            reader->read(&msgBodyArray);
    }

    return false;
}

void HatchMessenger::recvMessage()
{
    QByteArray data = client->readAll();
    QList<QByteArray> dataList = data.split(0xff);

    static bool splitted = false;

    qDebug() << "----------------------------------------------";
    qDebug() << "datalist.size(): " << dataList.size();

    if( splitted )
    {
        dataList[0] = tmpData + dataList[0];
        tmpData.clear();
        splitted = false;
    }

    foreach( QByteArray data, dataList )
    {
        int len = 0;
        int type = 0;
        if( data.length() > 0 )
        {
            readHeader(&data,&len,&type);
            QByteArray body = data.mid(sizeof(EC_HEADER));
            qDebug() << "Header.Len: " << len << "data.length:" << body.length();
            if( len == body.length() )
            {
                readBody(&body,len,type);
            }
            else
            {
                splitted = true;
                tmpData = data;
            }
        }
        qDebug() << "type: " << type << "data size: " << data.size();
    }
    qDebug() << "----------------------------------------------";

    //qDebug() << *(data.end());


    //splitMessage(&data);
}

int HatchMessenger::readBody( int len, int type )
{
    QByteArray data = client->read( len+1 );


    foreach(MessageReader* reader, readerList)
    {
        if( reader->getMsgType() == type )
            reader->read(&data);
    }

   // if( type != 3000 )
    //char end;
    //if( type != 4167 ) client->read(&end,1);

    //if( type == ENUM_ANSWER_RDT_LOGIN_VESSEL_POW_INFO_4130 )
        //sendMessage4067();

    //qDebug() << ":::::::::: " << end;

    return data.length();
}

int HatchMessenger::readBody(QByteArray *data, int len, int type)
{
    foreach(MessageReader* reader, readerList)
    {
        if( reader->getMsgType() == type )
            reader->read(data);
    }

    return data->size();
}

void HatchMessenger::readHeader(QByteArray *data, int* len, int* type )
{
    QByteArray headerData = data->left(sizeof(EC_HEADER));

    QDataStream msgStream(&headerData,QIODevice::ReadWrite);
    msgStream.setByteOrder(QDataStream::LittleEndian);

    EC_HEADER header = {0,0,0,0};
    msgStream >> header.load_len;
    msgStream >> header.msgType;
    msgStream >> header.msgId;
    msgStream >> header.sevId;

    //qDebug() << "Reader Msg type:" << header.msgType << header.load_len;

    *len = header.load_len;
    *type = header.msgType;

    //data->remove(0,sizeof(EC_HEADER));
}

void HatchMessenger::readHeader( int* len, int* type )
{
    QByteArray data = client->read(sizeof(EC_HEADER));
    QDataStream msgStream(&data,QIODevice::ReadWrite);
    msgStream.setByteOrder(QDataStream::LittleEndian);

    //char end = 255;

    EC_HEADER header = {0,0,0,0};
    msgStream >> header.load_len;
    msgStream >> header.msgType;
    msgStream >> header.msgId;
    msgStream >> header.sevId;

    //if( header.msgType == 3000 )
    //    client->read(1); // get rid of 255/255

    qDebug() << "Reader Msg type:" << header.msgType << header.load_len;

    *len = header.load_len;
    *type = header.msgType;
}

void HatchMessenger::connectToHost()
{
    qDebug() << "begin";
    QVariant va;
    client->setSocketOption(QAbstractSocket::KeepAliveOption, va);
    client->abort();
    client->close();
    client->connectToHost(HOST, 5555);



    if( client->waitForConnected() )
        qDebug() << "connected";
    else
    {
        QThread::sleep(3);
        emit client->disconnected();
    }



}


void HatchMessenger::stateChanged(QAbstractSocket::SocketState socketState)
{
    qDebug() << "status changes to " << socketState;
}

void HatchMessenger::connectionError(QAbstractSocket::SocketError socketError)
{
    qDebug() << socketError;
}

QByteArray HatchMessenger::buildPackage(quint16 msgType, quint16 msgId, const char *msg)
{
    EC_HEADER echeader={0,0,0,0};
    echeader.load_len = static_cast<quint16>(strlen(msg));
    echeader.msgType = msgType;
    memcpy(&(echeader.msgId),&msgId,sizeof(quint16));
    echeader.sevId = 0;
    quint32 len = echeader.load_len+sizeof(EC_HEADER)+1;

    quint8* pPack = new quint8[len];
    memcpy(pPack,&echeader,sizeof(EC_HEADER));
    memcpy(pPack+sizeof(EC_HEADER),msg,echeader.load_len);
    memset(pPack+len-1,0xff,1);

    QByteArray ba;
    QDataStream stream(&ba,QIODevice::WriteOnly);

    for(quint32 i = 0; i < len; i++)
    {
        stream << pPack[i];
    }

    //qDebug() <<  client->write(ba);

    delete[] pPack;
    pPack = NULL;

    return ba;
}


void HatchMessenger::initSendList()
{
    QTimer* timer = new QTimer(this);
    timer->setInterval(1000);
    timer->start();
    connect(timer, &QTimer::timeout,[=](){
        if( !outlist.isEmpty() )
        {
            qDebug() << outlist.first();
            outlist.pop_front();
        }
        QByteArray data = buildPackage(4000,1,"hello");

        outlist.push_back(data);
    });
}

QString HatchMessenger::getShipFileName(const QString &vesselClass)
{

    QDir dir(user.sharedPath + "ship_files");

    QStringList fileList =  dir.entryList(QStringList("*35TJ*.nsd"));
    if( fileList.size() )
        return fileList.at(0);

    return QString("");
}

int HatchMessenger::loadShipData(CShipData *pShipData, const QString &shipFile)
{
    FILE *fp;
    //char tmp[1024] = {0};
    //sprintf(tmp,"%sship_files\\%s", user.sharedPath.toStdString().c_str(),shipFile);
    QString filePath;
    filePath = user.sharedPath + "ship_files/" + shipFile;
    fp = fopen(filePath.toStdString().c_str(),"rb");
    if (fp==NULL)
    {
      return(-1);
    }


    int rtn;
    rtn = pShipData->ImportShipData(fp,(char*)(filePath.toStdString().c_str()));
    fclose(fp);

    qDebug() << shipData.m_head.usNStks;
    return rtn;
}

void HatchMessenger::loadSlotGroups()
{
    int layer = 1;
    for( int i = 0; i < 2*(shipData.m_head.usBays); i++ )
    {
        int maxTop = 0;
        int minBottom = 999;

        int bay = i/2+1;
        for( int j = 0; j < shipData.m_head.usNStks; j++)
        {
            if( shipData.m_pStack[j].usBay == bay
                && shipData.m_pStack[j].usLayer == layer )
            {
                int height = shipData.m_pStack[j].usTTier;
                maxTop = height>maxTop?height:maxTop;

                int bottom = shipData.m_pStack[j].usBTier;
                minBottom = bottom<minBottom?bottom:minBottom;
            }
        }

        SlotGroup* group = new SlotGroup;
        int height = maxTop - minBottom + 1;

        group->bayLayer = layer;
        group->bayNo = bay*2-1;

        for( int j = 0; j < shipData.m_head.usNStks; j++)
        {
            if( shipData.m_pStack[j].usBay == bay
                && shipData.m_pStack[j].usLayer == layer )
            {
                int top = shipData.m_pStack[j].usTTier;
                int bottom = shipData.m_pStack[j].usBTier;

                int bottomGap = bottom - minBottom;
                int topGap = maxTop - top;

                QList<Slot*> column;

                while( topGap > 0 )
                {
                    column.push_back(new Slot(false,group));
                    --topGap;
                }

                for( int k = bottom; k < top+1; k++ )
                {
                    column.push_back(new Slot(true,group));
                }

                while( bottomGap > 0 )
                {
                    column.push_back(new Slot(false,group));
                    --bottomGap;
                }

                int stack = shipData.m_pStack[j].usStack;

                if( stack&1 != 0 )
                {
                    group->columns.push_back(column);
                }
                else
                {
                    group->columns.push_front(column);
                }
            }
        }

        slotGroups.push_back(group);

        layer = (layer==1) ? 2:1;
    }

    /// useful debug info
    /*
    foreach( SlotGroup* group, slotGroups )
    {
        qDebug() << "------------------------------";
        qDebug() << QString::number(group->bayNo) + " " + QString::number(group->bayLayer) + " " + QString::number(group->columns.count());
        foreach( QList<Slot*> column, group->columns )
        {
            QString str;
            foreach( Slot* slot, column )
            {
                if( slot->isVisiable )
                    str += "1 ";
                else
                    str += "0 ";
            }
            qDebug() << str;
        }
        qDebug() << "------------------------------";

    }
    */
}

void HatchMessenger::sendMessage(quint16 msgType, const QString &msg,ENUM_SEND_MODE sendmode)
{
    if( msgType != 4002 )
        qDebug() << "Sending msg: " << msgType << msg;
    QByteArray data = buildPackage(msgType,1,msg.toStdString().c_str());
    client->write(data);
}

void HatchMessenger::sendHeartBeat(const QString& rdtId, const QString& userId)
{
    //QString rdtId = "RHX";
    //QString userId = "RHX123";
    QString msg = rdtId.toUpper() + "*3*" + userId.toUpper() + "*N*0*nogps*";


    QTimer* timer = new QTimer;
    timer->setInterval(10000);
    timer->start();
    connect(timer, &QTimer::timeout,[=](){
        static int heartbeatno = 0;
        //qDebug() << "send heartbeat no." << ++heartbeatno;
        sendMessage(ENUM_QUERY_RDT_STATUS_4002,msg);
    });

}

void HatchMessenger::sendMessage4000(const QString &rdtId)
{
    QString msg = rdtId + "*1*0*";
    //QString msg = "RH01*1*0*";

    sendMessage(ENUM_QUERY_RDT_PARAM_4000,msg.toUpper());
}

void HatchMessenger::sendMessage4001(const QString &rdtId, const QString &userId, const QString &password)
{
    //RDTID*LOGTYPE*USERNAME*PASSWORD
    QString msg = rdtId.toUpper() + "*7*" + userId + "*" + password + "*";
    sendMessage(ENUM_QUERY_RDT_LOGIN_4001,msg);
}

void HatchMessenger::sendMessage4004(const QString& pow, const QString& vesselRef,
                                     const QString& craneId, const QString& bundleId)
{
    //RDTID**VesselRefNo*USERNAME*CraneUserId*BundleId
    QString msg = QString("RHX") + "*" + pow.toUpper() + "*" + vesselRef.toUpper() +
                  "*" + "RHX123" +"*" + craneId + "*" + bundleId + "*";
    sendMessage(ENUM_QUERY_RDT_POINT_AND_VESSEL_LOGIN_4004,msg);
}

void HatchMessenger::sendMessage4069()
{
    QString msg = "RH01*EASMG1*";
    sendMessage(ENUM_QUERY_RDT_RH_VESSEL_TK_REFRESH_4069, msg);
}

void HatchMessenger::sendMessage4067( const QString& start, const QString& end )
{
    QString msg;
    msg += user.rdtId.toUpper() + "*" +
           user.vesselRef.toUpper() + "*" +
           start + "*" +  end + "*";

    //QString msg = "RH01*EASMG1*1*3*";
    sendMessage(ENUM_QUERY_RDT_BAY_STOWAGE_4067, msg);
}
