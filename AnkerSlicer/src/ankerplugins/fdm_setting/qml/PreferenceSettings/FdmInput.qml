import QtQuick 2.15
import QtQuick.Controls 2.15

import "../BaseControl" as BaseControl
Rectangle {
    id:root
    implicitHeight: 30
    implicitWidth: 460
    color: "transparent"
   // radius: 4

    property alias lineLabel: label.text
    property alias value: input.textInput
    property alias unit: input.textLabel
    property string type
    property alias inputEnable:input.enabled
    signal textEditFinished(string value)
    Label {
        id:label
        anchors.left: parent.left
        verticalAlignment: Text.AlignVCenter
        anchors.margins: 0
        color: "#FFFFFF"
    }
    BaseControl.BaseTextInput{
        id: input
        anchors.right: parent.right
        anchors.margins: 0
//        onInputEditFinished: {
//            textEditFinished(textInput)
//        }
        onInputTextEdited: {
            textEditFinished(textInput)
        }
    }
}
