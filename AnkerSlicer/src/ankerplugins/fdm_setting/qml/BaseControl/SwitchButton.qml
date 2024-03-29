import QtQuick 2.0

Rectangle {
    id:root
    width: 41
    height: 20
    opacity: 1
    radius: 10
     antialiasing: true;
    color: isChecked ? (root.enabled ? "#61D37D" :"#306C3F") : "#515151"

    signal checkedStateChanged(bool state)
    property bool  isChecked: false
    Rectangle {
        id:sliderLeft
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        radius: 12
        height: root.height
        width: sliderLeft.height
         antialiasing: true;
        anchors.margins: 2
        color: root.enabled ? "#DBDBDB" : "#747474"
        visible: isChecked ? false :true

    }
    Rectangle {
        id:sliderRight
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        radius: 12
        height: root.height
        width: sliderRight.height
         antialiasing: true;
        anchors.margins: 2
        color: root.enabled ? "#FFFFFF" : "#7CAD88"
        visible:  isChecked ? true :false

    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            mouse.accepted = false
            checkedStateChanged(isChecked)
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:4}
}
##^##*/
