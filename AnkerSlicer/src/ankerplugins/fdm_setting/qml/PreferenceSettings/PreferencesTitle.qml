import QtQuick 2.0

Rectangle {
    id: root
    implicitWidth: 300
    implicitHeight:20
    color: "transparent"
    anchors.margins: 0
    property bool expand : false
    property alias text : title.text
    signal expandCurrent(bool expand)
    Text {
        id: title
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        horizontalAlignment: Text.AlignLeft
        font.weight: Font.Bold
        color: "#FFFFFF"
    }
    Image {
        id: icon
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        source: expand ? "qrc:/images/images/fdm_an_icon_n.png" : "qrc:/images/images/fdm_pack_icon_n.png"
    }

    MouseArea {
        anchors.fill: parent
        //hoverEnabled: true
        acceptedButtons: Qt.LeftButton
        propagateComposedEvents: true
        onClicked: {
            mouse.accepted = false
            expand = !expand
            expandCurrent(expandCurrent)
        }
    }
}
