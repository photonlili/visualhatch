import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.2

Item
{
    id: mainItem
    anchors.fill: parent

    Rectangle
    {
        objectName: "rootRect"
        id: rootRect
        opacity: parent.opacity
        color:"#FFFFFF"
        width: parent.width
        height:parent.height

        property bool left_menu_shown: false
        property bool right_menu_shown: false

        // MENU RECT
        Rectangle {
            id: menu_rect
            width:parent.width
            height:parent.height
            anchors.fill: parent
            color: "#242629"
            //anchors.left: parent.left
            //opacity: rootRect.menu_shown ? 1:0

            Behavior on opacity { NumberAnimation { duration:500 } }

            Rectangle {
                id: left_menu_rect
                x:0
                y:0
                width: parent.width/2
                height: parent.height
                color: "#1F252B"

                Column {
                   id: menu_layout
                   anchors.fill: parent
                   anchors.left: parent.left
                   anchors.leftMargin: rootRect.width*0.03
                   anchors.rightMargin: 20
                   anchors.horizontalCenter: parent.horizontalCenter
                   anchors.topMargin: 35
                   anchors.centerIn: parent.Center
                   width: rootRect.width * 0.22


                   spacing: 10

                   TextField {
                       id: menu_pow_input
                       placeholderText: "Pow Name"
                       height:20
                       width: rootRect.width * 0.22
                       onAccepted: {
                            root.emitMessage4004(menu_pow_input.text,
                                                 menu_vessel_input.text,
                                                 menu_crane_input.text,
                                                 menu_bundle_input.text,function(msg){
                                                     console.log("Invalid input: " + msg)
                                                 })
                           rootRect.onMenuLeft()
                       }
                   }

                   TextField {
                       id: menu_vessel_input
                       placeholderText: "Vessel Reference"
                       height:20
                       width: rootRect.width * 0.22
                       onAccepted: {
                           root.emitMessage4004(menu_pow_input.text,
                                                menu_vessel_input.text,
                                                menu_crane_input.text,
                                                menu_bundle_input.text, function(msg){
                                                    console.log("Invalid input: " + msg)
                                                })
                           rootRect.onMenuLeft()
                       }
                   }

                   TextField {
                       id: menu_crane_input
                       placeholderText: "Crane id"
                       height:20
                       width: rootRect.width * 0.22
                       onAccepted: {
                           root.emitMessage4004(menu_pow_input.text,
                                                menu_vessel_input.text,
                                                menu_crane_input.text,
                                                menu_bundle_input.text,function(msg){
                                                    console.log("Invalid input: " + msg)
                                                })
                           rootRect.onMenuLeft()
                       }
                   }

                   TextField {
                       id: menu_bundle_input
                       placeholderText: "Bundle id"
                       height:20
                       width: rootRect.width * 0.22
                       onAccepted: {
                           root.emitMessage4004(menu_pow_input.text,
                                                menu_vessel_input.text,
                                                menu_crane_input.text,
                                                menu_bundle_input.text,function(msg){
                                                    console.log("Invalid input: " + msg)
                                                })
                           rootRect.onMenuLeft()
                       }
                   }

                }



            }

            Rectangle {
                id: right_menu_rect
                width: parent.width/2
                height: parent.height
                color: "#07376E"
                anchors.left: left_menu_rect.right

            }


        }


        // RIGHT MENU RECT


        // VIEW Rect
        Rectangle {
            //visible: false
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
                width: parent.width; height: 40;
                color: "#83BDF7"


                Rectangle {
                    id: menu_left_button
                    radius: 2
                    anchors {left: parent.left; verticalCenter: parent.verticalCenter; margins: 24 }
                    color: "white"; width: 28; height: 28; smooth: true
                    //Text { anchors.centerIn: parent; font.pixelSize: 28; text: "!" }
                    Column {
                        anchors.fill: parent
                        anchors.centerIn: parent
                        anchors.leftMargin: 6
                        anchors.topMargin: 7
                        spacing: 4
                        Rectangle{ height:2;width:15;color:"black"; }
                        Rectangle{ height:2;width:15;color:"black"; }
                        Rectangle{ height:2;width:15;color:"black"; }
                    }

                    scale: menu_button_mouse.pressed ? 1.1:1.0
                    Component.onCompleted: console.log("menu completed")
                    MouseArea {
                        id: menu_button_mouse
                        anchors.fill: parent
                        onClicked: {
                            rootRect.onMenuLeft()
                        }

                    }
                }

                Rectangle {
                    id: menu_right_button
                    radius: 2
                    anchors {right: parent.right; verticalCenter: parent.verticalCenter; margins: 24 }
                    color: "white"; width: 28; height: 28; smooth: true
                    //Text { anchors.centerIn: parent; font.pixelSize: 28; text: "!" }
                    Column {
                        anchors.fill: parent
                        anchors.centerIn: parent
                        anchors.leftMargin: 6
                        anchors.topMargin: 7
                        spacing: 4
                        Rectangle{ height:2;width:15;color:"black"; }
                        Rectangle{ height:2;width:15;color:"black"; }
                        Rectangle{ height:2;width:15;color:"black"; }
                    }

                    scale: menu_right_button_mouse.pressed ? 1.1:1.0
                    Component.onCompleted: console.log("menu completed")
                    MouseArea {
                        id: menu_right_button_mouse
                        anchors.fill: parent
                        onClicked: {
                            rootRect.onMenuRight()
                        }

                    }
                }
            }

            PinchArea {
                property real  g_scale
                anchors.fill: parent
                pinch.target: main_view
                enabled: true
                onPinchUpdated: {
                    //slot_container.scale = pinch.scale
                    //console.log(g_scale)
//                    console.log((pinch.center.x - pinch.previousCenter.x));
//                    if((pinch.center.x - pinch.previousCenter.x) > 1) {
//                        rootRect.onMenuLeft()
//                    }
                }

                property real startedX;
                property real endedX;
                onPinchStarted: {
                    startedX = pinch.center.x
                }
                onPinchFinished: {
                    endedX = pinch.center.x
                    console.log(endedX-startedX)
                    if( ( endedX - startedX ) > 1 )
                        rootRect.onMenuLeft();
                }
            }

            Rectangle {
                id: slot_container
                anchors.fill: parent
                anchors.top: menu_bar.bottom
                anchors.topMargin: 40
                anchors.leftMargin: 15
                anchors.rightMargin: 15
                anchors.bottomMargin: 15

                border.color: "green"

                width: 50
                height: 50

                Column {
                    spacing: 6
                    Rectangle { color: "red"; width: 50; height: 50 }
                        Rectangle {
                            id: greenRect; color: "green"; width: 20; height: 50

                        }
                        Rectangle { color: "blue"; width: 50; height: 20 }
                        focus: true
                        Keys.onSpacePressed: greenRect.visible = !greenRect.visible
                }

                Rectangle {
                    id: test_rect
                    width:20
                    height:20
                    border.color: "red"
                    anchors.centerIn: parent
                    scale: parent.scale
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

        function onMenuLeft() {
            game_translate_.x = rootRect.left_menu_shown ? 0 : rootRect.width * 0.28
            rootRect.left_menu_shown = !rootRect.left_menu_shown;
        }

        function onMenuRight() {
            game_translate_.x = rootRect.right_menu_shown ? 0 : -rootRect.width * 0.28
            rootRect.right_menu_shown = !rootRect.right_menu_shown
        }

    }
}
