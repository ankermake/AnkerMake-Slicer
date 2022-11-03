import QtQuick 2.0
import QtQuick.Controls 1.5

import "../BaseControl" as BaseControl
Rectangle {
    id:root
    implicitHeight: 30
    implicitWidth: 460
    color: "transparent"

    property alias lineLabel: label.text
    property bool switchIsChecked : false
    property bool switchEnable: true
    signal switchStateChanged(bool state)
    Label {
        id:label
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        verticalAlignment: Text.AlignVCenter
        color: "#FFFFFF"
    }

    BaseControl.SwitchButton {
        id: switchButton
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 5
        enabled: switchEnable
        isChecked: switchIsChecked
        onCheckedStateChanged:  {
            switchStateChanged(state)
        }
    }
}
