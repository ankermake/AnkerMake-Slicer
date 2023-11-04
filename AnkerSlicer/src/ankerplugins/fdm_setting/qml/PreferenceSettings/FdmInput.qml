import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import "../BaseControl" as BaseControl
Rectangle {
    id:root
    implicitHeight: 30
    implicitWidth: 460
    color: "transparent"
    anchors.margins: 0

    property alias lineLabel: label.text
    property alias value: input.textInput
    property alias unit: input.textLabel
    property string type
    property alias inputEnable:input.enabled
    signal textEditFinished(string value)
    RowLayout {
        anchors.fill: parent
        anchors.margins: 0
        Label {
            id:label
            Layout.fillWidth: true
            Layout.margins: 0
            verticalAlignment: Text.AlignVCenter
            Layout.fillHeight: true
            anchors.margins: 0
            color: "#FFFFFF"
        }
        BaseControl.BaseTextInput{
            id: input
            //anchors.right: parent.right
            anchors.margins: 0
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            Layout.fillHeight: true
            inputValidator:RegExpValidator{regExp: /^-?[0-9]{0,5}[.,]?[0-9]{0,3}$/ }
            onInputTextEdited: {
                //console.log("textInput === ", textInput, "vvvvalue = ", value, "disPlayTextInput =", input.disPlayTextInput)
                if(textInput.length  <= 0) {
                    textEditFinished("0")
                }else {
                    textEditFinished(textInput)
                }
            }
        }
    }
}
