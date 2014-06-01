import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Item
{
    id: mainItem
    anchors.fill: parent


    Connections {
        target: messenger
        onSetPOW: {

        }
    }

    Rectangle
    {
        objectName: "rootRect"
        id: rootRect
        opacity: parent.opacity
        color:"lightgreen"
        width: parent.width
        height:parent.height

        property bool menu_shown: false



        // MENU RECT
        Rectangle {
            id: menu_rect
            anchors.fill: parent
            color: "#303030"
            //opacity: rootRect.menu_shown ? 1:0

            Behavior on opacity { NumberAnimation { duration:1000 } }

            ColumnLayout {
                id: menu_layout
                anchors.fill: parent

                TextField {
                    id: menu_pow
                    placeholderText: "RH01"
                    height:20
                    //color:"#F5F5F5"
                    anchors  { left: parent.left; leftMargin: 12; }
                }

                TextField {
                    id: menu_vessel
                    placeholderText: "RH01"
                    height:20
                    //color:"#F5F5F5"
                    anchors  { left: parent.left; leftMargin: 12; }
                }

                TextField {
                    id: menu_bundle
                    placeholderText: "RH01"
                    height:20
                    //color:"#F5F5F5"
                    anchors  { left: parent.left; leftMargin: 12; }
                }
            }


            /* this is my sample menu content (TODO: replace with your own)  */
            /*ListView {
                anchors { fill: parent; margins: 22 }
                model: 8
                delegate: Item { height: 80; width: parent.width;
                    Text { anchors { left: parent.left; leftMargin: 12; verticalCenter: parent.verticalCenter }
                        color: "white"; font.pixelSize: 32; text: "This is menu #" + index  }
                    Rectangle { height: 2; width: parent.width * 0.7; color: "gray"; anchors { horizontalCenter: parent.horizontalCenter; bottom: parent.bottom } }
                }
            }*/

        }

        // VIEW Rect
        Rectangle {
            id: main_view
            anchors.fill: parent
            color: "#FFFFFF"

            /* this is what moves the normal view aside */
            transform: Translate {
                id: game_translate_
                x: 0
                Behavior on x { NumberAnimation { duration: 400; easing.type: Easing.OutQuad } }
            }

            ///MENU BAR
            Rectangle {
                id: menu_bar
                anchors.top: parent.top
                width: parent.width; height: 40; color: "#9095AB"


                Rectangle {
                    id: menu_button
                    anchors {left: parent.left; verticalCenter: parent.verticalCenter; margins: 24 }
                    color: "white"; width: 32; height: 32; smooth: true
                    Text { anchors.centerIn: parent; font.pixelSize: 28; text: "!" }
                    scale: menu_button_mouse.pressed ? 1.1:1.0
                    Component.onCompleted: console.log("menu completed")
                    MouseArea {
                        id: menu_button_mouse
                        anchors.fill: parent
                        onClicked: {
                            rootRect.onMenu()
                        }

                    }
                }
            }

        }


        Component.onCompleted:
        {
      //      Qt.createComponent("PopupDialog.qml").createObject(root,{"id":"hello"})
            //onMenu()
        }

        //PopupDialog{}
        //WorkPointDialog{}

        function onMenu() {
            game_translate_.x = rootRect.menu_shown ? 0 : rootRect.width * 0.28
            rootRect.menu_shown = !rootRect.menu_shown;
        }

    }
}
