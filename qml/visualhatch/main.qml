import QtQuick 2.2
import QtQuick.Window 2.1

Item
{
    objectName: "rootObject"
    id:root
    property int opacitypopup: 1


    width:   Screen.desktopAvailableWidth/2
    height:  Screen.desktopAvailableHeight/2
    //width:400
    //height:300
    opacity: 1

    signal message4000(string msg)
    signal message4001(string rdtId, string userId, string password)
    signal message4004(string pow, string vessel, string craneId, string bundleId)

    function emitMessage4000(data){
        if( data.length )
        {
            message4000(data);
            console.log("Emit 4000:" + data);
        }
    }

    function emitMessage4001(rdtId, userId, password) {
        if(rdtId.length && userId.length && password.length ){
            message4001(rdtId, userId, password);
            console.log("Emit 4001: " + rdtId + " " + userId + " " + password );
        }
    }

    function emitMessage4004( pow, vessel, craneId, bundleId, callback){

        if( pow.length && vessel.length )
        {
            message4004( pow, vessel, craneId, bundleId )
            console.log("Emit 4004: " + pow + " " + vessel + " " + craneId + " " + bundleId);
            return;
        }
        if( pow.length === 0 )
            callback("Pow name");
        else if( vessel.length === 0 )
            callback("Vessel Reference");
    }

    //PopupDialog{}
    WorkPointDialog{}
    Component.onCompleted: {
        Qt.createComponent("PopupDialog.qml").createObject(root,{})
    }

}

