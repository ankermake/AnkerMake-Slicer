import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "../BaseControl" as BaseControl
Rectangle {
    id : root
    implicitWidth: 127
    implicitHeight: 55
    anchors.margins: 0
    visible: true
    color:  "transparent"

    property alias title: label.text
    property alias comBoxModel: combox.model
    property alias currentText: combox.currentText
    property alias currentIndex: combox.currentIndex
    property alias buttonVisible: button.visible
    property alias buttonAllVisble:combox.buttonAllVisble
    property alias preferencesBurronVisible: combox.buttonVisible
    signal comboxActived()
    signal comboxButtonClicked(string name)
    signal resetParameters();
    Label {
        id: label
        anchors.left: parent.left
        anchors.top: parent.top
        font.pixelSize: 14
        color: "#FFFFFF"
    }
    RowLayout{
        id: printer
        anchors.left: parent.left
        anchors.top: label.bottom
        anchors.right: parent.right
        anchors.topMargin: 8

        BaseControl.BaseComboBox{
            id: combox
            anchors.margins: 5
            Layout.fillWidth: true
            Layout.fillHeight: true
            objectName: "Combox"
            onActivated: {
                comboxActived()
            }
            onButtonClicked: {
                comboxButtonClicked(name);
            }

        }
        Button{
            id:button
            implicitHeight: 24
            width: height
            visible: false
            background: Image {
                id: name
                fillMode:Image.PreserveAspectFit
                width: implicitHeight
                source: "qrc:/images/images/fdm_remakes_icon_n.png"
            }
            onClicked: {
                resetParameters()
            }
        }
    }

}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.5}
}
##^##*/
