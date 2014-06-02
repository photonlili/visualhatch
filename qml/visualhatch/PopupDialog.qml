import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0


Item {

    objectName: "loginPopupDlg"
    id: popupDlg

    width:parent.width
    height:parent.height
    //width:400
    //height:300
    anchors.centerIn: parent
    Behavior on opacity { NumberAnimation { duration: 1000 } }

    PropertyAnimation { target: popupDlg; property: "opacity";
                                     duration: 800; from: 0; to: 1;
                                     easing.type: Easing.InOutQuad ; running: true }


    property alias infomationMsg : infoMessage.text




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
                title: ""
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
                            rdtIdText.readOnly = true
                            root.emitMessage4000(text)
                            infomationMsg = "param done"
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
                        id: passwordText
                        Layout.fillWidth: true
                        placeholderText: "User password"

                        onAccepted: {
                            opacitypopup = 0
                            root.emitMessage4001(rdtIdText.text, userIdText.text, passwordText.text )
                            popupDlg.destroy(1000)
                            //Qt.createComponent("WorkPointDialog.qml").createObject(root,{})
                        }
                    }
                }
            }

            GroupBox{

                Layout.fillWidth: true
                flat: true
                Text{
                    id: infoMessage
                    anchors.centerIn: parent
                    Layout.fillWidth: true
                    text:"hello world"

                }
            }

            GroupBox{

                Layout.fillWidth: true
                flat: true



                Button{
                    //x: 130
                    //y: 0
                    id: loginButton
                    anchors.centerIn: parent

                    text: "Login"
                    transformOrigin: Item.Center

                    onClicked: {
                        opacitypopup = 0
                        root.emitMessage4001(rdtIdText.text, userIdText.text, passwordText.text )
                        popupDlg.destroy(1000)
                        //Qt.createComponent("WorkPointDialog.qml").createObject(root,{})
                    }
                }

            }
        }
    }
}
