import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0


Item {
    id: popupDlg

    width:parent.width
    height:parent.height
    //width:400
    //height:300
    anchors.centerIn: parent
    Behavior on opacity { NumberAnimation {
            duration: 1000
        } }

    PropertyAnimation { target: popupDlg; property: "opacity";
                                     duration: 800; from: 0; to: 1;
                                     easing.type: Easing.InOutQuad ; running: true }





    opacity: opacitypopup



    Rectangle {
        anchors.fill: parent
        anchors.centerIn: parent
        color: "lightsteelblue"
        border.color: "slategrey"

        Behavior on opacity { NumberAnimation { duration: 1000 } }



        ColumnLayout {
            id: mainLayout
            //anchors.fill: parent
            anchors.centerIn: parent

            GroupBox {
                id: rdtLoginBox
                flat:false
                title: "Fill necessary infomation to login"
                Layout.fillWidth: true

                ColumnLayout {

                    id: rdtInputLayout
                    //rows: 3

                    anchors.fill: parent

                    TextField {
                        id: rdtIdText
                        //width: parent.width
                        Layout.fillWidth: true
                        placeholderText: "RDT id"

                        onAccepted:{
                            userIdText.forceActiveFocus()
                            root.emitMessage4000(text)
                        }
                    }

                    TextField {
                        //width: parent.width
                        id:userIdText
                        Layout.fillWidth: true
                        placeholderText: "User id"
                    }
                    TextField {
                        //width: parent.width
                        Layout.fillWidth: true
                        placeholderText: "User password"
                    }
                }
            }

            GroupBox{
                id: loginButtons
                Layout.fillWidth: true
                flat: true
                Button{
                    //x: 130
                    //y: 0
                    anchors.centerIn: parent

                    text: "Login"
                    transformOrigin: Item.Center

                    onClicked: {
                        opacitypopup = 0
                        popupDlg.destroy(1000)
                    }
                }

            }
        }
    }
}
