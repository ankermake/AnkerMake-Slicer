import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.15

import "../BaseControl" as BaseControl
Rectangle {
    id:root
    implicitHeight: 16
    implicitWidth: 300


    property alias radioText: radioText.text
    property alias isvalue: valueLabel.text
    property bool isChecked: false
    signal radioButtonClicked()

    Image{
        id:radioIcon
        anchors.left: parent.left
        anchors.leftMargin: 5
        height: 18
        width: 18
        horizontalAlignment: Image.AlignHCenter
        source: isChecked ? "qrc:/images/images/fdm_radio_icon_s.png" : "qrc:/images/images/fdm_radio_icon_n.png"
    }
    Label {
        id:radioText
        anchors.left: radioIcon.right
        //verticalAlignment: Text.AlignVCenter
        anchors.leftMargin: 8
//        color: "#FFFFFF"
    }

    Label {
        text: qsTr("Layer Height")
        anchors.right: valueLabel.left
        verticalAlignment: Text.AlignVCenter
        color: "#999999"

    }
    Label {
        id:valueLabel
        anchors.right: parent.right
        verticalAlignment: Text.AlignVCenter
        color: "#999999"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            radioButtonClicked()
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.5;height:480;width:640}
}
##^##*/
