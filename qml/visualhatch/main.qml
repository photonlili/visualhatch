import QtQuick 2.2
import QtQuick.Window 2.1

Item
{
    id:root
    property int opacitypopup: 1


    width:   Screen.desktopAvailableWidth/2
    height:  Screen.desktopAvailableHeight/2
    //width:400
    //height:300
    opacity: 1

    signal qmlSignal(string msg)

    function emitMessage4000(data){
        if( data.length )
        {
            qmlSignal(data);
            console.log("Emit:" + data);
        }
    }


    Rectangle
    {
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

