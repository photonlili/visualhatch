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

    //PopupDialog{}
    WorkPointDialog{}
    Component.onCompleted: {
        Qt.createComponent("PopupDialog.qml").createObject(root,{})
    }

}

