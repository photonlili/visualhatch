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

    Rectangle
    {
        objectName: "rootRect"
        opacity: parent.opacity
        color:"lightgreen"
        width: parent.width
        height:parent.height

        Text
        {
            anchors.centerIn: parent

            font.family: "Segoe UI Light"
            font.pixelSize: 20
            text : "Hello World!"
            color: "black"
        }
        MouseArea
        {
            anchors.fill: parent

            onClicked:
            {
                console.log("root clicked");
                opacitypopup = 1

                Qt.createComponent("PopupDialog.qml").createObject(root,{})
            }
        }


        Component.onCompleted:
        {
      //      Qt.createComponent("PopupDialog.qml").createObject(root,{"id":"hello"})
        }

        PopupDialog{

        }

    }
}

