import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id:root
    implicitHeight:96
    implicitWidth:100
    color: "transparent"
    property alias gCodetext: text.text
    property alias gCodeDocument: text.textDocument
    property alias textEnabled: text.enabled
    property color borderColor: "#51545F"

    signal gCodeTextEditFinished()
    ScrollView{
        anchors.fill: parent
        height: 92
        background: Rectangle{
            border.color: borderColor
            border.width: 1
            color: "transparent"
        }
        clip: true
        TextEdit {
            id:text
            anchors.fill: parent

            height: contentHeight
            textMargin: 6
            readOnly: false
            activeFocusOnPress: true
            selectByMouse: true
            color: text.enabled ? "#FFFFFF" : "#999999"
            wrapMode: TextEdit.Wrap
        }
    }

    MouseArea{
        anchors.fill: parent
        hoverEnabled: true
        propagateComposedEvents: true
        onPressed:  {
            mouse.accepted = false
            text.forceActiveFocus();

        }
        onEntered:  {
            if(textEnabled) {
                borderColor = "#61D37D"
            }
        }

        onExited: {
            if(textEnabled) {
                borderColor = "#51545F"
                gCodeTextEditFinished()
            }

        }
    }
}

